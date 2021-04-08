#ifndef HEADER_sensor_TSL2561_hpp_ALREADY_INCLUDED
#define HEADER_sensor_TSL2561_hpp_ALREADY_INCLUDED

#include "sensor_i2c.hpp"
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

namespace sensor { 

    class TSL2561
    {
    public:
        bool valid() const {return valid_;}
        bool setup()
        {
            i2c::setup();

            if (!tsl_.begin())
                return false;

            //-- Set up TSL2561 oversampling and gain --//
            tsl_.setGain(TSL2561_GAIN_16X);
            tsl_.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);

            valid_ = true;
            return true;
        }
    private:
        bool valid_ = false;
        Adafruit_TSL2561_Unified tsl_{0x29, 12345};
    };

} 

#endif