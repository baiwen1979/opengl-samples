#include <cg_gem3d.h>
#include <cmath>
namespace cg {

template<typename T>
Vec3<T>::Vec3(): x(0), y(0), z(0) {}
template<typename T>
Vec3<T>::Vec3(T xx): x(xx), y(xx), z(xx) {}
template<typename T>
Vec3<T>::Vec3(T xx, T yy, T zz): x(xx), y(yy), z(zz) {}
// 加
template<typename T>
Vec3<T> Vec3<T>::operator + (const Vec3<T>& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}
// 减
template<typename T>
Vec3<T> Vec3<T>::operator - (const Vec3<T>& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}
// 数乘
template<typename T>
Vec3<T> Vec3<T>::operator * (const T& r) const {
    return Vec3(x * r, y * r, z * r);
}
// 点积
template<typename T>
T Vec3<T>::dotProduct(const Vec3<T> &v) const {
    return x * v.x + y * v.y + z * v.z;
}
// 叉积
template<typename T>
Vec3<T> Vec3<T>::crossProduct(const Vec3<T>& v) const {
    T x = y * v.z - z * v.y;
    T y = z * v.x - x * v.z;
    T z = x * v.y - y * v.x;
    return Vec3<T>(x, y, z);
}
// 模
template<typename T>
T Vec3<T>::norm() const {
    return x * x + y * y + z * z;
}
// 长度
template<typename T>
T Vec3<T>::length() const {
    return sqrt(norm());
}
// 下标访问运算
template<typename T>
const T& Vec3<T>::operator[] (uint8_t i) const {
    return (&x)[i];
}
template<typename T>
T& Vec3<T>::operator[] (uint8_t i) {
    return (&x)[i];
}
// 规范化
template<typename T>
Vec3<T>& Vec3<T>::normalize() 
{ 
    T n = norm(); 
    if (n > 0) { 
        T factor = 1 / sqrt(n); 
        x *= factor, y *= factor, z *= factor; 
    } 
 
    return *this; 
}
// 模板类实例化
template struct Vec3<int32_t>;
template struct Vec3<float>;

}