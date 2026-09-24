/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** Exception
*/

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <string>

namespace MyPgp {
    class MyPgpException : public std::exception {
    public:
        MyPgpException(const std::string &str) :
            _str("MyPgp Error: " + str) {};

        [[nodiscard]] const char *what() const noexcept override
        {
            return _str.c_str();
        };

    private:
        std::string _str;
    };
}

#endif /* !EXCEPTION_HPP_ */
