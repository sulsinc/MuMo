#ifndef HEADER_message_Version2_hpp_ALREADY_INCLUDED
#define HEADER_message_Version2_hpp_ALREADY_INCLUDED

#include "board_LoraWan.hpp"
#include "sensor_BME680.hpp"
#include "sensor_TSL2561.hpp"
#include "sensor_SCD30.hpp"
#include "message_util.hpp"

namespace message { 

    class Version2
    {
    public:
        static constexpr unsigned int version = 2;

        Version2()
        {
            char *ptr = buffer_+0;
            write_uint8(ptr, version);
        }

        bool set(const board::LoraWan::Data &data)
        {
            char *ptr = buffer_+1;
            write_uint8(ptr, data.battery_percentage);
            return true;
        }
        bool set(const sensor::BME680::Data &data)
        {
            char *ptr = buffer_+2;
            write_float8_8(ptr, data.humidity);
            write_float8_8(ptr, data.temperature);
            const unsigned long atmosferic_pressure = 101325; //In Pa
            const unsigned long pressure = (data.pressure - atmosferic_pressure) + 30000;
            write_uint16(ptr, pressure);
            return true;
        }
        bool set(const sensor::TSL2561::Data &data)
        {
            char *ptr = buffer_+8;
            write_uint16(ptr, data.lux);
            return true;
        }
        bool set(const sensor::SCD30::Data &data)
        {
            char *ptr = buffer_+10;
            write_uint16(ptr, data.co2);
            write_float8_8(ptr, data.temperature);
            write_float8_8(ptr, data.relative_humidity);
            return true;
        }

        char *data() { return buffer_; }
        unsigned int size() const { return sizeof(buffer_); }

        void print() const
        {
            Serial.println("Version2 message");
            for (unsigned int ix = 0; ix < sizeof(buffer_); ++ix)
            {
                Serial.print("ix ");
                Serial.print(ix);
                Serial.print(": ");
                Serial.println((int)buffer_[ix]);
            }
        }

    private:
        char buffer_[16];
    };

} 

#endif
