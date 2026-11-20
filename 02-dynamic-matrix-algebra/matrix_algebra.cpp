#include <iostream>

struct Matrix {
    int rows;
    int cols;
    double* data;

    // Safe allocation helper
    static double* safeAllocate(int size) {
        return new double[size];
    }

    // Safe free helper
    static void safeFree(double* ptr) {
        delete[] ptr;
    }

    // Constructor
    Matrix(int r, int c) : rows(r), cols(c) {
        data = safeAllocate(rows * cols);
    }

    // Destructor
    ~Matrix() {
        safeFree(data);
    }

    // Copy constructor
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = safeAllocate(rows * cols);
        for (int i = 0; i < rows * cols; ++i) {
            data[i] = other.data[i];
        }
    }

    // Assignment operator
    Matrix& operator=(const Matrix& other) {
        if (this == &other) return *this;

        safeFree(data);
        rows = other.rows;
        cols = other.cols;
        data = safeAllocate(rows * cols);
        for (int i = 0; i < rows * cols; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    // Transpose matrix
    Matrix transpose() const {
        Matrix transposed(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                transposed.data[j * rows + i] = data[i * cols + j];
            }
        }
        return transposed;
    }
};

// Function to multiply two matrices
Matrix multiply(const Matrix& a, const Matrix& b) {
    if (a.cols != b.rows) {
        return Matrix(0, 0); // Invalid product matrix
    }

    Matrix result(a.rows, b.cols);
    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < b.cols; ++j) {
            result.data[i * b.cols + j] = 0;
            for (int k = 0; k < a.cols; ++k) {
                result.data[i * b.cols + j] += a.data[i * a.cols + k] * b.data[k * b.cols + j];
            }
        }
    }
    return result;
}

// Function to print a matrix
void printMatrix(const Matrix& matrix) {
    if (matrix.rows == 0 || matrix.cols == 0) {
        std::cout << "(Invalid Matrix)\n";
    } else {
        for (int i = 0; i < matrix.rows; ++i) {
            for (int j = 0; j < matrix.cols; ++j) {
                std::cout << matrix.data[i * matrix.cols + j] << " ";
            }
            std::cout << "\n";
        }
    }
}

int main() {
    // Create a 2x3 matrix
    Matrix mat1(2, 3);
    mat1.data[0] = 1.0; mat1.data[1] = 2.0; mat1.data[2] = 3.0;
    mat1.data[3] = 4.0; mat1.data[4] = 5.0; mat1.data[5] = 6.0;

    // Create a 3x2 matrix
    Matrix mat2(3, 2);
    mat2.data[0] = 7.0; mat2.data[1] = 8.0;
    mat2.data[2] = 9.0; mat2.data[3] = 10.0;
    mat2.data[4] = 11.0; mat2.data[5] = 12.0;

    // Test multiplication
    std::cout << "Matrix 1:\n";
    printMatrix(mat1);

    std::cout << "Matrix 2:\n";
    printMatrix(mat2);

    Matrix result = multiply(mat1, mat2);
    std::cout << "Product of Matrix 1 and Matrix 2:\n";
    printMatrix(result);

    // Test transpose
    Matrix transposed = mat1.transpose();
    std::cout << "Transpose of Matrix 1:\n";
    printMatrix(transposed);

    // Test incompatible multiplication
    Matrix mat3(2, 2);
    mat3.data[0] = 1.0; mat3.data[1] = 2.0;
    mat3.data[2] = 3.0; mat3.data[3] = 4.0;

    Matrix invalidResult = multiply(mat1, mat3);
    std::cout << "Attempt to multiply incompatible matrices (Matrix 1 and Matrix 3):\n";
    printMatrix(invalidResult);

    return 0;
}
