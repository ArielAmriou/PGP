/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** ITextureGeneration
*/

#ifndef ITEXTUREGENERATION_HPP
    #define ITEXTUREGENERATION_HPP

    #include "PortablePixMap.hpp"

namespace My_pgp {
    class ITextureGeneration {
        public:
            virtual ~ITextureGeneration() = default;

            virtual PortablePixMap generate() = 0;
    };
}

#endif
