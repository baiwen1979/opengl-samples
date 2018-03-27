#ifndef _CG_MATH_HPP_
#define _CG_MATH_HPP_
#include <math.h>

namespace cg {

template<typename T>
inline T toRadian(T x) {
    return x * M_PI / 180.0f;
}

template<typename T>
inline T toDegree(T x) {
    return x * 180.0f / M_PI;
}


}

#endif //_CG_MATH_HPP_