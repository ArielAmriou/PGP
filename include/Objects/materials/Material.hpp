/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** Material
*/

#ifndef MATERIAL_HPP
    #define MATERIAL_HPP

    #include <optional>
    #include "Color.hpp"
    #include "Vector.hpp"
    #include "Ray.hpp"

namespace My_pgp {

    struct HitInfo;

    class Material {
        public:
            class Builder {
                public:
                    Builder() = default;

                    Builder &color(Maths::Color color);
                    Builder &metallic(double metallic);
                    Builder &specular(double specular);
                    Builder &roughness(double roughness);
                    Builder &opacity(double opacity);
                    Builder &refraction(double refraction);
                    Builder &texture(bool texture);

                    Material build() const {return Material(*this);}

                    [[nodiscard]] Maths::Color getColor() const {return _color;}
                    [[nodiscard]] double getMetallic() const {return _metallic;}
                    [[nodiscard]] double getSpecular() const {return _specular;}
                    [[nodiscard]] double getRoughness() const {return _roughness;}
                    [[nodiscard]] double getOpacity() const {return _opacity;}
                    [[nodiscard]] double getRefraction() const {return _refraction;}

                private:
                    Maths::Color _color = Maths::Color::WHITE;
                    double _metallic;
                    double _specular;
                    double _roughness;
                    double _opacity;
                    double _refraction;
            };

            explicit Material(Builder const &builder);


            Ray getReflectRay(const Ray &ray, const HitInfo &hit) const;

            std::optional<Ray> getRefractRay(const Ray &ray, const HitInfo &hit) const;
            double getFresnel(const Ray &ray, const HitInfo &hit) const;

            Maths::Color getColor(const HitInfo &hit) const;
            double getSpecular() const {return _specular;}
            double getRoughness() const {return _roughness;}
            double getOpacity() const {return _opacity;}

            Maths::Color getDiffuse() const
                {return Maths::Color(_color * (1 - _metallic));}
            double getShininess() const
                {return (1 - _roughness) * 100;}
    
        private:            
            Maths::Color _color;
            double _metallic;
            double _specular;
            double _roughness;
            double _opacity;
            double _refraction;
    };

}

#endif
