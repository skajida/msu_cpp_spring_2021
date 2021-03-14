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

std::ostream& operator<<(std::ostream &os, const TRow &obj) {
    if (obj.size) {
        os << obj.ptr[0];
        std::for_each(obj.ptr + 1, obj.ptr + obj.size, [&os](int32_t elem) { os << ' ' << elem; });
    }
    return os;
}
