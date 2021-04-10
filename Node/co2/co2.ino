#include "board_LoraWan.hpp"
#include "sensor_BME680.hpp"
#include "sensor_TSL2561.hpp"
#include "sensor_SCD30.hpp"
#include "com_Message.hpp"
#include "com_Lora.hpp"
#include "com_ids_subsoil.hpp"
#include <type_traits>

#define debug_mode 0

board::LoraWan my_board;

sensor::BME680 bme680;
sensor::TSL2561 tsl2561;
sensor::SCD30 scd30;

template <typename Ftor>
void each_sensor(Ftor &&ftor)
{
    //Not really a sensor, but we do measure the battery percentage
    ftor(my_board, "Board");

    //Actual sensors
    ftor(bme680, "BME680");
    ftor(tsl2561, "TSL2561");
    ftor(scd30, "SCD30");
}

com::Message message;

void take_all_measurements()
{
    auto take_one_measurement = [&](auto &sensor, const char *name){
        if (!sensor.valid())
            return;

        Serial.print(name); Serial.println(" is valid");

        using Sensor = decltype(sensor);
        typename std::remove_reference_t<Sensor>::Data my_data;
        if (!sensor.measure(my_data))
        {
            Serial.println("Warning: could not take measurement");
            return;
        }

        message.set(my_data);
    };
    each_sensor(take_one_measurement);
}

com::Lora lora_com;

void setup(void) {
    auto setup_sensor = [](auto &sensor, const char *name){
        sensor.setup();
    };
    each_sensor(setup_sensor);

    const auto ids = com::ids::subsoil();
    lora_com.setup(ids);

    my_board.led(false);

    delay(500);
    //perform first measurement on all sensors
    take_all_measurements();
    delay(500);
}

void loop(void) {
    Serial.println("########################## LOOP SEQUENCE #################################");

    lora_com.process();

    for (unsigned int i = 0; i < 10 ; i++)
    {
#if debug_mode
        delay(1000);
#else
        //Sleep for one minure
        my_board.sleep();
#endif

        take_all_measurements();

        if (message.valid())
        {
#if debug_mode
            message.print();
#endif
            lora_com.set_message(&message);
        }
    }
}
