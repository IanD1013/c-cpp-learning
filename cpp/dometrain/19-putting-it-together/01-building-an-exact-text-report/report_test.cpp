#include "report.h"

#include <iostream>
#include <string>

using namespace std;

static int failures = 0;

static void check(const string& label, const string& actual, const string& expected)
{
    if (actual == expected)
    {
        cout << "PASS  " << label << "\n";
    }
    else
    {
        ++failures;
        cout << "FAIL  " << label << "\n";
        cout << "  expected:\n[" << expected << "]\n";
        cout << "  actual:\n[" << actual << "]\n";
    }
}

int main()
{
    check("1. pads the name column to the longest name plus two",
          Solution::formatReport({ "alpha=1", "beta=2" }),
          "alpha: 1\n"
          "beta:  2");

    check("2. leaves an entry with no equals sign out of the report and out of the width",
          Solution::formatReport({ "gamma=go", "broken", "x=y" }),
          "gamma: go\n"
          "x:     y");

    check("3. writes a dash where the value is empty",
          Solution::formatReport({ "k=", "kk=v" }),
          "k:  -\n"
          "kk: v");

    check("4. returns the empty string for an empty list", Solution::formatReport({}), "");

    check("5. keeps everything after the first equals sign in the value",
          Solution::formatReport({ "a=b=c" }),
          "a: b=c");

    check("6. returns the empty string when no entry is well formed",
          Solution::formatReport({ "nope", "=v" }),
          "");

    cout << "\n" << (failures == 0 ? "all tests passed" : "some tests failed") << "\n";
    return failures == 0 ? 0 : 1;
}
