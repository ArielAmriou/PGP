/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** tests_error_handling
*/

#define __STDC_LIMIT_MACROS
#include <cstdint>

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>

#include "MyPgp.hpp"
#include "Exception.hpp"
#include "ArgsParser.hpp"
#include <deque>
#include <iostream>

static void redirect_all_std()
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

static std::string testMain(std::vector<std::string> args)
{
    try {
        MyPgp::MyPgp pgp(args);
        pgp.run();
    } catch (MyPgp::MyPgpException &e) {
        return e.what();
    } catch (const Parser::Help &) {
        return "Help Exception";
    } catch (const Parser::ArgsParserError &e) {
        return e.what();
    }
    return "No Error";
}

static std::string readHelp(void)
{
    std::string content;
    std::string str;
    std::string path = "public/help.txt";

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

Test(Parsing, Read_Help, .init = redirect_all_std)
{
    std::string help = readHelp() + "\n";
    MyPgp::MyPgp::displayHelp();
    fflush(stdout);
    cr_assert_stdout_eq_str(help.c_str());
}

Test(Parsing, Flag_Help)
{
    std::string value = testMain({"-h"});
    cr_assert_str_eq(value.c_str(), "Help Exception", "Test the Flag Help");
}

Test(Parsing, No_args)
{
    std::string value = testMain({});
    cr_assert_str_eq(value.c_str(), "Help Exception", "Test no Args");
}

Test(Parsing, Wrong_Methode)
{
    std::string value = testMain({"lol"});
    cr_assert_str_eq(value.c_str(), "Arguments error: lol is not a valid method", "Test no Args");
}

Test(Parsing, Wrong_Mode)
{
    std::string value = testMain({"xor"});
    cr_assert_str_eq(value.c_str(), "Arguments error: mode must be exactly one of -c, -d or -g", "Test no Args");
}

Test(Parsing, No_Key)
{
    std::string value = testMain({"xor", "-c"});
    cr_assert_str_eq(value.c_str(), "Arguments error: key is mandatory for -c and -d mode");
}

Test(Parsing, Wrong_methode_generate)
{
    std::string value = testMain({"xor", "-g", "d3", "e3"});
    cr_assert_str_eq(value.c_str(), "Arguments error: -g mode is only available for rsa and elgamal");
}

Test(Parsing, Not_Compatible)
{
    std::string value = testMain({"elgamal", "-g", "d3", "-b"});
    cr_assert_str_eq(value.c_str(), "Arguments error: -b mode is not compatible with -g");
}

Test(Parsing, Wrong_args_Generate_1)
{
    std::string value = testMain({"rsa", "-g", "d3", "e3", "d3"});
    cr_assert_str_eq(value.c_str(), "Arguments error: -g requires one or two args");
}

Test(Parsing, Wrong_args_Generate_2)
{
    std::string value = testMain({"rsa", "-g", "d3"});
    cr_assert_str_eq(value.c_str(), "Arguments error: -g requires two arguments P and Q for rsa");
}

Test(Parsing, Wrong_args_Generate_3)
{
    std::string value = testMain({"elgamal", "-g", "d3", "e3"});
    cr_assert_str_eq(value.c_str(), "Arguments error: -g requires one arguments P for ElGamal");
}

Test(Parsing, Wrong_args_Generate_4)
{
    std::string value = testMain({"rsa", "-g"});
    cr_assert_str_eq(value.c_str(), "Help Exception");
}

Test(Parsing, Wrong_option)
{
    std::string value = testMain({"rsa", "-c", "-b", " 0101-19bb"});
    cr_assert_str_eq(value.c_str(), "Arguments error: -b mode is not available for rsa");
}

Test(Parsing, Too_many_args)
{
    std::string value = testMain({"rsa", "-c", " 0101-19bb", "lol"});
    cr_assert_str_eq(value.c_str(), "Arguments error: too many arguments");
}