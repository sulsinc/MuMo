#include "board_LoraWan.hpp"
#include "sensor_BME680.hpp"
#include "sensor_TSL2561.hpp"
#include "sensor_SCD30.hpp"
#include "com_Message.hpp"
#include "com_Lora.hpp"
#include "com_ids_subsoil.hpp"
#include <type_traits>

#define debug_mode 1

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
    //TODO: even without BME680 attached, the pressure reading is still
    //comming through 713.25
    ftor(bme680, "BME680");
    ftor(tsl2561, "TSL2561");
    ftor(scd30, "SCD30");
}

//Returns true if this measurement is an outlier
bool take_all_measurements(com::Message &message)
{
    bool is_outlier = false;

    auto take_one_measurement = [&](auto &sensor, const char *name){
        if (!sensor.valid())
            return;

        Serial.print(name); Serial.println(" is valid");

        using Sensor = decltype(sensor);
        typename std::remove_reference_t<Sensor>::Data data;
        if (!sensor.measure(data))
        {
            Serial.println("Warning: could not take measurement");
            return;
        }

        message.set(data);
        if (data.is_outlier)
            is_outlier = true;
    };
    each_sensor(take_one_measurement);

    return is_outlier;
}
com::Message message;

com::Lora lora_com;

unsigned int send_message_countdown = 0;

void setup(void) {
    auto setup_sensor = [](auto &sensor, const char *name){
        sensor.setup();
    };
    each_sensor(setup_sensor);

    const auto ids = com::ids::subsoil();
    lora_com.setup(ids);

    my_board.led(false);

    //Give enough time for all the sensors to take a measurement
    //The SCD30 can take no faster than 1 measurement every 2 sec
    delay(2000);
}

void loop(void) {
    Serial.println("");
    Serial.print("mainloop send_message_countdown: "); Serial.println(send_message_countdown);

    Serial.println("Taking measurement");
    {
        const auto is_outlier = take_all_measurements(message);

        if (message.valid())
        {
#if debug_mode
            message.print();
#endif
            lora_com.set_message(&message);
        }

        if (is_outlier)
        {
            Serial.println("Outlier measurement, sending this immediately");
            send_message_countdown = 0;
        }
        else
        {
            Serial.println("Normal measurement");
        }
    }

    if (send_message_countdown == 0)
    {
        Serial.println("Sending message over Lora");
        lora_com.process();
        send_message_countdown = 10;
    }
    else
    {
        --send_message_countdown;
    }

#if debug_mode
    //Give enough time for all the sensors to take a measurement
    //The SCD30 can take no faster than 1 measurement every 2 sec
    delay(2000);
#else
    //Sleep for one minute
    my_board.sleep();
#endif
}
