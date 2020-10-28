#include <iostream>
#include "bignum.h"


int main() {
    BigNum n1{"4"};
    BigNum n2{"5"};
    bool a = n1 != n2;
    std::cout << a;
    return 0;
}

