#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

long long binpow(long long a, long long n);

class BigInteger {
  private:
    static const long long base_ = 9;
    int sgn_ = 1;
    size_t sz_ = 0;
    static constexpr long long maxDig_ = 1e9;
    std::vector<long long> digits_;

    void swap(BigInteger& x);

    void sum(const BigInteger& x);
    void diff(const BigInteger& x);

    void removeRedundant();

    void division(const BigInteger& a, int is_quotient);

  public:
    BigInteger(long long a);
    BigInteger(const std::string& str);
    BigInteger();

    BigInteger& operator+=(const BigInteger& a);
    BigInteger& operator-=(const BigInteger& a);
    BigInteger& operator*=(const BigInteger& a);
    BigInteger& operator/=(const BigInteger& a);
    BigInteger& operator%=(const BigInteger& a);

    bool isZero() const;
    void changeSign(int val);
    size_t size() const;
    int sign() const;

    void clear();

    long long operator[](size_t index) const;

    std::string toString() const;

    BigInteger operator-() const;

    BigInteger& operator++();
    BigInteger operator++(int);
    BigInteger& operator--();
    BigInteger operator--(int);

    explicit operator bool() const;

    friend void swap(BigInteger& x, BigInteger& y);
};

std::istream& operator>>(std::istream& in, BigInteger& x);
std::ostream& operator<<(std::ostream& out, const BigInteger& x);

BigInteger gcd(BigInteger x, BigInteger y);

BigInteger operator+(const BigInteger& a, const BigInteger& b);
BigInteger operator-(const BigInteger& a, const BigInteger& b);
BigInteger operator*(const BigInteger& a, const BigInteger& b);
BigInteger operator/(const BigInteger& a, const BigInteger& b);
BigInteger operator%(const BigInteger& a, const BigInteger& b);

bool operator==(const BigInteger& a, const BigInteger& b);
bool operator<(const BigInteger& a, const BigInteger& b);
bool operator!=(const BigInteger& a, const BigInteger& b);
bool operator>(const BigInteger& a, const BigInteger& b);
bool operator<=(const BigInteger& a, const BigInteger& b);
bool operator>=(const BigInteger& a, const BigInteger& b);

BigInteger operator""_bi(unsigned long long x);
BigInteger operator""_bi(const char* x, size_t /*unused*/);

class Rational {
  private:
    int sgn_ = 1;
    BigInteger a_ = 0, b_ = 1;

    void swapRational(Rational& x);
    void cutTheFraction();
    static void oppositeForWholeFractionIfNeeded(Rational& a, BigInteger& x);
    static void oppositeForNumeratorOrDenomimatorIfNeeded(BigInteger& x);

  public:
    Rational(const BigInteger& x);
    Rational(long long x);
    Rational();

    Rational operator-() const;

    const BigInteger& getNumerator() const;
    const BigInteger& getDenominator() const;

    std::string toString() const;
    static std::string asDecimalToString(const BigInteger& beforeDot,
                                         const BigInteger& afterDot,
                                         size_t precision = 0);

    int sign() const;

    Rational& operator+=(const Rational& x);
    Rational& operator-=(const Rational& x);
    Rational& operator*=(const Rational& x);
    Rational& operator/=(const Rational& x);

    std::string asDecimal(size_t precision = 0) const;

    explicit operator double() const;
};

Rational operator+(const Rational& a, const Rational& b);
Rational operator-(const Rational& a, const Rational& b);
Rational operator*(const Rational& a, const Rational& b);
Rational operator/(const Rational& a, const Rational& b);

bool operator==(const Rational& a, const Rational& b);
bool operator<(const Rational& a, const Rational& b);
bool operator!=(const Rational& a, const Rational& b);
bool operator>(const Rational& a, const Rational& b);
bool operator<=(const Rational& a, const Rational& b);
bool operator>=(const Rational& a, const Rational& b);
