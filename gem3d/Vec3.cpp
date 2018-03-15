#include <cg_vec3.hpp>
#include <cg_mat4.hpp>

namespace cg {
// 模板类实例化
template struct Vec3<int32_t>;
template struct Vec3<float>;

template struct Mat4<int32_t>;
template struct Mat4<float>;

}