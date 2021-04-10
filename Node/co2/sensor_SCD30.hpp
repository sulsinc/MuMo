#ifndef HEADER_sensor_SCD30_hpp_ALREADY_INCLUDED
#define HEADER_sensor_SCD30_hpp_ALREADY_INCLUDED

#include "sensor_i2c.hpp"
#include <Adafruit_Sensor.h>
#include <Adafruit_SCD30.h>

namespace sensor { 

    class SCD30
    {
    public:
        struct Data
        {
            float co2 = -1.00;
            float temperature = -1.00;
            float relative_humidity = -1.00;
        };

        bool valid() const {return valid_;}
        bool setup()
        {
            i2c::setup();

            if (!scd_.begin())
            {
                Serial.println("Could not find sensor SCD30.");
                return false;
            }

            valid_ = true;
            return true;
        }

        bool measure(Data &data)
        {
            if (!scd_.dataReady())
                return false;
            if (!scd_.read())
                return false;
            data.co2 = scd_.CO2;
            data.temperature = scd_.temperature;
            data.relative_humidity = scd_.relative_humidity;
            return true;
        }

    private:
        bool valid_ = false;
        Adafruit_SCD30 scd_;
    };

} 

#endif
