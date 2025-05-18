#ifndef HPP_MATRIX
#define HPP_MATRIX

/* Module provides a part of Matrix class for general purpose */

#include <cstddef>
#include <iostream>
#include <initializer_list>

#include <boost/json.hpp>
using namespace boost;


template<typename T>
class Matrix {
    private:
        T* m_data;
        size_t  m_row_count;
        size_t  m_col_count;

    public:
        Matrix(size_t n, size_t m) 
            : m_row_count(n), m_col_count(m), m_data(new int[n * m]) {}

        Matrix(size_t n) : Matrix(n, n) {}

        Matrix(size_t row_len, std::initializer_list<T> data) 
            : Matrix(data.size() / row_len, row_len) {
          
            size_t i = 0;
            for (auto v : data)
                m_data[i++] = v;
        }

        Matrix(const Matrix& mat) 
            : Matrix(mat.m_row_count, mat.m_col_count) {

            std::copy(mat.m_data, mat.m_data + m_row_count * m_col_count, m_data);
        };

        Matrix& operator=(const Matrix& mat) {
            Matrix<T> tmp(mat);
            std::swap(m_row_count, tmp.m_row_count);
            std::swap(m_col_count, tmp.m_col_count);
            std::swap(m_data, tmp.m_data);
            return *this;
        }

        ~Matrix() { delete[] m_data; }


        size_t get_row_count() const { return m_row_count; }
        size_t get_col_count() const { return m_col_count; }


        T* operator[](int index) { return m_data + index * m_col_count; }
        const T* operator[](int index) const { return m_data + index * m_col_count; }
};

typedef Matrix<int> matrix;


// DEV: boost json
template<typename T> 
void tag_invoke(json::value_from_tag, json::value& jv, Matrix<T> const& mat) {
    json::array rows;
            
    for (size_t i = 0; i < mat.get_row_count(); i++) {
        json::array row;
        for (size_t j = 0; j < mat.get_col_count(); j++) {
            row.emplace_back(mat[i][j]);
        }
            
        rows.push_back(std::move(row));
    }

    jv = rows;
}


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


template<typename T>
bool operator==(const Matrix<T>& lhs, const Matrix<T> rhs) {

    if (lhs.get_col_count() != rhs.get_col_count()
        or lhs.get_row_count() != rhs.get_row_count())
        return false;

    for (size_t i = 0; i < lhs.get_row_count(); i++) {
        for (size_t j = 0; j < lhs.get_col_count(); j++) {
            if (lhs[i][j] != rhs[i][j])
                return false;
        }
    }

    return true;
}

#endif
