#ifndef HEADER_sensor_i2c_hpp_ALREADY_INCLUDED
#define HEADER_sensor_i2c_hpp_ALREADY_INCLUDED

#include <Wire.h>

namespace sensor { 

    namespace i2c { 

        inline void setup()
        {
            static bool s_init = false;
            if (!s_init)
            {
                Wire.begin();
                s_init = true;
            }
        }

    } 

} 

#endif
