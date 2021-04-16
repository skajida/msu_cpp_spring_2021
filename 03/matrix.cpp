#include "matrix.h"

#include <algorithm>
#include <stdexcept>

TMatrix::TMatrix(std::pair<size_t, size_t> Shape)
    : shape(Shape)
    , ptr(new int32_t[shape.first * shape.second])
{
    if (!shape.first || !shape.second) {
        shape = std::make_pair(0, 0);
    }
}

TMatrix::TMatrix(size_t rows, size_t columns)
    : shape(rows, columns)
    , ptr(new int32_t[rows * columns]())
{
    if (!rows || !columns) {
        shape = std::make_pair(0, 0);
    }
}

TMatrix::TMatrix(const TMatrix &obj)
    : shape(obj.shape.first, obj.shape.second)
    , ptr(new int32_t[shape.first * shape.second])
{
    std::copy(obj.begin(), obj.end(), begin());
}

TMatrix::~TMatrix() {
    delete[] ptr;
}

size_t TMatrix::getRows() const {
    return shape.first;
}

size_t TMatrix::getColumns() const {
    return shape.second;
}

const TRow TMatrix::operator[](size_t idx) const {
    if (idx < shape.first) {
        return TRow(ptr + idx * shape.second, shape.second);
    }
    throw std::out_of_range("no such row");
}

TRow TMatrix::operator[](size_t idx) {
    if (idx < shape.first) {
        return TRow(ptr + idx * shape.second, shape.second);
    }
    throw std::out_of_range("no such row");
}

TMatrix& TMatrix::operator*=(int32_t multiplier) {
    std::for_each(begin(), end(), [multiplier](int32_t &elem) { elem *= multiplier; });
    return *this;
}

TMatrix TMatrix::operator+(const TMatrix &operand) const {
    if (shape != operand.shape) {
        throw std::invalid_argument("matrix operands have different shapes");
    }
    TMatrix ans(*this);
    auto inputIt = operand.begin();
    for (auto it = ans.begin(); it != ans.end(); ++it) {
        *it += *inputIt++;
    }
    return ans;
}

bool TMatrix::operator==(const TMatrix &operand) const {
    if (shape != operand.shape) {
        return false;
    }
    return std::equal(begin(), end(), operand.begin());
}

bool TMatrix::operator!=(const TMatrix &operand) const {
    return !(*this == operand);
}

const int32_t* TMatrix::begin() const {
    return ptr;
}

const int32_t* TMatrix::end() const {
    return ptr + shape.first * shape.second;
}

int32_t* TMatrix::begin() {
    return ptr;
}

int32_t* TMatrix::end() {
    return ptr + shape.first * shape.second;
}

std::ostream& operator<<(std::ostream &os, const TMatrix &obj) {
    if (obj.shape.first) {
        os << obj[0];
        for (size_t i = 1; i < obj.shape.first; ++i) {
            os << '\n' << obj[i];
        }
    }
    return os;
}
