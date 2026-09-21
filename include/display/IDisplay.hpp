/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** IDisplay
*/

#ifndef IDISPLAY_HPP
    #define IDISPLAY_HPP

    #include "Display.hpp"
    #include "PortablePixMap.hpp"
    #include "Color.hpp"

namespace My_pgp {

    class IDisplay {
        public:
            virtual ~IDisplay() = default;

            virtual Event getEvent() = 0;
            virtual void draw() = 0;
            virtual void setSceneSize(std::size_t width, std::size_t height) = 0;
            virtual void setPix(std::size_t width,
                std::size_t height, Maths::Color8bit color) = 0;
    };
};

#endif
