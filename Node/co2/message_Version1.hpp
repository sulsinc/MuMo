#ifndef HEADER_message_Version1_hpp_ALREADY_INCLUDED
#define HEADER_message_Version1_hpp_ALREADY_INCLUDED

#include "sensor_SCD30.hpp"

namespace message { 

    class Version1
    {
    public:
        Version1()
        {
            buffer_[0] = 1;
        }

        void set(const sensor::SCD30::Data &data)
        {
            {
                unsigned int v = data.co2;
                buffer_[10] = (v >> 8);
                buffer_[11] = (v % 256);
            }
            {
                float v = data.temperature;
                buffer_[12] = v;
                buffer_[13] = (v-(int)v)*100;
            }
            {
                float v = data.relative_humidity;
                buffer_[14] = v;
                buffer_[15] = (v-(int)v)*100;
            }
        }

        char *data() { return buffer_; }
        unsigned int size() const { return sizeof(buffer_); }

        void print() const
        {
            for (unsigned int i = 0; i < sizeof(buffer_); ++i)
                Serial.println((int)buffer_[i]);
        }

    private:
        char buffer_[16];
    };

} 

#endif
