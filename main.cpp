#include <iostream>
#include "bignum.h"

int main() {
    BigNum n1{"99"};
    BigNum n2{"2"};
    std::cout << n1+n2;
    return 0;
}
