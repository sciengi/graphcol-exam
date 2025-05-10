#ifndef HPP_MATRIX
#define HPP_MATRIX

/* Module provides a part of Matrix class for general purpose */

#include <cstddef>
#include <iostream>



template<typename T>
class Matrix {
    private:
        T* m_data;
        const size_t  m_row_count;
        const size_t  m_col_count;

    public:
        Matrix(size_t n, size_t m) 
            : m_row_count(n), m_col_count(m), m_data(new int[n * m]) {}
        Matrix(size_t n) : Matrix(n, n) {}

        ~Matrix() { delete[] m_data; }


        size_t get_row_count() const { return m_row_count; }
        size_t get_col_count() const { return m_col_count; }


        T* operator[](int index) { return m_data + index * m_col_count; }
};


typedef Matrix<int> matrix;


template<typename T> 
std::ostream& operator<<(std::ostream& stream, Matrix<T>& mat) {
    
    for (size_t i = 0; i < mat.get_row_count(); i++) {
        for (size_t j = 0; j < mat.get_col_count(); j++) {
            std::cout << mat[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    return stream;
}


template<typename T> 
std::istream& operator>>(std::istream& stream, Matrix<T>& mat) {

    for (size_t i = 0; i < mat.get_row_count(); i++) {
        for (size_t j = 0; j < mat.get_col_count(); j++) {
            std::cin >> mat[i][j];
        }
    }

    return stream;
}

#endif
