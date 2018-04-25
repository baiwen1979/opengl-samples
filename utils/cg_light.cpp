#include "cg_light.hpp"

namespace cg {

Light::Light(): color(Color3f(1.0f)), ambientIntensity(0.5f), diffuseIntensity(0.5f) {}

DirectionalLight::DirectionalLight(): Light(), direction(Vec3f(1.0f, -1.0f, -1.0f)) {}

PointLight::PointLight(): Light(), position(Vec3f(1.0f, 1.0f, 1.0f)) {
    attenuation.constant = 1.0f;
    attenuation.linear = 0.09f;
    attenuation.quadratic = 0.032f;
}

} // namespace