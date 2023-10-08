#include "biginteger.h"

long long binpow(long long a, long long n) {
    if (n == 0) {
        return 1;
    }
    if (n % 2 == 1) {
        return a * binpow(a, n - 1);
    } else {
        long long b = binpow(a, n / 2);
        return b * b;
    }
}

void BigInteger::swap(BigInteger& x) {
    std::swap(sz_, x.sz_);
    std::swap(sgn_, x.sgn_);
    std::swap(digits_, x.digits_);
}

BigInteger::BigInteger() : sz_(1), digits_(1) {}

BigInteger::BigInteger(const std::string& str) {
    if (str[0] == '-') {
        sgn_ *= -1;
    }
    long long cnt = 0;
    long long now = 0;
    for (size_t i = str.size(); i > 0; --i) {
        if (str[i - 1] == '-') {
            break;
        }
        if (cnt == base_) {
            cnt = 0;
            digits_.push_back(now);
            ++sz_;
            now = 0;
        }
        ++cnt;
        now += (str[i - 1] - '0') * binpow(10, cnt - 1);
    }
    if (cnt != 0) {
        digits_.push_back(now);
        ++sz_;
    }
}

BigInteger::BigInteger(long long a) {
    if (a < 0) {
        sgn_ = -sgn_;
        a *= -1;
    }
    while (a > 0) {
        digits_.push_back(a % maxDig_);
        ++sz_;
        a /= maxDig_;
    }
    if (digits_.empty()) {
        ++sz_;
        digits_.push_back(0);
    }
}

BigInteger operator""_bi(unsigned long long x) {
    BigInteger temp(x);
    return temp;
}

BigInteger operator""_bi(const char* x, size_t /*unused*/) {
    BigInteger temp(x);
    return temp;
}

std::string BigInteger::toString() const {
    std::string res;
    if (sgn_ == -1) {
        res += '-';
    }
    for (int i = sz_ - 1; i >= 0; --i) {
        std::string tmp = std::to_string(digits_[i]);
        reverse(tmp.begin(), tmp.end());
        if (i != static_cast<int>(digits_.size()) - 1) {
            while (static_cast<int>(tmp.size()) < base_) {
                tmp += '0';
            }
        }
        reverse(tmp.begin(), tmp.end());
        res += tmp;
    }
    return res;
}

size_t BigInteger::size() const {
    return sz_;
}

int BigInteger::sign() const {
    return sgn_;
}

void BigInteger::clear() {
    digits_.clear();
    digits_.push_back(0);
    sz_ = 1;
    sgn_ = 1;
}

long long BigInteger::operator[](size_t index) const {
    return digits_[index];
}

BigInteger BigInteger::operator-() const {
    if (sz_ == 1 && digits_[0] == 0) {
        return *this;
    }
    BigInteger copy = *this;
    copy.sgn_ *= -1;
    return copy;
}

void BigInteger::changeSign(int val) {
    sgn_ = val;
}

void BigInteger::removeRedundant() {
    while (digits_.size() > 1 && digits_.back() == 0) {
        digits_.pop_back();
        --sz_;
    }
}

bool BigInteger::isZero() const {
    return digits_.size() == 1 && digits_[0] == 0;
}

void BigInteger::sum(const BigInteger& x) {
    long long rem = 0;
    size_t mx = std::max(x.size(), sz_);
    size_t i;
    for (i = 0; i < mx; ++i) {
        long long a = 0, b = 0, c = 0;
        if (i < sz_) {
            a = digits_[i];
        }
        if (i < x.size()) {
            b = x.digits_[i];
        }
        c = a + b + rem;
        if (i == sz_) {
            ++sz_;
            digits_.push_back(c % maxDig_);
        } else {
            digits_[i] = c % maxDig_;
        }
        rem = c / maxDig_;
    }
    while (rem > 0) {
        if (i == sz_) {
            ++sz_;
            digits_.push_back(rem % maxDig_);
        }
        rem = rem / maxDig_;
    }
    removeRedundant();
}

void BigInteger::diff(const BigInteger& x) {
    while (digits_.size() < x.digits_.size()) {
        digits_.push_back(0);
        ++sz_;
    }
    for (size_t i = 0; i < x.digits_.size(); ++i) {
        digits_[i] -= x.digits_[i];
    }
    int idx_pos = -1, idx_neg = -1;
    for (size_t i = 0; i < digits_.size(); ++i) {
        if (digits_[i] < 0) {
            idx_neg = i;
        }
        if (digits_[i] > 0) {
            idx_pos = i;
        }
    }
    if (idx_pos < idx_neg) {
        sgn_ *= -1;
        for (size_t i = 0; i < digits_.size(); ++i) {
            digits_[i] *= -1;
        }
    }
    bool help = false;
    for (size_t i = 0; i < digits_.size(); ++i) {
        if (help && digits_[i] <= 0) {
            digits_[i] += maxDig_ - 1;
        } else if (help) {
            --digits_[i];
            help = false;
        } else if (digits_[i] < 0) {
            digits_[i] += maxDig_;
            help = true;
        }
    }
    removeRedundant();
}

BigInteger& BigInteger::operator-=(const BigInteger& a) {
    if (sgn_ != a.sign()) {
        sgn_ *= -1;
        (*this) += a;
        sgn_ *= -1;
        return *this;
    }
    diff(a);
    if (sz_ == 1 && digits_[0] == 0) {
        sgn_ = 1;
    }
    return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger& a) {
    if (sgn_ != a.sign()) {
        sgn_ *= -1;
        *this -= a;
        sgn_ *= -1;
        return *this;
    }
    sum(a);
    if (sz_ == 1 && digits_[0] == 0) {
        sgn_ = 1;
    }
    return *this;
}

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
    BigInteger temp = a;
    temp += b;
    return temp;
}

BigInteger operator-(const BigInteger& a, const BigInteger& b) {
    BigInteger temp = a;
    temp -= b;
    return temp;
}

BigInteger& BigInteger::operator++() {
    *this += 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger copy = *this;
    ++(*this);
    return copy;
}

BigInteger& BigInteger::operator--() {
    *this -= 1;
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger copy = *this;
    --copy;
    return copy;
}

BigInteger& BigInteger::operator*=(const BigInteger& a) {
    bool neg = false;
    if (sgn_ != a.sign()) {
        neg = true;
    }
    BigInteger temp(0);
    for (size_t i = 0; i < a.size(); ++i) {
        long long rememb = 0;
        long long cur = a.digits_[i];
        for (size_t j = 0; j < sz_; ++j) {
            long long b = digits_[j];
            long long c = b * cur + rememb;
            while (temp.size() <= i + j) {
                temp.digits_.push_back(0);
                ++temp.sz_;
            }
            temp.digits_[i + j] += c % maxDig_;
            rememb = c / maxDig_;
        }
        size_t post_j = sz_;
        while (rememb > 0) {
            while (temp.size() <= i + post_j) {
                temp.digits_.push_back(0);
                ++temp.sz_;
            }
            temp.digits_[i + post_j] += rememb % maxDig_;
            ++post_j;
            rememb = rememb / maxDig_;
        }
        temp +=
            0_bi;  // to ensure that all digits in the digits are correct ( < maxDigit_ )
    }
    swap(temp);
    if (neg) {
        sgn_ = -1;
    }
    if (sz_ == 1 && digits_[0] == 0) {
        sgn_ = 1;
    }
    return *this;
}

void BigInteger::division(const BigInteger& a, int is_quotient) {
    bool neg = false;
    if (sgn_ != a.sign()) {
        neg = true;
    }
    bool fl = true;
    if (a.sign() == -1) {
        fl = false;
    }
    BigInteger res(0);
    BigInteger rememb(0);
    for (size_t i = sz_; i > 0; --i) {
        BigInteger cur = digits_[i - 1] + rememb * maxDig_;
        long long l = 0, r = maxDig_;
        while (l + 1 < r) {
            long long md = (l + r) >> 1LL;
            if (a * (md * a.sign()) <= cur) {
                l = md;
            } else {
                r = md;
            }
        }
        if (fl) {
            cur -= (a * l);
        } else {
            cur += (a * l);
        }
        if (sz_ - i >= res.size()) {
            res.digits_.push_back(0);
            ++res.sz_;
        }
        res.digits_[sz_ - i] = l;
        rememb = cur;
    }
    if (is_quotient == 1) {
        std::reverse(res.digits_.begin(), res.digits_.end());
        swap(res);
    } else {
        swap(rememb);
    }
    removeRedundant();
    if (neg) {
        sgn_ = -1;
    }
    if (sz_ == 1 && digits_[0] == 0) {
        sgn_ = 1;
    }
}

BigInteger& BigInteger::operator/=(const BigInteger& a) {
    if (a.size() == 1 && a.digits_[0] == 0) {
        throw std::runtime_error("Division by 0");
    }
    division(a, 1);
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& a) {
    if (a.size() == 1 && a.digits_[0] == 0) {
        throw std::runtime_error("Division by 0");
    }
    division(a, 0);
    return *this;
}

BigInteger operator*(const BigInteger& a, const BigInteger& b) {
    BigInteger temp = a;
    temp *= b;
    return temp;
}

BigInteger operator/(const BigInteger& a, const BigInteger& b) {
    BigInteger temp = a;
    temp /= b;
    return temp;
}

BigInteger operator%(const BigInteger& a, const BigInteger& b) {
    BigInteger temp = a;
    temp %= b;
    return temp;
}

bool operator==(const BigInteger& a, const BigInteger& b) {
    if (a.size() != b.size()) {
        return false;
    }
    if (a.sign() != b.sign()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const BigInteger& a, const BigInteger& b) {
    return !(a == b);
}

bool operator<(const BigInteger& a, const BigInteger& b) {
    if (a.sign() != b.sign()) {
        return a.sign() < b.sign();
    }
    bool is_opp = false;
    if (a.sign() == -1) {
        is_opp = true;
    }
    if (a.size() != b.size()) {
        return a.sign() * a.size() < b.sign() * b.size();
    }
    for (int i = a.size() - 1; i >= 0; --i) {
        if (a[i] < b[i]) {
            return (true ^ is_opp) != 0;
        }
        if (a[i] > b[i]) {
            return (false ^ is_opp) != 0;
        }
    }
    return false;
}

bool operator>(const BigInteger& a, const BigInteger& b) {
    return b < a;
}

bool operator<=(const BigInteger& a, const BigInteger& b) {
    return !(a > b);
}

bool operator>=(const BigInteger& a, const BigInteger& b) {
    return !(a < b);
}

BigInteger::operator bool() const {
    return !(digits_.size() == 1 && digits_[0] == 0);
}

std::istream& operator>>(std::istream& in, BigInteger& x) {
    x.clear();
    std::string str;
    char symb;
    in.get(symb);
    while ((std::isspace(symb) != 0) && in.good()) {
        in.get(symb);
    }
    while ((!std::isspace(symb, in.getloc())) && in.good()) {
        str += symb;
        in.get(symb);
    }
    x = str;
    return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& x) {
    std::string to_out = x.toString();
    out << to_out;
    return out;
}

void swap(BigInteger& x, BigInteger& y) {
    std::swap(y.sz_, x.sz_);
    std::swap(y.sgn_, x.sgn_);
    std::swap(y.digits_, x.digits_);
}

void Rational::swapRational(Rational& x) {
    std::swap(sgn_, x.sgn_);
    swap(a_, x.a_);
    swap(b_, x.b_);
    if (a_ == 0) {
        sgn_ = 1;
    }
}

BigInteger gcd(BigInteger x, BigInteger y) {
    while (y) {
        x %= y;
        swap(x, y);
    }
    return x;
}

Rational::Rational() : a_(0), b_(1) {}

Rational::Rational(const BigInteger& x) : a_(x), b_(1) {
    if (a_.sign() == -1) {
        sgn_ = -1;
        a_.changeSign(1);
    }
}

Rational::Rational(long long x) : a_(x), b_(1) {
    if (a_.sign() == -1) {
        sgn_ = -1;
        a_.changeSign(1);
    }
}

Rational Rational::operator-() const {
    if (a_.size() == 1 && a_[0] == 0) {
        return *this;
    }
    Rational copy = *this;
    copy.sgn_ *= -1;
    return copy;
}

std::string Rational::toString() const {
    std::string res;
    if (sgn_ == -1 && a_ != 0) {
        res = "-";
    }
    res += a_.toString();
    if (b_ != 1) {
        res += "/" + b_.toString();
    }
    return res;
}

std::string Rational::asDecimalToString(const BigInteger& beforeDot,
                                        const BigInteger& afterDot,
                                        size_t precision) {
    std::string res;
    res += beforeDot.toString() + ".";
    std::string resAfterDot = afterDot.toString();
    std::string missing(precision - resAfterDot.size(), '0');
    resAfterDot = missing + resAfterDot;
    res += resAfterDot;
    return res;
}

std::string Rational::asDecimal(size_t precision) const {
    BigInteger tmp1;
    std::string pw(precision, '0');
    pw = "1" + pw;
    BigInteger x(pw);
    tmp1 = (a_ / b_);
    tmp1.changeSign(sgn_);
    if (precision == 0) {
        if (a_ == 0) {
            tmp1.changeSign(1);
        }
        return tmp1.toString();
    }
    BigInteger tmp2;
    tmp2 = (a_ % b_) * (x) / b_;
    return asDecimalToString(tmp1, tmp2, precision);
}

Rational::operator double() const {
    std::string s = asDecimal(20);
    return std::stod(s);
}

int Rational::sign() const {
    return sgn_;
}
void Rational::cutTheFraction() {
    BigInteger gc = gcd(a_, b_);
    a_ /= gc;
    b_ /= gc;
}

void Rational::oppositeForWholeFractionIfNeeded(Rational& a, BigInteger& x) {
    if (x.sign() == -1) {
        x.changeSign(-x.sign());
        a.sgn_ *= -1;
    }
}

void Rational::oppositeForNumeratorOrDenomimatorIfNeeded(BigInteger& x) {
    if (x.sign() == -1) {
        x.changeSign(-x.sign());
    }
}

Rational& Rational::operator+=(const Rational& x) {
    //nw.a_ = sgn_ * a_ * x.b_ + x.sgn_ * b_ * x.a_;
    a_ = sgn_ * a_ * x.b_ + x.sgn_ * b_ * x.a_;
    oppositeForWholeFractionIfNeeded(*this, a_);
    b_ *= x.b_;
    //nw.b_ = b_ * x.b_;
    oppositeForWholeFractionIfNeeded(*this, b_);
    cutTheFraction();
    return *this;
}

Rational& Rational::operator-=(const Rational& x) {
    a_ = sgn_ * a_ * x.b_ - x.sgn_ * b_ * x.a_;
    oppositeForWholeFractionIfNeeded(*this, a_);
    b_ *= x.b_;
    oppositeForWholeFractionIfNeeded(*this, b_);
    cutTheFraction();
    return *this;
}

Rational& Rational::operator*=(const Rational& x) {
    sgn_ = sgn_ * x.sgn_;
    a_ = a_ * x.a_;
    if (a_.isZero()) {
        sgn_ = 1;
    }
    oppositeForNumeratorOrDenomimatorIfNeeded(a_);
    b_ = b_ * x.b_;
    oppositeForNumeratorOrDenomimatorIfNeeded(b_);
    cutTheFraction();
    return *this;
}

Rational& Rational::operator/=(const Rational& x) {
    sgn_ = sgn_ * x.sgn_;
    a_ = a_ * x.b_;
    if (a_.isZero()) {
        sgn_ = 1;
    }
    oppositeForNumeratorOrDenomimatorIfNeeded(a_);
    b_ = b_ * x.a_;
    oppositeForNumeratorOrDenomimatorIfNeeded(b_);
    cutTheFraction();
    return *this;
}

Rational operator+(const Rational& a, const Rational& b) {
    Rational temp = a;
    temp += b;
    return temp;
}

Rational operator-(const Rational& a, const Rational& b) {
    Rational temp = a;
    temp -= b;
    return temp;
}

Rational operator*(const Rational& a, const Rational& b) {
    Rational temp = a;
    temp *= b;
    return temp;
}

Rational operator/(const Rational& a, const Rational& b) {
    Rational temp = a;
    temp /= b;
    return temp;
}

const BigInteger& Rational::getNumerator() const {
    return a_;
}

const BigInteger& Rational::getDenominator() const {
    return b_;
}

bool operator==(const Rational& a, const Rational& b) {
    return (a.getNumerator() == b.getNumerator() &&
            a.getDenominator() == b.getDenominator() && a.sign() == b.sign());
}

bool operator<(const Rational& a, const Rational& b) {
    return (a.sign() * a.getNumerator() * b.getDenominator() <
            b.sign() * a.getDenominator() * b.getNumerator());
}

bool operator!=(const Rational& a, const Rational& b) {
    return !(a == b);
}

bool operator>(const Rational& a, const Rational& b) {
    return b < a;
}
bool operator<=(const Rational& a, const Rational& b) {
    return !(a > b);
}

bool operator>=(const Rational& a, const Rational& b) {
    return !(a < b);
}
