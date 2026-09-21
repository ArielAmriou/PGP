/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** ${descriptor}
*/

#ifndef TRIANGLESHAPE_HPP
    #define TRIANGLESHAPE_HPP
    #include <IObject.hpp>
	#include <PrimitiveTriangles.hpp>
    #include <vector>

namespace My_pgp {
    class TriangleShape {
    public:
        TriangleShape() = default;

        TriangleShape(std::initializer_list<PrimitiveTriangle> triangles)
            : _triangles(triangles) {}

        template<typename T>
        TriangleShape(const T &triangles)
            : _triangles(triangles.begin(), triangles.end()) {}

        std::optional<HitInfo> hits(Ray const &ray) const;

    private:
        std::vector<PrimitiveTriangle> _triangles;
    };
}

#endif //TRIANGLESHAPE_HPP