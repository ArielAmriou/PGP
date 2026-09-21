/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** ${descriptor}
*/

#ifndef LIGHTCONFIG_HPP
    #define LIGHTCONFIG_HPP
    #include <memory>
    #include <vector>

    #include "ILightSource.hpp"

namespace My_pgp {
    class LightConfig {
    public:
        LightConfig();

        LightConfig(double ambient, double diffuse, std::vector<std::unique_ptr<ILightSource>> lights);
        [[nodiscard]] double getAmbient() const;
        [[nodiscard]] double getDiffuse() const;

        void setAmbient(double);
        void setDiffuse(double);
        std::vector<std::unique_ptr<ILightSource>> &getLights();
        [[nodiscard]] const std::vector<std::unique_ptr<ILightSource>> &getLights() const;

    private:
        double _ambient;
        double _diffuse;
        std::vector<std::unique_ptr<ILightSource>> _lights;
    };
}

#endif //LIGHTCONFIG_HPP