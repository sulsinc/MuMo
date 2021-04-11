#ifndef HEADER_sensor_SCD30_hpp_ALREADY_INCLUDED
#define HEADER_sensor_SCD30_hpp_ALREADY_INCLUDED

#include "sensor_i2c.hpp"
#include "stat_OutlierDetector.hpp"
#include <Adafruit_Sensor.h>
#include <Adafruit_SCD30.h>

namespace sensor { 

    class SCD30
    {
    public:
        struct Data
        {
            bool is_outlier = false;

            float co2 = -1.00;
            float temperature = -1.00;
            float relative_humidity = -1.00;
        };

        bool valid() const {return valid_;}

        bool setup()
        {
            if (valid_)
                return true;

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

            data.is_outlier = false;
            if (co2_od_.process(data.co2))
                data.is_outlier = true;
            if (temperature_od_.process(data.temperature))
                data.is_outlier = true;
            if (relative_humidity_od_.process(data.relative_humidity))
                data.is_outlier = true;

            return true;
        }

    private:
        bool valid_ = false;
        Adafruit_SCD30 scd_;
        stat::OutlierDetector<float, 10> co2_od_{100};
        stat::OutlierDetector<float, 10> temperature_od_{1};
        stat::OutlierDetector<float, 10> relative_humidity_od_{1};
    };

} 

#endif
