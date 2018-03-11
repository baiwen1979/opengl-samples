// 3D几何对象
#ifndef _CG_GEM3D_H_
#define _CG_GEM3D_H_

#include <cstdlib>
#include <iostream>
// 模板类：3维向量
namespace cg {

template<typename T>
class Vec3 {
public:
    // 构造函数
    Vec3();
    Vec3(T xx);
    Vec3(T xx, T yy, T zz);
    // 向量的+,-
    Vec3 operator + (const Vec3 &v) const;
    Vec3 operator - (const Vec3 &v) const;
    // 向量的数乘 
    Vec3 operator * (const T &r) const;
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
    Vec3& normalize();
    // 成员变量
    T x, y, z;
};

typedef Vec3<float> Vec3f;
typedef Vec3<int32_t> Vec3i;

}
#include "cg_gem3d.inl"

#endif //_CG_GEM3D_H_