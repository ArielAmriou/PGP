/*
** EPITECH PROJECT, 2026
** MyPgp
** File description:
** Main
*/

#include "ArgsParser.hpp"
#include "Exception.hpp"
#include "MyPgp.hpp"
#include <iostream>

int main(int ac, char **av)
{
    try {
        MyPgp::MyPgp pgp(ac, av);
        pgp.run();
    } catch (const Parser::Help &) {
        MyPgp::MyPgp::displayHelp();
        return MyPgp::EPISUCCESS;
    } catch (const Parser::ArgsParserError &e) {
        std::cerr << e.what() << std::endl;
        return MyPgp::EPIERROR;
    } catch (const MyPgp::MyPgpException &e) {
        std::cerr << e.what() << std::endl;
        return MyPgp::EPIERROR;
    }
    return MyPgp::EPISUCCESS;
}
