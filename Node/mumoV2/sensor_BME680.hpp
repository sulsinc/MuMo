#ifndef HEADER_sensor_BME680_hpp_ALREADY_INCLUDED
#define HEADER_sensor_BME680_hpp_ALREADY_INCLUDED

#include "sensor_i2c.hpp"
#include "stat_OutlierDetector.hpp"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

namespace sensor { 

    class BME680
    {
    public:
        struct Data
        {
            bool is_outlier = false;

            float temperature = -1.00;
            float humidity = -1.00;
            long pressure = -1;
        };

        bool valid() const {return valid_;}

        bool setup()
        {
            if (valid_)
                return true;

            i2c::setup();

            if (!bme_.begin(0x76))
            {
                Serial.println("Could not find sensor BME680.");
                return false;
            }

            //Set up BME680 oversampling and filter initialization
            bme_.setTemperatureOversampling(BME680_OS_8X); //1,2,4,8 of 16X
            bme_.setHumidityOversampling(BME680_OS_8X); //1,2,4,8 of 16X
            bme_.setPressureOversampling(BME680_OS_4X); //1,2,4,8 of 16X
            bme_.setIIRFilterSize(BME680_FILTER_SIZE_0); //Filter size for the resistance? 0, 1, 3, 7, 15, 31, 63, 127

            //Disabled the GAS heater to save power.
            bme_.setGasHeater(0, 0); // 0*C for 0 ms

            valid_ = true;
            return true;
        }

        bool measure(Data &data)
        {
            if (!valid_)
                return false;

            if (!bme_.performReading())
                return false;
            data.temperature = bme_.temperature;
            data.humidity = bme_.humidity;
            data.pressure = bme_.pressure;

            data.is_outlier = false;
            if (temperature_od_.process(data.temperature))
                data.is_outlier = true;
            if (humidity_od_.process(data.humidity))
                data.is_outlier = true;

            return true;
        }

    private:
        bool valid_ = false;
        Adafruit_BME680 bme_;
        stat::OutlierDetector<float, 10> temperature_od_{1};
        stat::OutlierDetector<float, 10> humidity_od_{1};
    };

} 

#endif
