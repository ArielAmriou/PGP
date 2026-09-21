/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** Matrix
*/

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <array>
#include <iostream>

namespace MyPgp {
    template <std::size_t Row, std::size_t Col>
    class Matrix {
        public:
            Matrix()
            {
                for (std::size_t i = 0; i < Row; i++)
                    for (std::size_t j = 0; j < Col; j++)
                        _matrix[i][j] = '\0';
            };

            char operator()(std::size_t row, std::size_t col) const {return _matrix[row][col];}
            char &operator()(std::size_t row, std::size_t col) {return _matrix[row][col];}

            template <std::size_t maCol>
            Matrix<Row, maCol> operator*(const Matrix<Col, maCol> &matrix)
            {
                Matrix<Row, maCol> mul;

                for (std::size_t i = 0; i < Row; i++)
                    calculateLine(mul, matrix, i);
                return mul;
            }

            template <std::size_t maCol>
            Matrix<Row, maCol> &operator*=(const Matrix<Col, maCol> &matrix)
            {
                *this = *this * matrix;
                return *this;
            }

        private:
            template <std::size_t maCol>
            void calculateLine(Matrix<Row, maCol> &mul, const Matrix<Col, maCol> &matrix, std::size_t i)
            {
                for (std::size_t j = 0; j < maCol; j++) {
                    char nb = 0;
                    for (std::size_t k = 0; k < Col; k++)
                        nb += (*this)(i, k) * matrix(k, j);
                    mul(i, j) = nb;
                }
            }

            std::array<std::array<char, Col>, Row> _matrix;
    };

    template <std::size_t Row, std::size_t Col>
    std::ostream &operator<<(std::ostream &s, const Matrix<Row, Col> &matrix)
    {
        s << "[";
            for (std::size_t i = 0; i < Row; i++) {
                if (i != 0)
                    s << ", ";
                s << "[";
                for (std::size_t j = 0; j < Col; j++) {
                    if (j != 0)
                        s << ", ";
                    s << matrix(i, j);
                }
                s << "]";
            }
        s << "]";
        return s;
    }
}

#endif /* !MATRIX_HPP_ */
