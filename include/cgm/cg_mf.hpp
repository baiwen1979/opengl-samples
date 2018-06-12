#ifndef _CG_MF_HPP_
#define _CG_MF_HPP_

#define _USE_MATH_DEFINES

#include <math.h>

namespace cg {

template <typename T>
inline T toRadian(const T& x) {
    return x * M_PI / 180.0f;
}

template <typename T>
inline T toDegree(const T& x) {
    return x * 180.0f / M_PI;
}

template <typename T>
inline T clamp(const T& v, const T& minValue, const T& maxValue) {
    return min(max(v, minValue), maxValue);
}

template <typename T>
T findMin(T * from, T * to) {
    T m = *from;
    for (T *p = from; p < to; p++) {
        if (m > *p) {
            m = *p;
        }
    }
    return m;
}

template <typename T>
T findMax(T * from, T * to) {
    T m = *from;
    for (T *p = from; p < to; p++) {
        if (m < *p) {
            m = *p;
        }
    }
    return m;
}

} //namespace cg

#endif //_CG_MF_HPP_