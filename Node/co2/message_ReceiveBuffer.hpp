#ifndef HEADER_message_ReceiveBuffer_hp_ALREADY_INCLUDED
#define HEADER_message_ReceiveBuffer_hp_ALREADY_INCLUDED

namespace message { 

    class ReceiveBuffer
    {
    public:
        char *data() {return buffer_;}
        unsigned int size() {return sizeof(buffer_);}

    private:
        char buffer_[256];
    };

} 

#endif
