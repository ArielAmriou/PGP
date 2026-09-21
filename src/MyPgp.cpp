/*
** EPITECH PROJECT, 2026
** MyPgp
** File description:
** MyPgp
*/

#include "MyPgp.hpp"

#include "Xor.hpp"
#include <iostream>
#include <bits/stdc++.h>

namespace MyPgp {

    void MyPgp::run() noexcept
    {
        std::string hexKey = "What is dead may never die";
        std::reverse(hexKey.begin(), hexKey.end());
        auto cript = Xor::encrypt("You know nothing, Jon Snow", hexKey);
        std::reverse(cript.begin(), cript.end());
        std::string final;
        for (std::size_t i = 0; i < cript.length(); ++i) {
            std::stringstream ss;
            ss << std::hex << static_cast<int>(cript[i]);
            std::string strByte = ss.str();
            if (strByte.length() < 2)
                strByte = "0" + strByte;
            final.append(strByte);
        }
        std::cout << final << std::endl;

        std::reverse(cript.begin(), cript.end());
        auto msg = Xor::decrypt(cript, hexKey);
        std::cout << msg << std::endl;
    }

}