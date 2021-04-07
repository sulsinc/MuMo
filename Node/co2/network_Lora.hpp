#ifndef HEADER_network_Lora_hpp_ALREADY_INCLUDED
#define HEADER_network_Lora_hpp_ALREADY_INCLUDED

#include "message_Version1.hpp"
#define lora lora_do_not_use_global_name
#include <LoRaWan.h>
#undef lora

namespace network { 

    class Lora
    {
    public:
        bool setup()
        {
            //-- Start Lora module --//
            lora_.init();

            lora_.setId(device_address, device_EUI, application_EUI); //devaddr, devEUI, appEUI
            lora_.setKey(network_session_key, app_session_key, app_key); //nwkskey, appskey, appkey

            lora_.setDeciveMode(LWOTAA);

            lora_.setDataRate(DR0, EU868);
            lora_.setAdaptiveDataRate(true);

            lora_.setChannel(0, 868.1);
            lora_.setChannel(1, 868.3);
            lora_.setChannel(2, 868.5);
            lora_.setChannel(3, 867.1);
            lora_.setChannel(4, 867.3);
            lora_.setChannel(5, 867.5);
            lora_.setChannel(6, 867.7);

            lora_.setReceiceWindowFirst(869.5, DR3);
            lora_.setReceiceWindowSecond(869.5, DR3);

            lora_.setPower(20);

            //Ping the TTN server to JOIN
            //TODO: do not hang forever
            while (!lora_.setOTAAJoin(JOIN));

            return true;
        }

        void set_low_power()
        {
            Serial.println("Lora into sleep modus");
            lora_.setDeviceLowPower();     // turn the LoRaWAN module into sleep mode
        }

        void set_message(message::Version1 *msg)
        {
            msg_v1_ = msg;
        }

        unsigned char *rx_data() {return rx_ptr_;}
        unsigned int   rx_size() const {return rx_size_;}

        void process()
        {
            if (!msg_v1_)
                return;

            lora_.setPower(20); //Send a command to wake up the lora module
            Serial.println("LoRa Awake!");
            delay(200);

            Serial.println("<<<<<< Sending package to TTN! >>>>>>");
            if (lora_.transferPacket(msg_v1_->data(), msg_v1_->size())) {
                short rssi;
                memset(&rx_, 0, sizeof(rx_));
                rx_size_ = lora_.receivePacket(rx_, sizeof(rx_), &rssi);

                rx_ptr_ = (rx_size_ > 0) ? (unsigned char *)rx_ : nullptr;
            }

            delay(20);
            set_low_power();
        }

    private:
        LoRaWanClass lora_;
        message::Version1 * msg_v1_ = nullptr;
        char rx_[256];
        unsigned char *rx_ptr_ = nullptr;
        unsigned int rx_size_ = 0;
    };

} 

#endif
