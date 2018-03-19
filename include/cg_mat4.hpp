#ifndef _CG_MAT4_HPP_
#define _CG_MAT4_HPP_

#include <cstdlib>
#include <iostream>

#include "cg_vec3.hpp"

namespace cg {

// 模板类：4x4矩阵
template<typename T> 
class Mat4 
{ 
public:
    /* 构造函数 */
    Mat4();
    Mat4 (T a, T b, T c, T d, T e, T f, T g, T h, 
          T i, T j, T k, T l, T m, T n, T o, T p);

    /* 成员运算符 */
    // 下标取值
    const T* operator [] (uint8_t i) const;
    // 下标设值
    T* operator [] (uint8_t i);

    // 矩阵相乘
    Mat4<T> operator * (const Mat4<T>& v) const;

    // 返回当前矩阵的转置
    Mat4<T> transposed() const;   
    // 将当前矩阵转置
    Mat4<T>& transpose ();

    // 顶点 * 当前矩阵
    template<typename S> 
    void multVertex3Matrix(const Vec3<S> &src, Vec3<S> &dst) const;
    // 向量 * 当前矩阵
    template<typename S> 
    void multVector3Matrix(const Vec3<S> &src, Vec3<S> &dst) const;

    // 返回当前矩阵的逆矩阵
    Mat4<T> inverse();
    // 将当前矩阵设置为其逆矩阵
    const Mat4<T>& invert();

    /* 静态成员 */
    static void multiply(const Mat4<T> &a, const Mat4& b, Mat4 &c);

    /* 成员变量 */
    T x[4][4];
};

}

#include "cg_mat4.inl"

#endif // _CG_MAT4_HPP_