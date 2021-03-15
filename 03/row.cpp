#include "row.h"

#include <algorithm>
#include <stdexcept>

TRow::TRow(int32_t *p, size_t s) : size(s), ptr(p) {}

int32_t TRow::operator[](size_t idx) const {
    if (idx < size) {
        return ptr[idx];
    }
    throw std::out_of_range("no such column");
}

int32_t& TRow::operator[](size_t idx) {
    if (idx < size) {
        return ptr[idx];
    }
    throw std::out_of_range("no such column");
}

const int32_t* TRow::begin() const {
    return ptr;
}

const int32_t* TRow::end() const {
    return ptr + size;
}

int32_t* TRow::begin() {
    return ptr;
}

int32_t* TRow::end() {
    return ptr + size;
}

std::ostream& operator<<(std::ostream &os, const TRow &obj) {
    if (obj.size) {
        os << obj.ptr[0];
        std::for_each(obj.begin() + 1, obj.end(), [&os](int32_t elem) { os << ' ' << elem; });
    }
    return os;
}
