/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** Rsa
*/

#ifndef RSA_HPP_
#define RSA_HPP_

#include "ACipher.hpp"
#include <boost/multiprecision/cpp_int.hpp>

namespace MyPgp {

    using boost::multiprecision::cpp_int;

    constexpr std::size_t MAX_FERMAT_K = 4;

    class RSA: public ACipher {
    private:
    
        static const cpp_int genFermat(std::size_t const k);

        static const cpp_int genE(cpp_int const &l);

        static const std::pair<cpp_int, cpp_int> parseKey(std::string const &k);
    
    public:
    
        static const cpp_int lambda(cpp_int const &p, cpp_int const &q);

        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false);
        
        static const std::string decrypt(const std::string &cript, const std::string &key, const bool block = false);

        static void keyGen(const std::string &p, const std::string &q);

    };

};

#endif /* !RSA_HPP_ */
