#include "matrix.h"

#include <algorithm>
#include <sstream>
#include <cassert>
#include <cstring>
#include <cmath>

void TestComplex1() {
    // testing correct creation
    constexpr std::pair<size_t, size_t> shape = std::make_pair(5, 3);
    const TMatrix matrix(shape);
    assert(matrix.getRows() == shape.first);
    assert(matrix.getColumns() == shape.second);
    assert(!std::any_of(matrix.begin(), matrix.end(), [](int32_t elem) { return elem; }));

    TMatrix mutable_matrix(matrix);                             // copy ctor
    try {
        matrix[-1][0];                                          // out_of_range exception (row)
    } catch (const std::out_of_range &e) {
        assert(!strcmp(e.what(), "no such row"));
        std::fill(mutable_matrix.begin(), mutable_matrix.end(), 777);
    }

    // executable sum of mutable and immutable matrices' elements
    int32_t a = 5 + matrix[shape.first - 1][shape.second - 1] + mutable_matrix[1][1];
    assert(a == 782);                                           // 5 + 0 + 777 == 782
}

void TestComplex2() {
    constexpr std::pair<size_t, size_t> shape = std::make_pair(4, 4);
    TMatrix matrix(shape);
    std::fill(matrix.begin(), matrix.end(), 2);
    matrix[0][0] = 1;
    const TMatrix sum_matrix = matrix + matrix;
    matrix *= 2;                                                // 2 * a == a + a
    assert(matrix == sum_matrix);                               // expected behavior
    matrix[2][3] = 5;
    assert(matrix != sum_matrix);

    /*
      filled with 2, except [0][0] element initialized with 1
      multiplied by 2
      [2][3] element setted with 5
    */
    std::ostringstream os;
    os << matrix;
    assert(os.str() == "2 4 4 4\n4 4 4 4\n4 4 4 5\n4 4 4 4");   // expected behavior
}

void TestFromExample() {
    constexpr size_t rows = 5;
    constexpr size_t cols = 3;
    TMatrix m(rows, cols);
    assert(m.getRows() == 5);
    assert(m.getColumns() == 3);

    m[1][2] = 5;
    double x = m[4][1];
    assert(std::fabs(x) < 1e-10);

    m *= 3;
    TMatrix m1(rows, cols);
    assert(m != m1);
    TMatrix m2 = m1 + m;
    std::ostringstream os;
    os << m2;
    assert(os.str() == "0 0 0\n0 0 15\n0 0 0\n0 0 0\n0 0 0");
}

void TestUnitMatrix() {
    TMatrix matrix(std::make_pair(1, 1));
    assert(matrix.getRows() == 1);
    assert(matrix.getColumns() == 1);
    assert(!matrix[0][0]);
    matrix[0][0] = 5;
    assert(matrix[0][0] == 5);
    matrix *= 5;
    assert(matrix[0][0] == 25);
    TMatrix another_matrix(std::make_pair(1, 1));
    another_matrix[0][0] = 17;
    const TMatrix sum = matrix + another_matrix;
    assert(sum[0][0] == 42);
    try {
        sum[0][1];                                              // out_of_range exception (column)
    } catch (const std::out_of_range &e) {
        assert(!strcmp(e.what(), "no such column"));
    }
}

void TestDiffShapesSum() {
    constexpr std::pair<size_t, size_t> shape = std::make_pair(5, 7);
    TMatrix matrix_lhs(shape);
    const TMatrix matrix_rhs(shape.second, shape.first);
    try {
        matrix_lhs + matrix_rhs;
    } catch (const std::invalid_argument &e) {
        assert(!strcmp(e.what(), "matrix operands have different shapes"));
    }
}

void TestEmptyMatrix() {
    constexpr std::pair<size_t, size_t> shape = std::make_pair(13, 0);
    const TMatrix matrix(shape);
    assert(!matrix.getRows() && !matrix.getColumns());
}

void TestMatrix() {
    TestComplex1();
    TestComplex2();
    TestFromExample();
    TestUnitMatrix();
    TestDiffShapesSum();
    TestEmptyMatrix();
    std::cerr << "TestMatrix is OK" << std::endl;
}

int main() {
    TestMatrix();
    return 0;
}
