#include "store.h"

#include <vector>

using namespace std;

// Nothing below is yours to edit. releaseOrder builds the store, adds every id, asks for one
// removal, and lets the store go out of scope. looseRelease builds a single Resource in a scope of
// its own, with no store anywhere. Every entry in either record was written by a destructor you
// wrote.
struct Solution
{
    public:
        static vector<int> releaseOrder(const vector<int>& ids, int removeId)
        {
            vector<int> record;
            {
                ResourceStore store(&record);
                for (int id : ids)
                {
                    store.add(id);
                }
                store.remove(removeId);
            }
            return record;
        }

        static vector<int> looseRelease(int id)
        {
            vector<int> record;
            {
                Resource lone(id, &record);
            }
            return record;
        }
};
