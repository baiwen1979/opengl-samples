#ifndef _CG_QUAT_HPP_
#define _CG_QUAT_HPP_

#include "cg_vec3.hpp"

namespace cg {

// 模板类：四元数
template<typename T>
class Quaternion {
public:
    Quaternion(T x, T y, T z, T w);
    // 规范化
    void normalize();
    // 获取共轭四元数
    Quaternion getConjugate();
    // 转换为角度
    Vec3<T> toDegrees();

    // 成员变量
    T x, y, z, w;
};

} // namespace cg

#include "cg_quat.inl"

#endif