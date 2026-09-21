/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** Texture
*/

#ifndef TEXTURE_HPP
    #define TEXTURE_HPP

    #include "PortablePixMap.hpp"
    #include "Vector.hpp"

namespace My_pgp {
    class Texture {
        public:
            Texture(std::string filename);
            Texture(PortablePixMap ppm);

            Maths::Color getColor(Maths::Vector2D uv, bool inf) const;

        private:
            PortablePixMap _ppm;
    };
}

#endif
