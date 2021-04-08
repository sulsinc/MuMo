#ifndef HEADER_board_LoraWan_hpp_ALREADY_INCLUDED
#define HEADER_board_LoraWan_hpp_ALREADY_INCLUDED

#include "clock_RealTime.hpp"
#include <LoRaWan.h>

namespace board { 
    class LoraWan
    {
    public:
        struct Data
        {
            unsigned int battery_percentage = 0;
        };

        bool valid() const {return valid_;}

        void setup(unsigned int baudrate = 115200)
        {
            Serial.begin(baudrate);

            // LoRaWan Board hardware settings
            for (unsigned char i = 0; i < 26; i ++) { // Important, set all pins to HIGH to save power during sleeps
                pinMode(i, OUTPUT);
                digitalWrite(i, HIGH);
            }
            pinMode(38, OUTPUT); //Specifically turn pin 38 OUTPUT and HIGH to activate the Grove connectors
            digitalWrite(38, HIGH);

            valid_ = true;
        }

        void led(bool on)
        {
            digitalWrite(LED_BUILTIN, on); //Turn the onboard led off now that we are joined
        }

        void sleep(clock::RealTime &rtc)
        {
            SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
            rtc.deep_sleep();            // bring CPU into deep sleep mode (until woken up by the RTC)
            SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
        }

        bool measure(Data &data)
        {
            if (!valid_)
                return false;

            //Measure battery charge
            {
                //Switch outputs low to do a clean measurement
                pinMode(CHARGE_STATUS_PIN, OUTPUT);
                digitalWrite(CHARGE_STATUS_PIN, LOW);
                delay(50);
                //Measure 3 times and map to percentage
                int raw = analogRead(BATTERY_POWER_PIN) + analogRead(BATTERY_POWER_PIN) + analogRead(BATTERY_POWER_PIN);
                unsigned int percentage = map(raw, 345, 390, 0, 100);
                //Back to input to save power
                pinMode(CHARGE_STATUS_PIN, INPUT);
                //constrain to realistic range for rechargeable batteries
                percentage = constrain(percentage, 0, 105);

                data.battery_percentage = percentage;
            }

            return true;
        }

    private:
        bool valid_ = false;
    };
} 

#endif
