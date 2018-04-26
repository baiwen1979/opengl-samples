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
    Vec3(T x);
    Vec3(T x, T y, T z);
    // 拷贝构造函数
    Vec3(const Vec3<T>& v);
    // 赋值运算符
    Vec3<T>& operator = (const Vec3<T>& v);
    // 向量的+,-
    Vec3<T> operator + (const Vec3<T>& v) const;
    Vec3<T>& operator += (const Vec3<T>& v);
    Vec3<T> operator - (const Vec3<T>& v) const;
    Vec3<T>& operator -= (const Vec3<T>& v);
    // ++ 和 --
    Vec3<T>& operator ++ ();   // ++v
    Vec3<T> operator ++ (int); // v++
    Vec3<T>& operator -- ();   // --v
    Vec3<T> operator -- (int); // v--
    // 向量的数乘 
    Vec3<T> operator * (const T& n) const;
    Vec3<T>& operator *= (const T& n);
    // 向量的分量积
    Vec3<T> operator * (const Vec3<T>& v) const;
    Vec3<T>& operator *= (const Vec3<T>& v);
    // 向量的点积
    T dot(const Vec3<T> &v) const; 
    // 向量的叉积
    Vec3<T> cross(const Vec3<T> &v) const;
    // 向量的模
    T norm() const;
    // 向量的长度
    T length() const;
    // 下标取值运算
    const T& operator [] (uint8_t i) const;
    // 下标设值运算符
    T& operator [] (uint8_t i);
    // 围绕指定轴旋转向量
    Vec3<T>& rotate(T angle, const Vec3<T>& axis);
    // 规范化
    Vec3<T>& normalize();

    // 计算规范化向量
    static Vec3<T> normalize(const Vec3<T>& v);
    // 计算两个向量的叉积
    static Vec3<T> cross(const Vec3<T>& u, const Vec3<T>& v);
    // 计算两个向量的点积　
    static T dot(const Vec3<T>& u, const Vec3<T>& v);

    // 成员变量
    T x, y, z;
};

}
#include "cg_vec3.inl"

#endif //_CG_VEC3_HPP_