#include <cgm/cg_vec3.hpp>
#include <cgm/cg_mat4.hpp>

namespace cg {
// 模板类实例化
template struct Vec2<int32_t>;
template struct Vec2<float_t>;
template struct Vec2<double_t>;

template struct Mat3<int32_t>;
template struct Mat3<float_t>;
template struct Mat3<double_t>;

template struct Vec3<int32_t>;
template struct Vec3<float_t>;
template struct Vec3<double_t>;

template struct Mat4<int32_t>;
template struct Mat4<float_t>;
template struct Mat4<double_t>;

}