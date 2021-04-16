#pragma once

#include "row.h"

#include <utility>

class TMatrix {
private:
    std::pair<size_t, size_t> shape;
    int32_t *ptr;
public:
    explicit TMatrix(std::pair<size_t, size_t>);
    TMatrix(size_t, size_t);
    TMatrix(const TMatrix&);
    ~TMatrix();

    TMatrix& operator=(const TMatrix&) = delete;

    size_t getRows() const;
    size_t getColumns() const;

    const TRow operator[](size_t idx) const;
    TRow operator[](size_t idx);

    TMatrix& operator*=(int32_t multiplier);
    TMatrix operator+(const TMatrix&) const;
    bool operator==(const TMatrix&) const;
    bool operator!=(const TMatrix&) const;

    const int32_t* begin() const;
    const int32_t* end() const;

    int32_t* begin();
    int32_t* end();

    friend std::ostream& operator<<(std::ostream&, const TMatrix&);
};
