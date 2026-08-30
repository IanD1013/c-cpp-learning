#include "BoundedCounter.h"

#include <vector>

using namespace std;

// This file is fixed. It includes your header, builds one BoundedCounter, records what value()
// answers before any amount is applied, then applies each amount in order and records the answer
// after each one.
struct Solution
{
        static int valueOf(const BoundedCounter& counter)
        {
            return counter.value();
        }

        static vector<int> driveCounter(int low, int high, int start, const vector<int>& amounts)
        {
            BoundedCounter counter(low, high, start);
            vector<int>    seen;
            seen.push_back(valueOf(counter));
            for (int amount : amounts)
            {
                counter.add(amount);
                seen.push_back(valueOf(counter));
            }
            return seen;
        }
};
