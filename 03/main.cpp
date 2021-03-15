#include "matrix.h"

#include <algorithm>
#include <sstream>
#include <cassert>

void TestMatrix() {
    {
        const std::pair<size_t, size_t> shape = std::make_pair(5, 3);
        const TMatrix matrix(shape.first, shape.second);
        assert(std::any_of(matrix.begin(), matrix.end(), [](int32_t elem) { return elem; }) == false);
        assert(matrix.getRows() == shape.first);
        assert(matrix.getColumns() == shape.second);

        TMatrix mutable_matrix(matrix);
        std::cout.setstate(std::ostream::failbit);              // empty output
        try {
            matrix[-1][0];
        } catch (const std::out_of_range &e) {
            std::cout << e.what() << std::endl;
            std::fill(mutable_matrix.begin(), mutable_matrix.end(), 777);
        }
        std::cout.clear();
        int32_t a = 5 + matrix[shape.first - 1][shape.second - 1] + mutable_matrix[1][1];
        assert(a == 782);                                       // 5 + 0 + 777
    }
    {
        const std::pair<size_t, size_t> shape = std::make_pair(4, 4);
        TMatrix matrix(shape.first, shape.second);
        std::fill(matrix.begin(), matrix.end(), 2);
        const TMatrix sum_matrix = matrix + matrix;
        matrix *= 2;
        assert(matrix == sum_matrix);
        matrix[2][3] = 5;
        assert(matrix != sum_matrix);

        std::ostringstream oss;
        oss << matrix;
        assert(oss.str() == "4 4 4 4\n4 4 4 4\n4 4 4 5\n4 4 4 4");
    }
    std::cerr << "TestMatrix is OK" << std::endl;
}

int main() {
    TestMatrix();
    return 0;
}
