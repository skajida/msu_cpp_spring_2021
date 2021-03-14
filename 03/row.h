#pragma once

#include <iostream>
#include <cstdint>

class TRow {
private:
    size_t size;
    int32_t *ptr;
public:
    TRow(int32_t*, size_t);
    TRow(const TRow&) = delete;

    TRow& operator=(const TRow&) = delete;

    int32_t operator[](size_t idx) const;
    int32_t& operator[](size_t idx);

    friend std::ostream& operator<<(std::ostream&, const TRow&);
};
