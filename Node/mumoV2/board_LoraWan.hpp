#ifndef HEADER_board_LoraWan_hpp_ALREADY_INCLUDED
#define HEADER_board_LoraWan_hpp_ALREADY_INCLUDED

#include <LoRaWan.h>
#include <RTCZero.h>

namespace board { 

    class LoraWan
    {
    public:
        struct Data
        {
            unsigned int battery_percentage = 0u;
        };

        bool valid() const {return valid_;}

        void setup(unsigned int baudrate = 115200u)
        {
            if (valid_)
                return;

            Serial.begin(baudrate);

            //Set all pins to HIGH to save power during sleeps
            for (unsigned int pin = 0; pin < 26; ++pin)
            {
                pinMode(pin, OUTPUT);
                digitalWrite(pin, HIGH);
            }

            //Specifically turn pin 38 OUTPUT and HIGH to activate the Grove connectors
            pinMode(38u, OUTPUT);
            digitalWrite(38u, HIGH);

            //Setup real-time clock
            rtc0_.begin(false);
            next_alarm_clock_ = rtc0_.getEpoch() + 60; // calculate the time of the first alarm (in one minute)
            rtc0_.setAlarmEpoch(next_alarm_clock_);
            rtc0_.enableAlarm(rtc0_.MATCH_SS); //check the alarm based on seconds, so we wake up every minute

            valid_ = true;
        }

        void led(bool on)
        {
            digitalWrite(LED_BUILTIN, on); //Turn the onboard led off now that we are joined
        }

        void sleep()
        {
            SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
            //Bring CPU into deep sleep mode (until woken up by the RTC)
            rtc0_.standbyMode();
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
        RTCZero rtc0_;
        unsigned long next_alarm_clock_ = 0u;
    };

} 

#endif
