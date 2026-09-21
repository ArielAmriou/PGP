/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** ArgParser
*/

#ifndef ARGPARSER_HPP_
    #define ARGPARSER_HPP_

    #include <string>
    #include <vector>
    #include <exception>
    #include <optional>
    #include <iomanip>

namespace MyPgp {

    class ArgsParserError : public std::exception {
    public:
        ArgsParserError() {};

        [[nodiscard]] const char *what() const noexcept override
            { return "Arguments parsing error"; };
    };

    class ArgsParser {
    public:
        
        static bool isArg(
            std::reference_wrapper<std::vector<std::string>> args, std::string flag);

        template<typename T>
        static std::optional<T> getArg(
            std::reference_wrapper<std::vector<std::string>> args, std::string flag)
        {
            for (auto arg = args.get().begin(); arg != args.get().end(); ++arg) {
                if (*arg == flag && arg + 1 != args.get().end()) {
                    T tmp;
                    std::istringstream(*(arg + 1)) >> tmp;
                    args.get().erase(arg);
                    args.get().erase(arg);
                    return tmp;
                }
                if (*arg == flag)
                    throw ArgsParserError();
            }
            return {};
        }
    };
    
};

#endif /* !ARGPARSER_HPP_ */
