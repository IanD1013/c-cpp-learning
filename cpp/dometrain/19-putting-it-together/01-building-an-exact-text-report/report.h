#pragma once

#include <string>
#include <vector>

// Note: no `using namespace std;` here. A header is pasted into every file that
// includes it, so a using-directive in a header forces the whole std namespace
// on code that never asked for it. Qualify with std:: in headers instead.

struct Solution
{
    public:
        // Declaration only. The definition lives in report.cpp.
        static std::string formatReport(const std::vector<std::string>& entries);
};
