#ifndef _CG_VEC2_HPP_
#define _CG_VEC2_HPP_

#include <cmath>
#include <cstdlib>

namespace cg {

// 模板类：二维向量
template<typename T>
class Vec2 {
public:
    /*** 构造函数 ***/
    // 构造函数
    Vec2();
    Vec2(T v);
    Vec2(T x, T y);
    // 拷贝构造函数
    Vec2(const Vec2<T>& v);

    /*** 运算符 ***/
    // 赋值运算符
    Vec2<T>& operator = (const Vec2<T>& v);
    // + 和 +=
    Vec2<T> operator + (const Vec2<T>& v) const;
    Vec2<T>& operator += (const Vec2<T>& v);
    // - 和 -=
    Vec2<T> operator - (const Vec2<T>& v) const;
    Vec2<T>& operator -= (const Vec2<T>& v);
    // ++ 和 --
    Vec2<T>& operator ++ ();
    Vec2<T> operator ++ (int);
    Vec2<T>& operator -- ();
    Vec2<T> operator -- (int);
    // 数乘
    Vec2<T> operator * (const T& v) const;
    Vec2<T>& operator *= (const T& v);
    // 下标取值运算符
    const T& operator [] (uint8_t i) const;
    // 下标设值运算符
    T& operator [] (uint8_t i);

    /*** 成员函数 ***/
    // 点积
    T dot(const Vec2<T>& v) const;
    // 模
    T norm() const;
    // 长度
    T length() const;
    // 规范化
    Vec2<T>& normalize();

    /*** 成员变量 ***/
    T x, y;
};

}

#include "cg_vec2.inl"

#endif