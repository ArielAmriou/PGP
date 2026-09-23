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
#include <functional>

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

            Matrix(const std::string &str)
            {
                std::string tmp(str);
                while (tmp.size() < Row * Col)
                    tmp.push_back('\0');
                for (std::size_t i = 0; i < Col; i++) {
                    for (std::size_t j = 0; j < Row; j++) {
                        (*this)(j, i) = tmp[i * Row + j];
                    }
                }
            }

            Matrix(const std::array<u_int8_t, Row * Col> &hex)
            {
                for (std::size_t i = 0; i < Col; i++) {
                    for (std::size_t j = 0; j < Row; j++) {
                        (*this)(j, i) = hex[i * Row + j];
                    }
                }
            }

            char operator()(std::size_t row, std::size_t col) const {return _matrix[row][col];}
            char &operator()(std::size_t row, std::size_t col) {return _matrix[row][col];}

            [[nodiscard]] std::size_t getColSize() const { return Col; }
            [[nodiscard]] std::size_t getRowSize() const { return Row; }

            [[nodiscard]] std::string getString() const
            {
                std::string word;
                for (std::size_t i = 0; i < Col; i++) {
                    for (std::size_t j = 0; j < Row; j++) {
                        word.push_back((*this)(j, i));
                    }
                }
                return word;
            }

            void setString(const std::string &str)
            {
                std::string tmp(str);
                while (tmp.size() < Row * Col)
                    tmp.push_back('\0');
                for (std::size_t i = 0; i < Col; i++) {
                    for (std::size_t j = 0; j < Row; j++) {
                        (*this)(j, i) = tmp[i * Row + j];
                    }
                }
            }

            [[nodiscard]] Matrix<Row, 1> getNColumn(std::size_t idx) const
            {
                Matrix<Row, 1> matrix;
                for (std::size_t i = 0; i < Row; i++)
                    matrix(i, 0) = (*this)(i, idx % Col);
                return matrix;
            }

            void modifyNColumn(const Matrix<Row, 1> &matrix, std::size_t idx)
            {
                for (std::size_t i = 0; i < Row; i++)
                    (*this)(i, idx % Col) = matrix(i, 0);
            }

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

            template <std::size_t maCol>
            Matrix<Row, maCol> customMul(const Matrix<Col, maCol> &matrix,
                std::function<char(char a, char b)> add, std::function<char(char a, char b)> mul)
            {
                Matrix<Row, maCol> cal;

                for (std::size_t i = 0; i < Row; i++)
                    customCalculateLine(cal, matrix, i, add, mul);
                return cal;
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

            template <std::size_t maCol>
            void customCalculateLine(Matrix<Row, maCol> &cal, const Matrix<Col, maCol> &matrix, std::size_t i,
                std::function<char(char a, char b)> add, std::function<char(char a, char b)> mul)
            {
                for (std::size_t j = 0; j < maCol; j++) {
                    char nb = 0;
                    for (std::size_t k = 0; k < Col; k++)
                        nb = add(nb, mul((*this)(i, k), matrix(k, j)));
                    cal(i, j) = nb;
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
