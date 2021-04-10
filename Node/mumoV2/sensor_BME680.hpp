#ifndef HEADER_sensor_BME680_hpp_ALREADY_INCLUDED
#define HEADER_sensor_BME680_hpp_ALREADY_INCLUDED

#include "sensor_i2c.hpp"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

namespace sensor { 

    class BME680
    {
    public:
        struct Data
        {
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

            return true;
        }

    private:
        bool valid_ = false;
        Adafruit_BME680 bme_;
    };

} 

#endif
