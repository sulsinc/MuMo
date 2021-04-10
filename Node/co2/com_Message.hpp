#ifndef HEADER_com_Message_hpp_ALREADY_INCLUDED
#define HEADER_com_Message_hpp_ALREADY_INCLUDED

#include "board_LoraWan.hpp"
#include "sensor_BME680.hpp"
#include "sensor_TSL2561.hpp"
#include "sensor_SCD30.hpp"

namespace com { 

    class Message
    {
    public:
        Message()
        {
            clear();
        }

        void clear()
        {
            memset(&buffer_, 0u, sizeof(buffer_));
        }

        const unsigned char version() const {return buffer_[0u];}

        bool valid() const {return version() > 0u;}

        bool set(const board::LoraWan::Data &data)
        {
            set_minimal_version_(1u);

            unsigned int ix = 1u;
            write_uint8_(data.battery_percentage, ix);

            return true;
        }
        bool set(const sensor::BME680::Data &data)
        {
            set_minimal_version_(1u);

            unsigned int ix = 2u;
            write_float8_8_(data.humidity, ix);
            write_float8_8_(data.temperature, ix);
            const unsigned long atmosferic_pressure = 101325; //In Pa
            const unsigned long pressure = (data.pressure - atmosferic_pressure) + 30000;
            write_uint16_(pressure, ix);

            return true;
        }
        bool set(const sensor::TSL2561::Data &data)
        {
            set_minimal_version_(1u);

            unsigned int ix = 8u;
            write_uint16_(data.lux, ix);

            return true;
        }
        bool set(const sensor::SCD30::Data &data)
        {
            set_minimal_version_(2u);

            unsigned int ix = 10u;
            write_uint16_(data.co2, ix);
            write_float8_8_(data.temperature, ix);
            write_float8_8_(data.relative_humidity, ix);

            return true;
        }

        unsigned char *data()
        {
            switch (version())
            {
                case 1u: break;
                case 2u: break;
                default: return nullptr; break;
            }
            return buffer_;
        }
        unsigned int size() const
        {
            switch (version())
            {
                case 1u: return 10u; break;
                case 2u: return 16u; break;
                default: break;
            }
            return 0u;
        }

        void print() const
        {
            Serial.println("Message buffer");
            for (unsigned int ix = 0; ix < sizeof(buffer_); ++ix)
            {
                Serial.print("buffer[");
                Serial.print(ix);
                Serial.print("] = ");
                Serial.println((int)buffer_[ix]);
            }
        }

    private:
        void set_minimal_version_(unsigned char minimal_version)
        {
            unsigned char &version = buffer_[0u];
            if (minimal_version > version)
                version = minimal_version;
        }
        void write_uint8_(unsigned int v, unsigned int &ix)
        {
            buffer_[ix++] = v;
        }
        void write_uint16_(unsigned int v, unsigned int &ix)
        {
            buffer_[ix++] = (v >> 8);
            buffer_[ix++] = (v % 256);
        }
        void write_float8_8_(float v, unsigned int &ix)
        {
            const int vi = v;
            buffer_[ix++] = vi;
            buffer_[ix++] = (v-vi)*100;
        }

        unsigned char buffer_[16u];
    };

} 

#endif
