#ifndef HPP_MATRIX
#define HPP_MATRIX

/* Module provides a Matrix class for representing graphs */

/* TODO: 
 * - Add template (with constexpr argument for side_len)
 * - Replace pointer by std:array
 * - Add error handling and value checking
 * - Replace int* in operator[] by a span-class
 *   Or check C++20 std::span and C++23 operator[]
 * - Add const operator[]
 */


#include <iostream>



class Matrix {
    private:
        int* m_data;
        size_t  m_side_len;

    public:
        Matrix(size_t n);
        ~Matrix();


        size_t get_side_len() const;

        int* get_flat();


        int* operator[](int index);


};

std::istream& operator>>(std::istream& stream, Matrix& mat);
std::ostream& operator<<(std::ostream& stream, Matrix& mat);

#endif
