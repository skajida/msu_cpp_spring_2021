#include "bigint.h"

#include <charconv>
#include <iomanip>

std::string_view strip(std::string_view src) {
    while (!src.empty() && isspace(src.front())) {
        src.remove_prefix(1);
    }
    while (!src.empty() && isspace(src.back())) {
        src.remove_suffix(1);
    }
    return src;
}

bool TBigInt::removeLeadingZeros(TVector &data) {
    while (!data.empty() && !data.back()) {
        data.pop_back();
    }
    return data.empty();
}

void TBigInt::readBigInt(std::string_view src) {
    neg = src.front() == '-';
    src.remove_prefix(src.front() == '-' || src.front() == '+');
    for (int32_t i = src.size(); i > 0; i -= digitShift) {
        const char *ptr = i < digitShift ? src.data() : &src[i - digitShift];
        uint32_t digit = 0;
        std::from_chars(ptr, ptr + std::min(i, digitShift), digit);
        data.push_back(digit);
    }
    neg = removeLeadingZeros(data) ? false : neg;
}

TBigInt::TBigInt() : neg(), data() {}

TBigInt::TBigInt(uint32_t val) : neg(val & signBit), data() {
    val = neg ? ~val + 1 : val;
    while (val) {
        data.push_back(val % base);
        val /= base;
    }
}

TBigInt::TBigInt(std::string_view str) : neg(), data() {
    std::string_view stripped_str = strip(str);
    readBigInt(stripped_str);
}

TBigInt::TBigInt(const TBigInt &obj) : neg(obj.neg), data(obj.data) {}

TBigInt::TBigInt(TBigInt &&obj) : neg(obj.neg), data(std::move(obj.data)) {
    obj.neg = false;
}

void TBigInt::clear() {
    neg = false;
    data.clear();
}

TBigInt& TBigInt::operator=(uint32_t val) {
    neg = val & signBit;
    data.clear();
    val = neg ? ~val + 1 : val;
    while (val) {
        data.push_back(val % base);
        val /= base;
    }
    return *this;
}

TBigInt& TBigInt::operator=(const TBigInt &obj) {
    neg = obj.neg;
    data = obj.data;
    return *this;
}

TBigInt& TBigInt::operator=(TBigInt &&obj) {
    neg = obj.neg;
    data = std::move(obj.data);
    obj.neg = false;
    return *this;
}

std::strong_ordering TBigInt::operator<=>(const TBigInt &obj) const {
    if (neg != obj.neg) {
        return obj.neg < neg ? std::strong_ordering::less : std::strong_ordering::greater;
    }
    const auto less = neg ? std::strong_ordering::greater : std::strong_ordering::less;
    const auto greater = neg ? std::strong_ordering::less : std::strong_ordering::greater;
    if (data.size() != obj.data.size()) {
            return data.size() < obj.data.size() ? less : greater;
    } else if (std::lexicographical_compare(data.rbegin(), data.rend(), obj.data.rbegin(), obj.data.rend())) {
        return less;
    }
    return greater;
}

bool TBigInt::operator==(const TBigInt &obj) const {
    if (neg != obj.neg || data.size() != obj.data.size()) {
        return false;
    }
    return std::equal(data.begin(), data.end(), obj.data.begin());
}

TBigInt TBigInt::operator-() const {
    TBigInt res = *this;
    if (res != 0) {
        res.neg = !neg;
    }
    return res;
}

TVector operator-(TVector::TVectorView lhs, TVector::TVectorView rhs) {
    TVector res = lhs;
    res.resize(lhs.size() + 1);
    for (size_t i = 0, carry = 0; i < rhs.size() || carry; ++i) {
        res[i] -= rhs[i] + carry;
        if ((carry = !!(res[i] & TBigInt::signBit))) {  // -Werror=parentheses
            res[i] += TBigInt::base;
        }
    }
    return res;
}

TBigInt TBigInt::operator-(const TBigInt &obj) const {
    if (neg != obj.neg) {
        return *this + (-obj);
    } else if (neg != (*this < obj)) {
        return -(obj - *this);
    }
    TBigInt res;
    res.data = this->data - obj.data;
    res.neg = removeLeadingZeros(res.data) ? false : neg;
    return res;
}

TVector operator+(TVector::TVectorView lhs, TVector::TVectorView rhs) {
    TVector res;
    res.reserve(std::max(lhs.size(), rhs.size()) + 1);
    res = lhs;
    for (size_t i = 0, carry = 0; i < std::max(res.size(), rhs.size()) || carry; ++i) {
        if (i == res.size()) {
            res.push_back(0);
        }
        res[i] += rhs[i] * (i < rhs.size()) + carry;
        if ((carry = res[i] >= TBigInt::base)) {        // -Werror=parentheses
            res[i] -= TBigInt::base;
        }
    }
    return res;
}

TBigInt TBigInt::operator+(const TBigInt &obj) const {
    if (neg != obj.neg) {
        return neg ? obj - (-*this) : *this - (-obj);
    }
    TBigInt res;
    res.neg = neg;
    res.data = this->data + obj.data;
    return res;
}

TVector operator*(TVector::TVectorView lhs, TVector::TVectorView rhs) {
    TVector res;
    res.resize(lhs.size() + rhs.size() + 1);
    if (res.capacity() < 8) {                           // trivial
        for (size_t i = 0; i < lhs.size(); ++i) {
            for (size_t j = 0, carry = 0; j < rhs.size() || carry; ++j) {
                uint32_t curr = res[i + j] + lhs[i] * rhs[j] + carry;
                carry = curr / TBigInt::base;
                res[i + j] = curr - carry * TBigInt::base;
            }
        }
    } else {                                            // karatsuba
        if (lhs.size() < rhs.size()) {
            std::swap(lhs, rhs);
        }
        auto [lhs_first, lhs_second] = lhs.split();
        auto [rhs_first, rhs_second] = rhs.split(lhs_first.size());
        TVector product_1 = lhs_first * rhs_first;
        TVector product_2 = lhs_second * rhs_second;
        TVector product_3 = (lhs_first + lhs_second) * (rhs_first + rhs_second);
        std::copy(product_1.begin(), product_1.end(), res.begin());
        std::copy(product_2.begin(), product_2.end(), res.begin() + lhs_first.size() + rhs_first.size());
        TVector med = product_3 - (product_1 + product_2);
        med = med + TVector::TVectorView(med.size(), res.begin() + lhs_first.size());
        std::copy(med.begin(), med.end(), res.begin() + lhs_first.size());
    }
    TBigInt::removeLeadingZeros(res);
    return res;
}

TBigInt TBigInt::operator*(const TBigInt &obj) const {
    if (data.empty() || obj.data.empty()) {
        return TBigInt();
    }
    TBigInt res;
    res.neg = neg != obj.neg;
    res.data = data * obj.data;
    return res;
}

std::istream& operator>>(std::istream &is, TBigInt &obj) {
    obj.clear();
    std::streamoff pos = (is >> std::ws).tellg();
    int32_t ch = is.peek();
    if (ch == EOF) {
        return is;
    }
    std::string str;
    if (ch == '-' || ch == '+') {
        is.ignore(1);
        str.push_back(ch);
    }
    if (!isdigit(is.peek())) {
        is.seekg(pos);
        is.setstate(std::istream::failbit);
        return is;
    }
    while (isdigit(ch = is.peek())) {
        str.push_back(ch);
        is.ignore(1);
    }
    obj.readBigInt(str);
    return is;
}

std::ostream& operator<<(std::ostream &os, const TBigInt &obj) {
    if (obj.data.empty()) {
        return os << 0;
    } else if (obj.neg) {
        os << '-';
    }
    os << obj.data.back() << std::setfill('0');
    for (size_t i = obj.data.size() - 1; i--; os << obj.data[i]) {
        os << std::setw(TBigInt::digitShift);
    }
    return os << std::setfill(' ');
}
