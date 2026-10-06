/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** Unit tests for RSA
*/

#include <criterion/criterion.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "RSA.hpp"
#include "Exception.hpp"

using boost::multiprecision::cpp_int;

namespace {

    /* Subject example 1: keyGen("d3", "e3") */
    const std::string SMALL_P       = "d3";
    const std::string SMALL_Q       = "e3";
    const std::string SMALL_PUB     = "0101-19bb";
    const std::string SMALL_PRIV    = "9d5b-19bb";
    const std::string SMALL_MSG     = "WF";
    const std::string SMALL_CRYPT   = "8f84";

    /* Subject example 2: 512 bits modulus */
    const std::string BIG_P =
        "4b1da73924978f2e9c1f04170e46820d648edbee12ccf4d4462af89b080c86e1";
    const std::string BIG_Q =
        "bb3ca1e126f7c8751bd81bc8daa226494efb3d128f72ed9f6cacbe96e14166cb";
    const std::string BIG_PUB =
        "010001-c9f91a9ff3bd6d84005b9cc8448296330bd23480f8cf8b36fd4edd0a8cd925de139a0076b962f"
        "4d57f50d6f9e64e7c41587784488f923dd60136c763fd602fb3";
    const std::string BIG_PRIV =
        "81b08f4eb6dd8a4dd21728e5194dfc4e349829c9991c8b5e44b31e6ceee1e56a11d66ef23389be92ef7"
        "a4178470693f509c90b86d4a1e1831056ca0757f3e209-c9f91a9ff3bd6d84005b9cc8448296330bd234"
        "80f8cf8b36fd4edd0a8cd925de139a0076b962f4d57f50d6f9e64e7c41587784488f923dd60136c763fd6"
        "02fb3";
    const std::string BIG_MSG = "The night is dark and full of terrors";
    const std::string BIG_CRYPT =
        "445b349e7318ad6af16b0bbb718be88ba1c41751f95751cd58857f88fe31f970405c6ec3f16d79172543b"
        "f4e571b5596d212f3e79cd08ef14abd244e325b80";

    /* Hex strings are little endian (lowest byte first), as in the subject */
    cpp_int fromLittleEndianHex(const std::string &hex)
    {
        std::string bigEndian;

        for (std::size_t i = 0; i + 2 <= hex.size(); i += 2)
            bigEndian = hex.substr(i, 2) + bigEndian;
        return cpp_int("0x" + bigEndian);
    }

    struct KeyPair {
        std::string pub;
        std::string priv;
    };

    KeyPair runKeyGen(const std::string &p, const std::string &q)
    {
        std::ostringstream out;
        std::streambuf *old = std::cout.rdbuf(out.rdbuf());

        try {
            MyPgp::RSA::keyGen(p, q);
        } catch (...) {
            std::cout.rdbuf(old);
            throw;
        }
        std::cout.rdbuf(old);

        std::istringstream in(out.str());
        std::string line;
        KeyPair kp;
        int found = 0;
        while (std::getline(in, line)) {
            auto pos = line.find(": ");
            if (pos == std::string::npos)
                continue;
            std::string name = line.substr(0, pos);
            std::string value = line.substr(pos + 2);
            if (name == "public key") { kp.pub = value; found++; }
            else if (name == "private key") { kp.priv = value; found++; }
        }
        cr_assert_eq(found, 2, "keyGen must print public key and private key, got: %s", out.str().c_str());
        return kp;
    }

    std::pair<cpp_int, cpp_int> splitKey(const std::string &key)
    {
        auto pos = key.find('-');

        cr_assert(pos != std::string::npos, "Key must be \"exp-mod\", got %s", key.c_str());
        return {fromLittleEndianHex(key.substr(0, pos)), fromLittleEndianHex(key.substr(pos + 1))};
    }

    template <typename F>
    void assertKeyParsingError(F fn)
    {
        try {
            fn();
        } catch (MyPgp::KeyParsingError &) {
            return;
        } catch (...) {
            cr_assert_fail("Expected a KeyParsingError, got another exception type");
        }
        cr_assert_fail("Expected a KeyParsingError, but nothing was thrown");
    }
}

/* ------------------------------------------------------------------ */
/* lambda                                                              */
/* ------------------------------------------------------------------ */

Test(RSA_Lambda, is_lcm_of_p_minus_1_and_q_minus_1)
{
    cr_assert(MyPgp::RSA::lambda(cpp_int(3), cpp_int(5)) == cpp_int(4), "lcm(2, 4) must be 4");
    cr_assert(MyPgp::RSA::lambda(cpp_int(5), cpp_int(7)) == cpp_int(12), "lcm(4, 6) must be 12");
    cr_assert(MyPgp::RSA::lambda(cpp_int(7), cpp_int(13)) == cpp_int(12), "lcm(6, 12) must be 12");
}

Test(RSA_Lambda, subject_primes)
{
    cpp_int lambda = MyPgp::RSA::lambda(cpp_int(0xd3), cpp_int(0xe3));

    cr_assert(lambda == cpp_int(23730), "lcm(210, 226) must be 23730, got %s", lambda.str().c_str());
}

Test(RSA_Lambda, is_symmetric)
{
    cpp_int p = fromLittleEndianHex("d3");
    cpp_int q = fromLittleEndianHex("e3");

    cr_assert(MyPgp::RSA::lambda(p, q) == MyPgp::RSA::lambda(q, p), "lambda(p, q) must equal lambda(q, p)");
}

/* ------------------------------------------------------------------ */
/* keyGen                                                              */
/* ------------------------------------------------------------------ */

Test(RSA_KeyGen, subject_small_example)
{
    KeyPair kp = runKeyGen(SMALL_P, SMALL_Q);

    cr_assert_str_eq(kp.pub.c_str(), SMALL_PUB.c_str(), "Wrong public key for p = d3, q = e3");
    cr_assert_str_eq(kp.priv.c_str(), SMALL_PRIV.c_str(), "Wrong private key for p = d3, q = e3");
}

Test(RSA_KeyGen, subject_big_example)
{
    KeyPair kp = runKeyGen(BIG_P, BIG_Q);

    cr_assert_str_eq(kp.pub.c_str(), BIG_PUB.c_str(), "Wrong public key for the 512 bits example");
    cr_assert_str_eq(kp.priv.c_str(), BIG_PRIV.c_str(), "Wrong private key for the 512 bits example");
}

Test(RSA_KeyGen, is_deterministic)
{
    KeyPair a = runKeyGen(BIG_P, BIG_Q);
    KeyPair b = runKeyGen(BIG_P, BIG_Q);

    cr_assert_str_eq(a.pub.c_str(), b.pub.c_str(), "Same primes must give the same public key");
    cr_assert_str_eq(a.priv.c_str(), b.priv.c_str(), "Same primes must give the same private key");
}

Test(RSA_KeyGen, modulus_is_p_times_q)
{
    cpp_int p = fromLittleEndianHex(SMALL_P);
    cpp_int q = fromLittleEndianHex(SMALL_Q);
    KeyPair kp = runKeyGen(SMALL_P, SMALL_Q);

    cr_assert(splitKey(kp.pub).second == p * q, "Public modulus must be p * q");
    cr_assert(splitKey(kp.priv).second == p * q, "Private modulus must be p * q");
}

Test(RSA_KeyGen, public_exponent_is_a_fermat_number)
{
    /* Fermat numbers 2^(2^k) + 1 for k in [0, MAX_FERMAT_K] */
    KeyPair kp = runKeyGen(BIG_P, BIG_Q);
    cpp_int e = splitKey(kp.pub).first;
    bool isFermat = false;

    for (std::size_t k = 0; k <= MyPgp::MAX_FERMAT_K; k++) {
        cpp_int fermat = (cpp_int(1) << (1 << k)) + 1;
        if (e == fermat)
            isFermat = true;
    }
    cr_assert(isFermat, "Public exponent %s must be a Fermat number", e.str().c_str());
}

Test(RSA_KeyGen, private_exponent_inverts_public_exponent)
{
    cpp_int p = fromLittleEndianHex(BIG_P);
    cpp_int q = fromLittleEndianHex(BIG_Q);
    KeyPair kp = runKeyGen(BIG_P, BIG_Q);
    cpp_int e = splitKey(kp.pub).first;
    cpp_int d = splitKey(kp.priv).first;

    cr_assert((e * d) % MyPgp::RSA::lambda(p, q) == cpp_int(1), "e * d must be 1 modulo lambda(p, q)");
}

/* ------------------------------------------------------------------ */
/* encrypt                                                             */
/* ------------------------------------------------------------------ */

Test(RSA_Encrypt, subject_small_example)
{
    std::string crypt = MyPgp::RSA::encrypt(SMALL_MSG, SMALL_PUB);

    cr_assert_str_eq(crypt.c_str(), SMALL_CRYPT.c_str(), "Wrong ciphertext for \"WF\"");
}

Test(RSA_Encrypt, subject_big_example)
{
    std::string crypt = MyPgp::RSA::encrypt(BIG_MSG, BIG_PUB);

    cr_assert_str_eq(crypt.c_str(), BIG_CRYPT.c_str(), "Wrong ciphertext for the 512 bits example");
}

Test(RSA_Encrypt, is_deterministic)
{
    std::string first = MyPgp::RSA::encrypt(BIG_MSG, BIG_PUB);
    std::string second = MyPgp::RSA::encrypt(BIG_MSG, BIG_PUB);

    cr_assert_str_eq(first.c_str(), second.c_str(), "Textbook RSA without padding must give the same ciphertext");
}

Test(RSA_Encrypt, ciphertext_is_smaller_than_modulus)
{
    std::string crypt = MyPgp::RSA::encrypt(BIG_MSG, BIG_PUB);
    cpp_int n = splitKey(BIG_PUB).second;

    cr_assert(fromLittleEndianHex(crypt) < n, "Ciphertext must be smaller than the modulus");
}

Test(RSA_Encrypt, key_without_separator_is_rejected)
{
    assertKeyParsingError([] { MyPgp::RSA::encrypt(SMALL_MSG, "0101"); });
}

Test(RSA_Encrypt, key_with_too_many_parts_is_rejected)
{
    assertKeyParsingError([] { MyPgp::RSA::encrypt(SMALL_MSG, "0101-19bb-00"); });
}

/* ------------------------------------------------------------------ */
/* decrypt                                                             */
/* ------------------------------------------------------------------ */

Test(RSA_Decrypt, subject_small_example)
{
    std::string msg = MyPgp::RSA::decrypt(SMALL_CRYPT, SMALL_PRIV);

    cr_assert_str_eq(msg.c_str(), SMALL_MSG.c_str(), "Wrong plaintext for \"8f84\"");
}

Test(RSA_Decrypt, subject_big_example)
{
    std::string msg = MyPgp::RSA::decrypt(BIG_CRYPT, BIG_PRIV);

    cr_assert_str_eq(msg.c_str(), BIG_MSG.c_str(), "Wrong plaintext for the 512 bits example");
}

Test(RSA_Decrypt, roundtrip_small_key)
{
    KeyPair kp = runKeyGen(SMALL_P, SMALL_Q);

    for (const std::string &msg : {std::string("A"), std::string("Hi"), std::string("WF")}) {
        std::string crypt = MyPgp::RSA::encrypt(msg, kp.pub);
        std::string res = MyPgp::RSA::decrypt(crypt, kp.priv);

        cr_assert_str_eq(res.c_str(), msg.c_str(),
            "Decrypt(encrypt(\"%s\")) must give back the message", msg.c_str());
    }
}

Test(RSA_Decrypt, roundtrip_big_key)
{
    KeyPair kp = runKeyGen(BIG_P, BIG_Q);
    std::string crypt = MyPgp::RSA::encrypt(BIG_MSG, kp.pub);
    std::string res = MyPgp::RSA::decrypt(crypt, kp.priv);

    cr_assert_str_eq(res.c_str(), BIG_MSG.c_str(), "Decrypt(encrypt(msg)) must give back the message");
}

Test(RSA_Decrypt, wrong_private_key_does_not_give_the_message)
{
    KeyPair other = runKeyGen("d3", "e5");
    std::string res = MyPgp::RSA::decrypt(SMALL_CRYPT, other.priv);

    cr_assert_str_neq(res.c_str(), SMALL_MSG.c_str(), "A private key from another pair must not decrypt the message");
}

Test(RSA_Decrypt, key_without_separator_is_rejected)
{
    assertKeyParsingError([] { MyPgp::RSA::decrypt(SMALL_CRYPT, "9d5b"); });
}
