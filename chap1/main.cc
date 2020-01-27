#include "prog1.h"
#include <iostream>

using namespace std;

int main() {
    A_stm stm = prog();
    int max_args = maxargs(stm);
    cout << "maxargs: " << max_args << endl;
    interp(stm);
}