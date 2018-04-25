#ifndef _CG_MF_HPP_
#define _CG_MF_HPP_

#include <math.h>

template <typename T>
inline T toRadian(T x) {
    return x * M_PI / 180.0f;
}

template <typename T>
inline T toDegree(T x) {
    return x * 180.0f / M_PI;
}

template <typename T>
inline T clamp(T& v, T& min, T& max) {
    return min(max(v, min), max);
}

template <typename T>
T min(T * from, T * to) {
    T m = *from;
    for (T *p = from; p < to; p++) {
        if (m > *p) {
            m = *p;
        }
    }
    return m;
}

template <typename T>
T max(T * from, T * to) {
    T m = *from;
    for (T *p = from; p < to; p++) {
        if (m < *p) {
            m = *p;
        }
    }
    return m;
}
#endif //_CG_MF_HPP_