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
    Mat4(T v);
    Mat4(
        const T& a, const T& b, const T& c, const T& d, 
        const T& e, const T& f, const T& g, const T& h, 
        const T& i, const T& j, const T& k, const T& l, 
        const T& m, const T& n, const T& o, const T& p);

    Mat4(const Mat4<T>& m);

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
    void multPos(const Vec3<S> &src, Vec3<S> &dst) const;
    // 向量 * 当前矩阵
    template<typename S> 
    void multDir(const Vec3<S> &src, Vec3<S> &dst) const;

    // 返回当前矩阵的逆矩阵
    Mat4<T> inverse();
    // 将当前矩阵设置为其逆矩阵
    const Mat4<T>& invert();

    /* 静态成员 */
    // 乘
    static void multiply(const Mat4<T>& a, const Mat4<T>& b, Mat4<T>& c);

    // 平移矩阵
    static Mat4<T> translate(const Mat4<T>& m, T tx, T ty, T tz);
    static Mat4<T> translate(const Mat4<T>& m, const Vec3<T>& v);
    // 旋转矩阵
    static Mat4<T> rotateX(const Mat4<T>& m, T angle);
    static Mat4<T> rotateY(const Mat4<T>& m, T angle);
    static Mat4<T> rotateZ(const Mat4<T>& m, T angle);
    static Mat4<T> rotate(const Mat4<T>& m, T angle, const Vec3<T>& v);
    // 缩放矩阵
    static Mat4<T> scale(const Mat4<T>& m, T sx, T sy, T sz);
    static Mat4<T> scale(const Mat4<T>& m, const Vec3<T>& v);
    // 正交投影矩阵
    static Mat4<T> ortho(T left, T right, T top, T bottom, T zNear, T zFar);
    // 透视投影矩阵
    static Mat4<T> perspective(T fovy, T aspect, T zNear, T zFar);
    // 观察视图矩阵
    static Mat4<T> lookAt(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up);

    /* 成员变量 */
    T x[4][4];
};

}

#include "cg_mat4.inl"

#endif // _CG_MAT4_HPP_