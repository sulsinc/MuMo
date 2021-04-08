#ifndef HEADER_clock_RealTime_hpp_ALREADY_INCLUDED
#define HEADER_clock_RealTime_hpp_ALREADY_INCLUDED

#include <RTCZero.h>

namespace clock { 

    class RealTime
    {
    public:
        void setup()
        {
            rtc0_.begin(false);
            next_alarm_clock_ = rtc0_.getEpoch() + 60; // calculate the time of the first alarm (in one minute)
            rtc0_.setAlarmEpoch(next_alarm_clock_);
            rtc0_.enableAlarm(rtc0_.MATCH_SS); //check the alarm based on seconds, so we wake up every minute
        }

        void deep_sleep()
        {
            // bring CPU into deep sleep mode (until woken up by the RTC)
            rtc0_.standbyMode();
        }
    private:
        RTCZero rtc0_;
        unsigned long next_alarm_clock_;
    };

} 

#endif
