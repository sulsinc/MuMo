#ifndef HEADER_network_Lora_hpp_ALREADY_INCLUDED
#define HEADER_network_Lora_hpp_ALREADY_INCLUDED

#include "device_Info.hpp"
#include "message_Version1.hpp"
#include "message_Version2.hpp"
#include <LoRaWan.h>

namespace network { 

    class Lora
    {
    public:
        bool setup(const device::Info &device_info)
        {
            if (!device_info.valid())
                return false;

            //-- Start Lora module --//
            lora.init();

            lora.setId((char *)device_info.device_address, (char *)device_info.device_EUI, (char *)device_info.application_EUI); //devaddr, devEUI, appEUI

            lora.setKey((char *)device_info.network_session_key, (char *)device_info.app_session_key, (char *)device_info.app_key); //nwkskey, appskey, appkey

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
            Serial.println("Lora into sleep modus");
            lora.setDeviceLowPower();     // turn the LoRaWAN module into sleep mode
        }

        void set_message(message::Version1 *msg)
        {
            msg_v1_ = msg;
        }
        void set_message(message::Version2 *msg)
        {
            msg_v2_ = msg;
        }

        unsigned char *rx_data() {return rx_ptr_;}
        unsigned int   rx_size() const {return rx_size_;}

        void process()
        {
            auto try_send_message = [&](auto *&msg){
                if (!msg)
                    return false;

                lora.setPower(20); //Send a command to wake up the lora module
                Serial.println("LoRa Awake!");
                delay(200);

                Serial.println("<<<<<< Sending package to TTN! >>>>>>");
                if (lora.transferPacket((unsigned char *)msg->data(), msg->size())) {
                    short rssi;
                    memset(&rx_, 0, sizeof(rx_));
                    rx_size_ = lora.receivePacket(rx_, sizeof(rx_), &rssi);

                    rx_ptr_ = (rx_size_ > 0) ? (unsigned char *)rx_ : nullptr;
                }

                delay(20);
                set_low_power();

                msg = nullptr;

                return true;
            };

            //We send only one message per time
            if (try_send_message(msg_v1_)) return;
            if (try_send_message(msg_v2_)) return;
        }

    private:
        message::Version1 * msg_v1_ = nullptr;
        message::Version2 * msg_v2_ = nullptr;
        char rx_[256];
        unsigned char *rx_ptr_ = nullptr;
        unsigned int rx_size_ = 0;
    };

} 

#endif
