#pragma once

class BoundedCounter
{
    public:
        BoundedCounter(int low, int high, int start)
            : low_(low)
        {
            if (high < low)
            {
                high_ = low;
            }
            else
            {
                high_ = high;
            }

            value_ = start;
            if (value_ < low_)
            {
                value_ = low_;
            }
            else if (value_ > high_)
            {
                value_ = high_;
            }
        }

        void add(int amount)
        {
            value_ = value_ + amount;
            if (value_ < low_)
            {
                value_ = low_;
            }
            else if (value_ > high_)
            {
                value_ = high_;
            }
        }

        int value() const
        {
            return value_;
        }

    private:
        int low_;
        int high_;
        int value_;
};
