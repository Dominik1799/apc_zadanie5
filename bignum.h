#pragma once

// here you can include whatever you want :)
#include <string>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <iostream>

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
    friend BigNum operator*(BigNum lhs, const BigNum& rhs);



};

BigNum::BigNum() {
    numBuffer = std::to_string(0);
    sign = "+";
}

BigNum::BigNum(int64_t n) {
    numBuffer = std::to_string(n);
    if (n < 0)
        sign = "-";
    else
        sign = "+";
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
            if (wasFirstNumber || i+1 == str.size())
                numBuffer += str[i];
        }else if ((str[i] == '+' || str[i] == '-') && i == 0)
            continue;
        else
            exitProgram(1,"Wrong format: " + str);


    }
}

BigNum::BigNum(const BigNum &other) {
    numBuffer = other.numBuffer;
    sign = other.sign;
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
    for (size_t i = 0; i < len1; i++) {
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
    if (lhs.sign == "-" && rhs2.sign == "-"){
        rhs2.sign = "+";
        return lhs-rhs2;
    }
    if (lhs.sign == "-"){
        lhs.sign = "+";
        return rhs2 - lhs;
    }
    if (rhs2.sign == "-"){
        rhs2.sign = "+";
        return lhs-rhs2;
    }
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
    BigNum res{bigNumResult};
    return res;
}

BigNum operator-(BigNum lhs, const BigNum& rhs){
    bool negative = false;
    BigNum rhs2{rhs};
    if (lhs.sign == "-" && rhs2.sign == "-"){
        lhs.sign = "+";
        rhs2.sign = "+";
        return rhs2 - lhs;
    }
    if (rhs2.sign == "-"){
        rhs2.sign = "+";
        return lhs + rhs2;
    }
    if (lhs.sign == "-"){
        lhs.sign = "+";
        rhs2.sign = "+";
        BigNum res{lhs + rhs2};
        res.sign = "-";
        return res;
    }
    if (isSmaller(lhs,rhs2)){
        std::swap(lhs.numBuffer,rhs2.numBuffer);
        negative = true;
    }
    alignAndReverse(lhs,rhs2);
    int result,carry = 0;
    bool shouldCarry = false;
    std::string bigNumResult;
    for (size_t i = 0; i < lhs.numBuffer.size();i++){
        if ((lhs.numBuffer[i]-'0') < rhs2.numBuffer[i]-'0' + carry){
            lhs.numBuffer[i]+=10;
            shouldCarry = true;
        }
        result = (lhs.numBuffer[i]-'0' - (rhs2.numBuffer[i]-'0' + carry));
        bigNumResult+=static_cast<char>(result +'0');
        if (shouldCarry)
            carry = 1;
        else
            carry = 0;
        shouldCarry = false;
    }
    std::reverse(bigNumResult.begin(),bigNumResult.end());
    if (negative)
        bigNumResult = '-' + bigNumResult;
    BigNum res{bigNumResult};
    return res;

}
BigNum operator*(BigNum lhs, const BigNum& rhs){
    BigNum rhs2{rhs};
    if (lhs.numBuffer == "0" || rhs2.numBuffer == "0")
        return BigNum{"0"};
    int len1 = lhs.numBuffer.size();
    int len2 = rhs2.numBuffer.size();
    std::vector<int> result(len1 + len2, 0);
    int i_n1 = 0;
    int i_n2;
    for (int i=len1-1; i>=0; i--){
        int carry = 0;
        int n1 = lhs.numBuffer[i] - '0';
        i_n2 = 0;
        for (int j=len2-1; j>=0; j--){
            int n2 = rhs.numBuffer[j] - '0';
            int sum = n1*n2 + result[i_n1 + i_n2] + carry;
            carry = sum/10;
            result[i_n1 + i_n2] = sum % 10;
            i_n2++;
        }
        if (carry > 0)
            result[i_n1 + i_n2] += carry;

        i_n1++;
    }

    std::string res;
    for (int j : result){
        res += std::to_string(j);
    }
    std::string sign;
    std::reverse(res.begin(),res.end());
    if (lhs.sign == "-" && rhs2.sign == "-")
        sign = "+";
    else if (lhs.sign == "-")
        sign = "-";
    BigNum bigNumRes{res};
    bigNumRes.sign = sign;
    return bigNumRes;
}




#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif

bool operator==(const BigNum& lhs, const BigNum& rhs){
    return lhs.numBuffer == rhs.numBuffer && lhs.sign == rhs.sign;
}

bool operator!=(const BigNum& lhs, const BigNum& rhs){
    return lhs.numBuffer != rhs.numBuffer || lhs.sign != rhs.sign;
}
bool operator<(const BigNum& lhs, const BigNum& rhs){
    return isSmaller(lhs,rhs);
}
bool operator>(const BigNum& lhs, const BigNum& rhs){
    return !isSmaller(lhs,rhs) && lhs != rhs;
}
bool operator<=(const BigNum& lhs, const BigNum& rhs){
    return isSmaller(lhs,rhs) || lhs==rhs;
}
bool operator>=(const BigNum& lhs, const BigNum& rhs){
    return !isSmaller(lhs,rhs);
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
