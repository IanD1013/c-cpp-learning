#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    int x = 1;
    double y = 2.0;
    int z = x + static_cast<int>(y);
    cout << z << endl;

    return 0;
}