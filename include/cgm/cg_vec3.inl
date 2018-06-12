// 模版函数实现文件
#ifndef _CG_VEC3_INL
#define _CG_VEC3_INL

#include <cmath>
#include <cassert>
#include "cg_vec3.hpp"
#include "cg_quat.hpp"

using namespace std;

namespace cg {

template <typename T>
Vec3<T>::Vec3(): x(0), y(0), z(0) {}

template <typename T>
Vec3<T>::Vec3(T xx): x(xx), y(xx), z(xx) {}

template <typename T>
Vec3<T>::Vec3(T xx, T yy, T zz): x(xx), y(yy), z(zz) {}

template <typename T>
Vec3<T>::Vec3(const Vec3<T>& v): x(v.x), y(v.y), z(v.z) {}

template <typename T>
Vec3<T>& Vec3<T>::operator = (const Vec3<T>& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

template <typename T>
bool Vec3<T>::operator == (const Vec3<T>& v) const {
    return (x == v.x && y == v.y && z == v.z);
}

template <typename T>
bool Vec3<T>::operator != (const Vec3<T>& v) const {
    return !(*this == v);
}

// + 和 -
template <typename T>
Vec3<T> Vec3<T>::operator + (const Vec3<T>& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

template <typename T>
Vec3<T>& Vec3<T>::operator += (const Vec3<T>& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

template <typename T>
Vec3<T> Vec3<T>::operator - (const Vec3<T>& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

template <typename T>
Vec3<T>& Vec3<T>::operator -= (const Vec3<T>& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

template <typename T>
Vec3<T> Vec3<T>::operator - () const {
    return Vec3<T>(-x, -y, -z);
}

template <typename T>
const Vec3<T>& Vec3<T>::operator +() const {
    return *this;
}

// ++ 和 --
template <typename T>
Vec3<T>& Vec3<T>::operator ++ () {
    ++x;
    ++y;
    ++z;
    return *this;
}

template <typename T>
Vec3<T> Vec3<T>::operator ++ (int) {
    Vec3<T> ret(x, y, z);
    x++;
    y++;
    z++;
    return ret;
}

template <typename T>
Vec3<T>& Vec3<T>::operator -- () {
    --x;
    --y;
    --z;
    return *this;
}

template <typename T>
Vec3<T> Vec3<T>::operator -- (int) {
    Vec3<T> ret(x, y, z);
    x--;
    y--;
    z--;
    return *this;
}

// 数乘
template <typename T>
Vec3<T> Vec3<T>::operator * (const T& n) const {
    return Vec3<T>(x * n, y * n, z * n);
}
template <typename T>
Vec3<T>& Vec3<T>::operator *= (const T& n) {
    x *= n;
    y *= n;
    z *= n;
    return *this;
}

// 数除
template <typename T>
Vec3<T> Vec3<T>::operator / (const T& n) const {
    assert(n != 0);
    T inv = T(1) / n;
    return Vec3<T>(x * inv, y * inv, z * inv);
}

template <typename T>
Vec3<T>& Vec3<T>::operator /= (const T& n) {
    assert(n != 0);
    T inv = T(1) / n;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

// 按分量相乘
template <typename T>
Vec3<T> Vec3<T>::operator * (const Vec3<T>& v) const {
    return Vec3<T>(x * v.x, y * v.y, z * v.z);
}

template <typename T>
Vec3<T>& Vec3<T>::operator *= (const Vec3<T>& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

// 按分量相除
template <typename T>
Vec3<T> Vec3<T>::operator / (const Vec3<T>& v) const {
    assert( v.x != 0 && v.y != 0 && v.z != 0);
    return Vec3<T>(x / v.x, y / v.y, z / v.z);
}

template <typename T>
Vec3<T>& Vec3<T>::operator /= (const Vec3<T>& v) {
    assert( v.x != 0 && v.y != 0 && v.z != 0);
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

// 点积
template <typename T>
T Vec3<T>::dot(const Vec3<T> &v) const {
    return x * v.x + y * v.y + z * v.z;
}

// 叉积
template <typename T>
Vec3<T> Vec3<T>::cross(const Vec3<T>& v) const {
    T xx = y * v.z - z * v.y;
    T yy = z * v.x - x * v.z;
    T zz = x * v.y - y * v.x;
    return Vec3<T>(xx, yy, zz);
}

// 模
template <typename T>
T Vec3<T>::norm() const {
    return x * x + y * y + z * z;
}

// 长度
template <typename T>
T Vec3<T>::length() const {
    return sqrt(norm());
}

// 下标访问运算
template <typename T>
const T& Vec3<T>::operator[] (uint8_t i) const {
    assert(i < 3);
    return (&x)[i];
}
template <typename T>
T& Vec3<T>::operator[] (uint8_t i) {
    assert(i < 3);
    return (&x)[i];
}

// 规范化
template <typename T>
Vec3<T>& Vec3<T>::normalize() { 
    T n = norm(); 
    if (n > 0) { 
        T factor = 1 / sqrt(n); 
        x *= factor, y *= factor, z *= factor; 
    }
    return *this; 
}

// 旋转向量
template <typename T>
Vec3<T>& Vec3<T>::rotate(T angle, const Vec3<T>& axis) {
    const T sinHalfAngle = sin(toRadian(angle / 2));
    const T cosHalfAngle = cos(toRadian(angle / 2));

    const T rx = axis.x * sinHalfAngle;
    const T ry = axis.y * sinHalfAngle;
    const T rz = axis.z * sinHalfAngle;
    const T rw = cosHalfAngle;

    Quaternion<T> q(rx, ry, rz, rw);
    Quaternion<T> qc = q.getConjugate();
    Quaternion<T> w = q * (*this) * qc;

    x = w.x;
    y = w.y;
    z = w.z;

    return *this;
}

template <typename T>
Vec3<T> Vec3<T>::normalize(const Vec3<T>& v) {
    Vec3<T> n(v);
    return n.normalize();
}

template <typename T>
Vec3<T> Vec3<T>::cross(const Vec3<T>& u, const Vec3<T>& v) {
    return u.cross(v);
}

template <typename T>
T Vec3<T>::dot(const Vec3<T>& u, const Vec3<T>& v) {
    return u.dot(v);
}

// 重载流输出运算符
template <typename T>
std::ostream& operator << (std::ostream &os, const Vec3<T> &v) {
    return os << '(' << v.x << ',' << v.y << ',' << v.z << ')';
}

// 左数乘运算符
template <typename T>
Vec3<T> operator * (const T& n, const Vec3<T>& v) {
    return Vec3<T> (n * v.x, n * v.y, n * v.z);
}

// 单位球面坐标转换到单位（笛卡尔坐标）向量
template <typename T> 
Vec3<T> sphericalToCartesian(const T &theta, const T &phi) { 
    T x = cos(phi) * sin(theta);
    T y = sin(phi) * sin(theta);
    T z = cos(theta);
    return Vec3<T>(x, y, z); 
}

// 计算单位（笛卡尔坐标）向量的单位球面坐标:Theta
template <typename T> 
inline T sphericalTheta(const Vec3<T> &v) { 
    return acos(clamp<T>(v[2], -1, 1)); 
}

// 计算单位（笛卡尔坐标）向量的单位球面坐标:Phi
template <typename T> 
inline T sphericalPhi(const Vec3<T> &v) { 
    T p = atan2(v[1], v[0]); 
    return (p < 0) ? p + 2 * M_PI : p; 
}

// 计算单位（笛卡尔坐标）向量在单位球面坐标的余弦
template <typename T> 
inline T cosTheta(const Vec3<T> &w) { 
    return w[2]; 
} 

// 计算单位（笛卡尔坐标）向量在单位球面坐标的正弦的平方
template <typename T> 
inline T sinTheta2(const Vec3<T> &w) 
{ 
    return std::max(T(0), 1 - cosTheta(w) * cosTheta(w)); 
} 

// 计算单位（笛卡尔坐标）向量在单位球面坐标Theta的正弦
template <typename T> 
inline T sinTheta(const Vec3<T> &w) 
{ 
    return sqrt(sinTheta2(w)); 
}

// 计算单位（笛卡尔坐标）向量在单位球面坐标Phi的余弦
template<typename T> 
inline T cosPhi(const Vec3<T> &w) 
{ 
    T sintheta = sinTheta(w); 
    if (sintheta == 0) return 1; 
    return clamp<T>(w[0] / sintheta, -1, 1); 
} 

// 计算单位（笛卡尔坐标）向量在单位球面坐标Phi的正弦
template<typename T> 
inline T sinPhi(const Vec3<T> &w) 
{ 
    T sintheta = sinTheta(w); 
    if (sintheta == 0) return 0; 
    return clamp<T>(w[1] / sintheta, -1, 1); 
} 

} // namespace cg

#endif //_CG_VEC3_INL