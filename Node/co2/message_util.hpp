#ifndef HEADER_message_util_hpp_ALREADY_INCLUDED
#define HEADER_message_util_hpp_ALREADY_INCLUDED

namespace message { 

    inline void write_uint8(char *&ptr, unsigned int v)
    {
        *ptr = v; ++ptr;

    }
    inline void write_uint16(char *&ptr, unsigned int v)
    {
        *ptr = (v >> 8); ++ptr;
        *ptr = (v % 256); ++ptr;
    }
    inline void write_float8_8(char *&ptr, float v)
    {
        const int vi = v;
        *ptr = vi; ++ptr;
        *ptr = (v-vi)*100; ++ptr;
    }
} 

#endif
