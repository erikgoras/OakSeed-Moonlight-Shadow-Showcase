#include <iostream>

struct Matrix {
    int rows;
    int cols;
    double* data;

    Matrix() : rows(0), cols(0), data(nullptr) {}

    Matrix(int r, int c) : rows(r), cols(c) {
        data = new double[rows * cols];
    }

    ~Matrix() {
        delete[] data;
    }

    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    double& operator()(int r, int c) {
        return data[r * cols + c];
    }

    const double& operator()(int r, int c) const {
        return data[r * cols + c];
    }

    void transpose() {
        Matrix transposed(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                transposed(j, i) = data[i * cols + j];
            }
        }
        std::swap(rows, cols);
        std::swap(data, transposed.data);
    }
};

Matrix multiply(const Matrix& a, const Matrix& b) {
    if (a.cols != b.rows) {
        return Matrix(); // Return empty matrix
    }

    Matrix result(a.rows, b.cols);
    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < b.cols; ++j) {
            result(i, j) = 0.0;
            for (int k = 0; k < a.cols; ++k) {
                result(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    return result;
}

void printMatrix(const Matrix& m, const std::string& label) {
    std::cout << label << ":\n";
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            std::cout << m(i, j) << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    // Create a 2x3 matrix
    Matrix a(2, 3);
    a(0, 0) = 1.0; a(0, 1) = 2.0; a(0, 2) = 3.0;
    a(1, 0) = 4.0; a(1, 1) = 5.0; a(1, 2) = 6.0;

    // Create a 3x2 matrix
    Matrix b(3, 2);
    b(0, 0) = 7.0; b(0, 1) = 8.0;
    b(1, 0) = 9.0; b(1, 1) = 10.0;
    b(2, 0) = 11.0; b(2, 1) = 12.0;

    // Multiply matrices
    Matrix result = multiply(a, b);
    if (result.rows == 0 || result.cols == 0) {
        std::cout << "Multiplication failed: Dimension mismatch.\n";
    } else {
        printMatrix(result, "Result of A * B");
    }

    // Transpose the first matrix
    Matrix transposedA = a;
    transposedA.transpose();
    printMatrix(transposedA, "Transpose of A");

    // Test incompatible multiplication
    Matrix incompatibleB(2, 3);
    Matrix invalidResult = multiply(a, incompatibleB);
    if (invalidResult.rows == 0 || invalidResult.cols == 0) {
        std::cout << "Multiplication failed: Dimension mismatch.\n";
    } else {
        printMatrix(invalidResult, "Result of A * incompatibleB");
    }

    return 0;
}
