#ifndef HEADER_stat_OutlierDetector_hpp_ALREADY_INCLUDED
#define HEADER_stat_OutlierDetector_hpp_ALREADY_INCLUDED

#include "stat_MovingAverage.hpp"

namespace stat { 

    template <typename T, unsigned int Size>
    class OutlierDetector
    {
    public:
        OutlierDetector(T tolerance): tolerance_(tolerance) {}

        //Returns true if v is detected as an outlier
        bool process(T v)
        {
            ma_.append(v);

            float avg;
            if (!ma_.average(avg))
                //Not enough measurements yet to make a measurement
                return true;

            auto abs_diff = v-avg;
            if (abs_diff < 0)
                abs_diff = -abs_diff;

            const bool is_outlier = abs_diff > tolerance_;

            return is_outlier;
        }

    private:
        const T tolerance_;
        MovingAverage<T, Size> ma_;
    };

} 

#endif
