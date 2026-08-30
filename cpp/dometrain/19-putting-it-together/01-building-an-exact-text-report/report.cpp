#include "report.h"

using namespace std;

// Field is an implementation detail of formatReport, so it stays out of the
// header. Nothing outside this file needs to know it exists.
struct Field
{
    public:
        string name;
        string value;
};

string Solution::formatReport(const vector<string>& entries)
{
    vector<Field> fields;
    size_t        maxNameLength = 0;

    for (const string& entry : entries)
    {
        size_t split = entry.find('=');
        if (split == string::npos || split == 0)
        {
            continue;
        }

        string name  = entry.substr(0, split);
        string value = entry.substr(split + 1);

        fields.push_back({ name, value });
        if (name.length() > maxNameLength)
        {
            maxNameLength = name.length();
        }
    }

    if (fields.empty())
    {
        return "";
    }

    string report      = "";
    size_t targetWidth = maxNameLength + 2;

    for (size_t i = 0; i < fields.size(); ++i)
    {
        const Field& f = fields[i];

        report += f.name + ":";

        // Current length is f.name.length() + 1 (the colon)
        size_t currentLen = f.name.length() + 1;
        for (size_t s = currentLen; s < targetWidth; ++s)
        {
            report += " ";
        }

        if (f.value.empty())
        {
            report += "-";
        }
        else
        {
            report += f.value;
        }

        if (i < fields.size() - 1)
        {
            report += "\n";
        }
    }

    return report;
}
