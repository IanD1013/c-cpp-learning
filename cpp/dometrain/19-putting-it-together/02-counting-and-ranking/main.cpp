#include "entry.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Solution
{
    public:
        static vector<string> rankByCount(const vector<string>& values, int topCount)
        {
            if (topCount <= 0 || values.empty())
            {
                return {};
            }

            map<string, Entry> tally;
            for (size_t i = 0; i < values.size(); ++i)
            {
                const string& word = values[i];
                if (tally.find(word) == tally.end())
                {
                    tally[word] = { word, 1, static_cast<int>(i) };
                }
                else
                {
                    tally[word].count++;
                }
            }

            vector<Entry> entries;
            for (auto const& [key, entry] : tally)
            {
                entries.push_back(entry);
            }

            sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
                if (a.count != b.count)
                {
                    return a.count > b.count; // Higher count first
                }
                return a.firstIndex < b.firstIndex; // Earlier index first
            });

            size_t         resultSize = min(static_cast<size_t>(topCount), entries.size());
            vector<string> ranked;
            for (size_t i = 0; i < resultSize; ++i)
            {
                ranked.push_back(entries[i].word);
            }

            return ranked;
        }
};
