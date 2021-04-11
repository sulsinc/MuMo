#ifndef HEADER_stat_MovingAverage_hpp_ALREADY_INCLUDED
#define HEADER_stat_MovingAverage_hpp_ALREADY_INCLUDED

namespace stat { 

    //T should be big enough to hold the sum of all the samples
    template <typename T, unsigned int Size>
    class MovingAverage
    {
    public:
        //Update the oldest sample with a new one
        void append(T v)
        {
            const auto ix = (ix_ < 0) ? ix_+Size : ix_;
            samples_[ix] = v;

            ++ix_;
            if (ix_ == Size)
                ix_ = 0;
        }

        //Compute the average over all samples
        //Returns false if less than Size samples are present
        bool average(float &avg)
        {
            if (ix_ < 0)
                //Not enough measuremts yet
                return false;

            T sum = 0;
            for (unsigned int ix = 0; ix < Size; ++ix)
                sum += samples_[ix];

            avg = sum;
            avg *= 1.0f/Size;

            return true;
        }

    private:
        int ix_ = -Size;
        T samples_[Size];
    };

} 

#endif
