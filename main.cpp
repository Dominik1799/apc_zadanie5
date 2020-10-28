#include <iostream>
#include "bignum.h"


int main() {
    BigNum n1{"-4"};
    BigNum n2{"-50"};
    n1 = n1;
    std::cout << n1*n2;
    return 0;
}

