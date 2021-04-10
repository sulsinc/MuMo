#ifndef HEADER_message_Version1_hpp_ALREADY_INCLUDED
#define HEADER_message_Version1_hpp_ALREADY_INCLUDED

#include "board_LoraWan.hpp"
#include "sensor_BME680.hpp"
#include "sensor_TSL2561.hpp"
#include "message_util.hpp"

namespace message { 

    class Version1
    {
    public:
        static constexpr unsigned int version = 1;

        Version1()
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
        template <typename Data>
        bool set(const Data &data)
        {
            return false;
        }

        char *data() { return buffer_; }
        unsigned int size() const { return sizeof(buffer_); }

        void print() const
        {
            Serial.println("Version1 message");
            for (unsigned int i = 0; i < sizeof(buffer_); ++i)
                Serial.println((int)buffer_[i]);
        }

    private:
        char buffer_[10];
    };

} 

#endif
