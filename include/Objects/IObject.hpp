/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** ${descriptor}
*/

#ifndef IOBJECT_HPP
    #define IOBJECT_HPP
    #include <optional>
    #include "Material.hpp"
    #include "Info.hpp"

namespace My_pgp {
    class IObject {
    public:
        virtual ~IObject() = default;

        virtual std::optional<HitInfo> hits(const Ray &) = 0;
    };
}

#endif //IOBJECT_HPP