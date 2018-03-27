// 3D几何对象
#ifndef _CG_VEC3_HPP_
#define _CG_VEC3_HPP_

#include <cstdlib>
#include <iostream>
namespace cg {

// 模板类：3维向量
template<typename T>
class Vec3 {
public:
    // 构造函数
    Vec3();
    Vec3(T xx);
    Vec3(T xx, T yy, T zz);
    // 向量的+,-
    Vec3<T> operator + (const Vec3<T>& v) const;
    Vec3<T>& operator += (const Vec3& v);
    Vec3<T> operator - (const Vec3<T>& v) const;
    Vec3<T>& operator -= (const Vec3& v);
    // 向量的数乘 
    Vec3<T> operator * (const T&r) const;
    Vec3<T>& operator *= (const T& r);
    // 向量的点积
    T dotProduct(const Vec3<T> &v) const; 
    // 向量的叉积
    Vec3<T> crossProduct(const Vec3<T> &v) const;
    // 向量的模
    T norm() const;
    // 向量的长度
    T length() const;
    // 下标取值运算
    const T& operator [] (uint8_t i) const;
    // 下标设值运算符
    T& operator [] (uint8_t i);
    // 向量规范化
    Vec3<T>& normalize();
    // 成员变量
    T x, y, z;
};

}
#include "cg_vec3.inl"

#endif //_CG_VEC3_HPP_