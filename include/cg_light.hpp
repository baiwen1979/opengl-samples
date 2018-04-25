#ifndef _CG_LIGHT_HPP_
#define _CG_LIGHT_HPP_

#include "cgm/cg_math.h"
#include "cg_types.h"

namespace cg {

// 光
struct Light {
    Light();
    Color3f color; // 颜色
    float ambientIntensity; // 环境光照强度
    float diffuseIntensity; // 漫反射光照强度
};

// 平行光
struct DirectionalLight: public Light {
    DirectionalLight();
    Vec3f direction; // 方向
};

// 点光源
struct PointLight: public Light {
    PointLight();

    Vec3f position; // 位置
    struct {
        float constant;
        float linear;
        float quadratic;
    } attenuation; // 衰减系数

};

} // namespace

#endif //_CG_LIGHT_HPP_