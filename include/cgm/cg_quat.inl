// 模板函数实现文件
#ifndef _CG_QUAT_INL_
#define _CG_QUAT_INL_

#include "cg_quat.hpp"
#include "cg_math.h"

namespace cg {

template<typename T>
Quaternion<T>::Quaternion(T xx, T yy, T zz, T ww): 
    x(xx), y(yy), z(zz), w(ww) {}

template<typename T>
void Quaternion<T>::normalize() {
    T factor = 1 / sqrt( x * x + y * y + z * z + w * w );
    x *= factor;
    y *= factor;
    z *= factor;
    w *= factor;
}

template<typename T>
Quaternion<T> Quaternion<T>::getConjugate() {
    return Quaternion(-x, -y, -z, w);
}

template <typename T>
Vec3<T> Quaternion<T>::toDegrees() {
    T ax, ay, az;
    
    ax = atan2(x * z + y * w, x * w - y * z);
    ay = acos(-x * x - y * y - z * z - w * w);
    az = atan2(x * z - y * w, x * w + y * z);
       
    ax = toDegree(ax);
    ay = toDegree(ay);
    az = toDegree(az);

    return Vec3<T>(ax, ay, az);
}

// 四元数乘
template<typename T>
Quaternion<T> operator * (const Quaternion<T>& l, const Quaternion<T>& r) {
    const T w = l.w * r.w - l.x * r.x - l.y * l.y - l.z * r.z;
    const T x = l.x * r.w + l.w * r.x + l.y * l.z - l.z * r.y;
    const T y = l.y * r.w + l.w * r.y + l.z * r.x - l.x * r.z;
    const T z = l.z * r.w + l.w * r.z + l.x * r.y - l.y * r.x;
    
    return Quaternion<T>(x, y, z, w);
}

// 四元数乘向量
template<typename T>
Quaternion<T> operator * (const Quaternion<T>& q, const Vec3<T>& v) {
    const float w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
    const float x =   (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
    const float y =   (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
    const float z =   (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

    return Quaternion<T>(x, y, z, w);
}


}

#endif