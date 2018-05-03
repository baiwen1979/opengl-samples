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
    // v1 = v2
    Vec2<T>& operator = (const Vec2<T>& v);
    // v1 == v2 和 v1 != v2
    bool operator == (const Vec2<T>& v) const;
    bool operator != (const Vec2<T>& v) const;
    // v1 + v2 和 v1 += v2
    Vec2<T> operator + (const Vec2<T>& v) const;
    Vec2<T>& operator += (const Vec2<T>& v);
    // v1 - v2 和 v1 -= v2
    Vec2<T> operator - (const Vec2<T>& v) const;
    Vec2<T>& operator -= (const Vec2<T>& v);
    // -v 和 +v
    Vec2<T> operator - () const;
    const Vec2<T>& operator + () const;
    // ++v, v++ 和 --v, v-- 
    Vec2<T>& operator ++ ();
    Vec2<T> operator ++ (int);
    Vec2<T>& operator -- ();
    Vec2<T> operator -- (int);
    // v * 2 和 v *= 2
    Vec2<T> operator * (const T& n) const;
    Vec2<T>& operator *= (const T& n);
    // v / 2 和 v /= 2
    Vec2<T> operator / (const T& n) const;
    Vec2<T>& operator /= (const T& n);
    // v1 * v2 和 v1 *= v2
    Vec2<T> operator * (const Vec2<T>& v) const;
    Vec2<T>& operator *= (const Vec2<T>& v);
    // v1 / v2 和 v1 /= v2
    Vec2<T> operator / (const Vec2<T>& v) const;
    Vec2<T>& operator /= (const Vec2<T>& v);
    // v1 = v[i]
    const T& operator [] (uint8_t i) const;
    // v[i] = v1
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