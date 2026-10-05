/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** Unit tests for RSA
*/

#include <criterion/criterion.h>

#include "RSA.hpp"

using boost::multiprecision::cpp_int;
using boost::multiprecision::powm;

const cpp_int MERSENNE_61 = cpp_int("2305843009213693951");
const cpp_int PRIME_1E9_7 = cpp_int("1000000007");

Test(RSA_Lambda, known_values)
{
    cr_assert(MyPgp::RSA::lambda(3, 5) == 4, "lambda(3, 5) must be lcm(2, 4) = 4");
    cr_assert(MyPgp::RSA::lambda(3, 11) == 10, "lambda(3, 11) must be lcm(2, 10) = 10");
    cr_assert(MyPgp::RSA::lambda(5, 7) == 12, "lambda(5, 7) must be lcm(4, 6) = 12");
}

Test(RSA_Lambda, divides_totient)
{
    cpp_int p = 7;
    cpp_int q = 13;
    cpp_int totient = (p - 1) * (q - 1);

    cr_assert(totient % MyPgp::RSA::lambda(p, q) == 0, "lambda(n) must divide phi(n)");
}

Test(RSA_Lambda, is_smaller_or_equal_to_totient)
{
    cpp_int p = MERSENNE_61;
    cpp_int q = PRIME_1E9_7;
    cpp_int totient = (p - 1) * (q - 1);

    cr_assert(MyPgp::RSA::lambda(p, q) <= totient, "lambda(n) must be at most phi(n)");
}

Test(RSA_Lambda, carmichael_property_holds)
{
    cpp_int p = 3;
    cpp_int q = 11;
    cpp_int n = p * q;
    cpp_int lambda = MyPgp::RSA::lambda(p, q);

    for (cpp_int a = 2; a < n; a++) {
        if (boost::multiprecision::gcd(a, n) != 1)
            continue;
        cr_assert(powm(a, lambda, n) == 1,
            "a^lambda must be 1 mod n for a = %s", a.str().c_str());
    }
}

Test(RSA_Lambda, carmichael_property_holds_with_big_primes)
{
    cpp_int p = MERSENNE_61;
    cpp_int q = PRIME_1E9_7;
    cpp_int n = p * q;
    cpp_int lambda = MyPgp::RSA::lambda(p, q);

    for (unsigned int a : {2u, 3u, 5u, 7u, 65537u}) {
        cr_assert(powm(cpp_int(a), lambda, n) == 1,
            "a^lambda must be 1 mod n for a = %u", a);
    }
}
