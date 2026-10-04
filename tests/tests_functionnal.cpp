/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** tests_functionnal
*/

#define __STDC_LIMIT_MACROS
#include <cstdint>

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>

#include "MyPgp.hpp"
#include "Exception.hpp"
#include <deque>
#include <iostream>

static void redirect_all_std()
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

static std::string testMain(std::vector<std::string> args, const std::string &filename = "")
{
    if (!filename.empty()) {
        std::string path = "tests/files/" + filename;
        FILE *input = fopen(path.c_str(), "r");
        cr_assert_not_null(input, "The file %s doesn't exist.", path.c_str());
        
        dup2(fileno(input), STDIN_FILENO);
        fclose(input);
        clearerr(stdin);
        std::cin.clear();
    }
    try {
        MyPgp::MyPgp pgp(args);
        pgp.run();
    } catch (MyPgp::MyPgpException &e) {
        return e.what();
    }
    return "No Error";
}

static std::string readFile(std::string filename)
{
    std::string content;
    std::string str;
    std::string path = "tests/files/" + filename;

    bool first = false;

    std::ifstream file(path);
    while (std::getline(file, str)) {
        if (first)
            content += "\n" + str;
        else {
            content += str;
            first = true;
        }
    }
    file.close();
    return content;
}

Test(Block, Key_Wrong_Size, .init = redirect_all_std)
{
    std::string methode = "xor";
    std::string key = "576861742069732064656164206d6179206e657665722064696565";

    auto result = readFile("ciphered_" + methode) + "\n";
    std::string value = testMain({methode, "-c", "-b", key}, "message_" + methode);
    cr_assert_str_eq(value.c_str(), "MyPgp Error: The message and the symmetric key must be the same size");
}

Test(XOR, test_if_encryption_work, .init = redirect_all_std)
{
    std::string methode = "xor";
    std::string key = "576861742069732064656164206d6179206e6576657220646965";

    auto result = readFile("ciphered_" + methode) + "\n";
    std::string value = testMain({methode, "-c", "-b", key}, "message_" + methode);
    cr_assert_str_eq(value.c_str(), "No Error", "When executing crypt on %s it throw: %s.", methode.c_str(), value.c_str());
    fflush(stdout);
    cr_assert_stdout_eq_str(result.c_str(), "Wrong result for crypt %s.", methode.c_str());
}

Test(XOR, test_if_decryption_work, .init = redirect_all_std)
{
    std::string methode = "xor";
    std::string key = "576861742069732064656164206d6179206e6576657220646965";

    auto result = readFile("message_" + methode) + "\n";
    std::string value = testMain({methode, "-d", "-b", key}, "ciphered_" + methode);
    cr_assert_str_eq(value.c_str(), "No Error", "When executing decrypt on %s it throw: %s.", methode.c_str(), value.c_str());
    fflush(stdout);
    cr_assert_stdout_eq_str(result.c_str(), "Wrong result for decrypt %s.", methode.c_str());
}

Test(AES, test_if_encryption_work, .init = redirect_all_std)
{
    std::string methode = "aes";
    std::string key = "57696e74657220697320636f6d696e67";

    auto result = readFile("ciphered_" + methode) + "\n";
    std::string value = testMain({methode, "-c", "-b", key}, "message_" + methode);
    cr_assert_str_eq(value.c_str(), "No Error", "When executing crypt on %s it throw: %s.", methode.c_str(), value.c_str());
    fflush(stdout);
    cr_assert_stdout_eq_str(result.c_str(), "Wrong result for crypt %s.", methode.c_str());
}

Test(AES, test_if_decryption_work, .init = redirect_all_std)
{
    std::string methode = "aes";
    std::string key = "57696e74657220697320636f6d696e67";

    auto result = readFile("message_" + methode) + "\n";
    std::string value = testMain({methode, "-d", "-b", key}, "ciphered_" + methode);
    cr_assert_str_eq(value.c_str(), "No Error", "When executing decrypt on %s it throw: %s.", methode.c_str(), value.c_str());
    fflush(stdout);
    cr_assert_stdout_eq_str(result.c_str(), "Wrong result for decrypt %s.", methode.c_str());
}

Test(ElGamal, test_if_generation_work, .init = redirect_all_std)
{
    std::string methode = "elgamal";
    std::string key = "4b1da73924978f2e9c1f04170e46820d648edbee12ccf4d4462af89b080c86e1:34";

    auto result = readFile("generated_" + methode) + "\n";
    std::string value = testMain({methode, "-g", key});
    cr_assert_str_eq(value.c_str(), "No Error", "When executing generated on %s it throw: %s.", methode.c_str(), value.c_str());
    fflush(stdout);
    cr_assert_stdout_eq_str(result.c_str(), "Wrong result for generated %s.", methode.c_str());
}


Test(ElGamal, test_if_encryption_work, .init = redirect_all_std)
{
    std::string methode = "elgamal";
    std::string key = "4b1da73924978f2e9c1f04170e46820d648edbee12ccf4d4462af89b080c86e1-02-b10ad02e10b74b1091d7125eeaf9a285c71313d5a2449094e207ff419a473942:34";

    auto result = readFile("ciphered_" + methode) + "\n";
    std::string value = testMain({methode, "-c", key}, "message_" + methode);
    cr_assert_str_eq(value.c_str(), "No Error", "When executing crypt on %s it throw: %s.", methode.c_str(), value.c_str());
    fflush(stdout);
    cr_assert_stdout_eq_str(result.c_str(), "Wrong result for crypt %s.", methode.c_str());
}

Test(ElGamal, test_if_decryption_work, .init = redirect_all_std)
{
    std::string methode = "elgamal";
    std::string key = "4b1da73924978f2e9c1f04170e46820d648edbee12ccf4d4462af89b080c86e1-02-f75bb7d29ca6c8d20babaf060dde70259710f53556f8b16b9ce3509ef7a1edd3";

    auto result = readFile("message_" + methode) + "\n";
    std::string value = testMain({methode, "-d", key}, "ciphered_" + methode);
    cr_assert_str_eq(value.c_str(), "No Error", "When executing decrypt on %s it throw: %s.", methode.c_str(), value.c_str());
    fflush(stdout);
    cr_assert_stdout_eq_str(result.c_str(), "Wrong result for decrypt %s.", methode.c_str());
}
