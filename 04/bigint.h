#pragma once

#include "vector.h"

#include <iostream>

std::string_view strip(std::string_view);

class TBigInt {
private:
    constexpr static int32_t digitShift = 4;

    bool neg;
    TVector data;

    void readBigInt(std::string_view);
public:
    constexpr static uint32_t base = 10'000;
    constexpr static uint32_t signBit = 1u << 31;

    TBigInt();
    TBigInt(uint32_t);
    explicit TBigInt(std::string);
    TBigInt(const TBigInt&);
    TBigInt(TBigInt&&);

    static bool removeLeadingZeros(TVector&);
    void clear();

    TBigInt& operator=(uint32_t);
    TBigInt& operator=(const TBigInt&);
    TBigInt& operator=(TBigInt&&);

    std::strong_ordering operator<=>(const TBigInt&) const;
    bool operator==(const TBigInt&) const;

    TBigInt operator-() const;
    TBigInt operator-(const TBigInt&) const;
    TBigInt operator+(const TBigInt&) const;
    TBigInt operator*(const TBigInt&) const;

    friend std::istream& operator>>(std::istream&, TBigInt&);
    friend std::ostream& operator<<(std::ostream&, const TBigInt&);
};
