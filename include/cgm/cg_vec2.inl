#ifndef _CG_VEC2_INL
#define _CG_VEC2_INL

#include <iostream>
#include "cg_vec2.hpp"

namespace cg {

template <typename T>
Vec2<T>::Vec2(): x(0), y(0) {}

template <typename T>
Vec2<T>::Vec2(T v): x(v), y(v) {}

template <typename T>
Vec2<T>::Vec2(T xx, T yy): x(xx), y(yy) {}

template <typename T>
Vec2<T>::Vec2(const Vec2<T>& v): x(v.x), y(v.y) {}

template <typename T>
Vec2<T>& Vec2<T>::operator = (const Vec2<T>& v) {
    x = v.x;
    y = v.y;
    return *this;
}

template <typename T>
Vec2<T> Vec2<T>::operator + (const Vec2<T>& v) const {
    return Vec2<T>(x + v.x, y + v.y);
}

template <typename T>
Vec2<T>& Vec2<T>::operator += (const Vec2<T>& v) {
    x += v.x;
    y += v.y;
    return *this;
}

template <typename T>
Vec2<T> Vec2<T>::operator - (const Vec2<T>& v) const {
    return Vec2<T>(x - v.x, y - v.y);
}

template <typename T>
Vec2<T>& Vec2<T>::operator -= (const Vec2<T>& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::operator ++ () {
    ++x;
    ++y;
    return *this;
}

template <typename T>
Vec2<T> Vec2<T>::operator ++ (int) {
    Vec2<T> ret(x, y);
    x++;
    y++;
    return ret;
}

template <typename T>
Vec2<T>& Vec2<T>::operator -- () {
    --x;
    --y;
    return *this;
}

template <typename T>
Vec2<T> Vec2<T>::operator -- (int) {
    Vec2<T> ret(x, y);
    x--;
    y--;
    return ret;
}

template <typename T>
Vec2<T> Vec2<T>::operator * (const T& v) const {
    return Vec2<T>(x * v, y * v);
}

template <typename T>
Vec2<T>& Vec2<T>::operator *= (const T& v) {
    x *= v;
    y *= v;
    return *this;
}

template <typename T>
Vec2<T> Vec2<T>::operator * (const Vec2<T>& v) const {
    return Vec2<T>(x * v.x, y * v.y);
}

template <typename T>
Vec2<T>& Vec2<T>::operator *= (const Vec2<T>& v) {
    x *= v.x;
    y *= v.y;
    return *this;
}


template <typename T>
const T& Vec2<T>::operator[] (uint8_t i) const {
    assert(i < 2);
    return (&x)[i];
}

template<typename T>
T& Vec2<T>::operator[] (uint8_t i) {
    assert(i < 2);
    return (&x)[i];
}

template<typename T>
T Vec2<T>::norm() const {
    return x * x + y * y;
}

template<typename T>
T Vec2<T>::length() const {
    return sqrt(norm());
}

template<typename T>
Vec2<T>& Vec2<T>::normalize() {
    T len = length();
    if (len > 0) {
        x = x / len;
        y = y / len;
    }
    return *this;
}

template<typename T>
T Vec2<T>::dot(const Vec2<T>& v) const {
    return x * v.x + y * v.y;
}

// 重载流输出运算符
template<typename T>
std::ostream& operator << (std::ostream &os, const Vec2<T> &v) {
    return os << '(' << v.x << ',' << v.y << ')';
}

// 左数乘运算符
template<typename T>
Vec2<T> operator * (const T& n, const Vec2<T>& v) {
    return Vec2<T>(n * v.x, n * v.y);
}

}

#endif