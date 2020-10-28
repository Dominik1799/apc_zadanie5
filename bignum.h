#pragma once

// here you can include whatever you want :)
#include <string>
#include <vector>
#include <stdint.h>
#include <algorithm>

#define SUPPORT_DIVISION 0 // define as 1 when you have implemented the division
#define SUPPORT_IFSTREAM 0 // define as 1 when you have implemented the input >>

// if you do not plan to implement bonus, just delete those lines
class BigNum final
{
public:
    // constructors
    BigNum();
    BigNum(int64_t n);

    explicit BigNum(const std::string& str);

    // copy
    BigNum(const BigNum& other);
    BigNum& operator=(const BigNum& rhs);

    // unary operators
    const BigNum& operator+() const;
    BigNum operator-() const;

    // binary arithmetics operators
    BigNum& operator+=(const BigNum& rhs);
    BigNum& operator-=(const BigNum& rhs);
    BigNum& operator*=(const BigNum& rhs);

#if SUPPORT_DIVISION == 1
    BigNum& operator/=(const BigNum& rhs); // bonus
    BigNum& operator%=(const BigNum& rhs); // bonus
#endif

private:
    // here you can add private data and members, but do not add stuff to
    // public interface, also you can declare friends here if you want
    std::string numBuffer;
    std::string sign;
    void exitProgram(int code,const std::string& message);
    friend bool operator==(const BigNum& lhs, const BigNum& rhs);
    friend bool operator!=(const BigNum& lhs, const BigNum& rhs);
    friend bool operator<(const BigNum& lhs, const BigNum& rhs);
    friend bool operator>(const BigNum& lhs, const BigNum& rhs);
    friend bool operator<=(const BigNum& lhs, const BigNum& rhs);
    friend bool operator>=(const BigNum& lhs, const BigNum& rhs);
    friend std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs);
    friend bool isSmaller(const BigNum& num1, const BigNum& num2);
    friend BigNum operator+(BigNum lhs, const BigNum& rhs);
    friend void alignAndReverse(BigNum& lhs,  BigNum& rhs);
    friend BigNum operator-(BigNum lhs, const BigNum& rhs);

};

BigNum::BigNum() {
    numBuffer = std::to_string(0);
}

BigNum::BigNum(int64_t n) {
    numBuffer = std::to_string(n);
}

BigNum::BigNum(const std::string &str) {
    bool wasFirstNumber = false;
    sign = str[0] =='+' || (str[0] >= '0' && str[0] <= '9') ? "+" : "";
    sign = str[0] =='-' ? "-" : sign;
    for (size_t i = 0; i < str.size(); i++){
        if (str[i] >= '1' && str[i] <= '9'){
            wasFirstNumber = true;
            numBuffer += str[i];
        } else if (str[i] == '0'){
            if (wasFirstNumber)
                numBuffer += str[i];
        }else if ((str[i] == '+' || str[i] == '-') && i == 0)
            continue;
        else
            exitProgram(1,"Wrong format: " + str);


    }
}

BigNum::BigNum(const BigNum &other) {
    numBuffer = other.numBuffer;
}

void BigNum::exitProgram(int code,const std::string& message) {
    std::cerr << message << "\n";
    exit(code);
}
//is num1 smaller than num2?
bool isSmaller(const BigNum& num1, const BigNum& num2){
    if (num1.sign != num2.sign){
        if (num1.sign == "+")
            return false;
        return true;
    }
    size_t len1 = num1.numBuffer.size();
    size_t len2 = num2.numBuffer.size();
    if (num1.sign == "+"){
        if (len1 > len2)
            return false;
        else if (len1 < len2)
            return true;
    } else {
        if (len1 > len2)
            return true;
        else if (len1 < len2)
            return false;
    }
    for (int i = 0; i < len1; i++) {
        if (num1.numBuffer[i] < num2.numBuffer[i])
            return true;
        else if (num1.numBuffer[i] > num2.numBuffer[i])
            return false;
    }
    return false;
}

void alignAndReverse(BigNum& lhs, BigNum& rhs){
    if (lhs.numBuffer.size() > rhs.numBuffer.size()){
        const size_t diff = lhs.numBuffer.size() - rhs.numBuffer.size();
        std::string zeroes(diff,'0');
        rhs.numBuffer = zeroes + rhs.numBuffer;
    }
    if (lhs.numBuffer.size() < rhs.numBuffer.size()){
        const size_t diff = rhs.numBuffer.size() - lhs.numBuffer.size();
        std::string zeroes(diff,'0');
        lhs.numBuffer = zeroes + lhs.numBuffer;
    }
    std::reverse(lhs.numBuffer.begin(),lhs.numBuffer.end());
    std::reverse(rhs.numBuffer.begin(),rhs.numBuffer.end());
}


BigNum operator+(BigNum lhs, const BigNum& rhs){
    BigNum rhs2{rhs};
    alignAndReverse(lhs,rhs2);
    int result,carry = 0;
    std::string bigNumResult;
    for (size_t i = 0; i < lhs.numBuffer.size();i++){
        result = (lhs.numBuffer[i]-'0')+ carry + (rhs2.numBuffer[i]-'0');
        carry = result / 10;
        bigNumResult += static_cast<char>((result % 10)+'0');
    }
    if (carry != 0){
        bigNumResult+=static_cast<char>(carry +'0');
    }
    std::reverse(bigNumResult.begin(),bigNumResult.end());
    rhs2.numBuffer = bigNumResult;
    return rhs2;
}

BigNum operator-(BigNum lhs, const BigNum& rhs){
    BigNum rhs2{rhs};
    alignAndReverse(lhs,rhs2);
    int result,carry = 0;
    std::string bigNumResult;
    for (size_t i = 0; i < lhs.numBuffer.size();i++){
        result = ((lhs.numBuffer[i]-'0') + carry) - (rhs2.numBuffer[i]-'0');
        carry = result / 10;
        bigNumResult += static_cast<char>((result % 10)+'0');
    }

};
BigNum operator*(BigNum lhs, const BigNum& rhs);

#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif

bool operator==(const BigNum& lhs, const BigNum& rhs){
    return lhs.numBuffer == rhs.numBuffer;
}

bool operator!=(const BigNum& lhs, const BigNum& rhs){
    return lhs.numBuffer != rhs.numBuffer;
}
bool operator<(const BigNum& lhs, const BigNum& rhs){
    return false;
}
bool operator>(const BigNum& lhs, const BigNum& rhs){
    return false;
}
bool operator<=(const BigNum& lhs, const BigNum& rhs){
    return false;
}
bool operator>=(const BigNum& lhs, const BigNum& rhs){
    return false;
}


std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs){
    if (rhs.sign == "-")
        lhs << "-";
    lhs << rhs.numBuffer;
    return lhs;
}



#if SUPPORT_IFSTREAM == 1
std::istream& operator>>(std::istream& lhs, BigNum& rhs); // bonus
#endif
