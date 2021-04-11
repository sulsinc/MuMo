#ifndef HEADER_com_Lora_hpp_ALREADY_INCLUDED
#define HEADER_com_Lora_hpp_ALREADY_INCLUDED

#include "com_Ids.hpp"
#include "com_Message.hpp"
#include <LoRaWan.h>

namespace com { 

    class Lora
    {
    public:
        bool setup(const Ids &ids)
        {
            if (!ids.valid())
                return false;

            //Start Lora module
            lora.init();

            lora.setId((char *)ids.device_address, (char *)ids.device_EUI, (char *)ids.application_EUI); //devaddr, devEUI, appEUI

            lora.setKey((char *)ids.network_session_key, (char *)ids.app_session_key, (char *)ids.app_key); //nwkskey, appskey, appkey

            lora.setDeciveMode(LWOTAA);

            lora.setDataRate(DR0, EU868);
            lora.setAdaptiveDataRate(true);

            lora.setChannel(0, 868.1);
            lora.setChannel(1, 868.3);
            lora.setChannel(2, 868.5);
            lora.setChannel(3, 867.1);
            lora.setChannel(4, 867.3);
            lora.setChannel(5, 867.5);
            lora.setChannel(6, 867.7);

            lora.setReceiceWindowFirst(869.5, DR3);
            lora.setReceiceWindowSecond(869.5, DR3);

            lora.setPower(20);

            //Ping the TTN server to JOIN
            //TODO: do not hang forever
            while (!lora.setOTAAJoin(JOIN));

            return true;
        }

        void set_low_power()
        {
            //Turn the LoRaWAN module into sleep mode
            lora.setDeviceLowPower();
        }

        void set_message(Message *msg) { msg_ = msg; }

        unsigned char *rx_data()       {return rx_data_;}
        unsigned int   rx_size() const {return rx_size_;}

        void process()
        {
            if (!msg_)
                return;

            //Wake-up the lora module
            lora.setPower(20);
            delay(200);

            Serial.print("Transferring Lora packet of "); Serial.print(msg_->size()); Serial.println(" bytes.");
            //transferPacket() needs an unsigned char * to work, char * seems to do something else
            if (lora.transferPacket((unsigned char *)msg_->data(), msg_->size())) {
                short rssi;
                memset(&rx_buffer_, 0, sizeof(rx_buffer_));
                rx_size_ = lora.receivePacket(rx_buffer_, sizeof(rx_buffer_), &rssi);

                rx_data_ = (rx_size_ > 0) ? (unsigned char *)rx_buffer_ : nullptr;
            }

            delay(20);
            set_low_power();

            msg_->clear();
            msg_ = nullptr;
        }

    private:
        Message * msg_ = nullptr;
        char rx_buffer_[256u];
        unsigned char *rx_data_ = nullptr;
        unsigned int   rx_size_ = 0;
    };

} 

#endif
