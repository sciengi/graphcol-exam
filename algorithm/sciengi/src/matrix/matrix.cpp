#include <matrix.h>



Matrix::Matrix(size_t n) : m_side_len(n), m_data(new int[n * n]) {}


Matrix::~Matrix() { delete[] m_data; }

        
size_t Matrix::get_side_len() const { return m_side_len; }


int* Matrix::get_flat() { return m_data; }


int* Matrix::operator[](int index) { return m_data + index * m_side_len; }


std::istream& operator>>(std::istream& stream, Matrix& mat) {
    
    for (size_t i = 0; i < mat.get_side_len(); i++) {
        for (size_t j = 0; j < mat.get_side_len(); j++) {
            std::cin >> mat[i][j];
        }
    }

    return stream;
}


std::ostream& operator<<(std::ostream& stream, Matrix& mat) {
    
    for (size_t i = 0; i < mat.get_side_len(); i++) {
        for (size_t j = 0; j < mat.get_side_len(); j++) {
            std::cout << mat[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    return stream;
}
