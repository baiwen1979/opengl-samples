// 模版函数实现文件
#ifndef _CG_GEM3D_INL
#define _CG_GEM3D_INL

#include "cg_gem3d.h"

namespace cg {

// 重载流输出运算符
template<typename T>
std::ostream& operator << (std::ostream &os, const Vec3<T> &v) {
    return os << '(' << v.x << ',' << v.y << ',' << v.z << ')';
}

}

#endif //_CG_GEM3D_INL