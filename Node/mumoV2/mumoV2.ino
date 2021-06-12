#include "board_LoraWan.hpp"
#include "sensor_BME680.hpp"
#include "sensor_TSL2561.hpp"
#include "sensor_SCD30.hpp"
#include "com_Message.hpp"
#include "com_Lora.hpp"
#include "com_ids_subsoil.hpp"
#include "com_Display.hpp"
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

com::Display display;

void handle(const sensor::BME680::Data &data){}
void handle(const sensor::TSL2561::Data &data){}
void handle(const board::LoraWan::Data &data){}
void handle(const sensor::SCD30::Data &data)
{
    auto draw_co2 = [&](auto &u8g2)
    {
        String msg = "CO2: ";
        msg += data.co2;
        u8g2.drawStr(0, 10, msg.c_str());

        digitalWrite(8, HIGH);
        if (data.co2 <= 500)
            msg = "LOW";
        else if (data.co2 <= 800)
            msg = "MEDIUM";
        else
        {
            msg = "HIGH";
            digitalWrite(8, LOW);
            auto freq = data.co2;
            /* freq = 1000; */
            tone(9, freq, 1000);
        }
        u8g2.drawStr(0, 20, msg.c_str());
    };
    display.show(draw_co2);
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
        handle(data);
    };
    each_sensor(take_one_measurement);

    return is_outlier;
}
com::Message message;

#define use_lora 0
#if use_lora
com::Lora lora_com;
#endif

unsigned int send_message_countdown = 0;

//Delay between different measurements
//Give enough time for all the sensors to take a measurement
//The SCD30 can take no faster than 1 measurement every 2 sec
unsigned int measurement_delay_ms = 2500;

void setup(void) {
    auto setup_sensor = [](auto &sensor, const char *name){
        sensor.setup();
    };
    each_sensor(setup_sensor);

#if use_lora
    const auto ids = com::ids::subsoil();
    lora_com.setup(ids);
#endif

    my_board.led(false);

    delay(measurement_delay_ms);

    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);

    display.setup();
}

void loop(void) {
    Serial.println("");
    Serial.print("mainloop send_message_countdown: "); Serial.println(send_message_countdown);

    Serial.println("Taking measurement");
    {
        const auto is_outlier = take_all_measurements(message);

        if (message.valid())
        {
#if use_lora
#if debug_mode
            message.print();
#endif
            lora_com.set_message(&message);
#endif
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
#if use_lora
        Serial.println("Sending message over Lora");
        lora_com.process();
#endif
        send_message_countdown = 10;
    }
    else
    {
        --send_message_countdown;
    }

#if debug_mode
    //Give enough time for all the sensors to take a measurement
    //The SCD30 can take no faster than 1 measurement every 2 sec
    delay(measurement_delay_ms);
#else
    //Sleep for one minute
    my_board.sleep();
#endif
}
