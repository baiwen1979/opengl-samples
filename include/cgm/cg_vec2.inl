#ifndef _CG_VEC2_INL
#define _CG_VEC2_INL

#include <iostream>
#include <cassert>
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
bool Vec2<T>::operator == (const Vec2<T>& v) const {
    return (x == v.x && y == v.y);
}

template <typename T>
bool Vec2<T>::operator != (const Vec2<T>& v) const {
    return !(*this == v);
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
Vec2<T> Vec2<T>::operator - () const {
    return Vec2<T>(-x, -y);
}

template <typename T>
const Vec2<T>& Vec2<T>::operator +() const {
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
Vec2<T> Vec2<T>::operator * (const T& n) const {
    return Vec2<T>(x * n, y * n);
}

template <typename T>
Vec2<T>& Vec2<T>::operator *= (const T& n) {
    x *= n;
    y *= n;
    return *this;
}

template <typename T>
Vec2<T> Vec2<T>::operator / (const T& n) const {
    assert( n != T(0));
    T inv = T(1) / n;
    return Vec2<T>(inv * x, inv * y);
}

template <typename T>
Vec2<T>& Vec2<T>::operator /= (const T& n) {
    assert( n != T(0));
    T inv = T(1) / n;
    x *= inv;
    y *= inv;
    return *this;
}

template <typename T>
Vec2<T> Vec2<T>::operator / (const Vec2<T>& v) const {
    assert(v.x != T(0) && v.y != T(0));
    return Vec2<T>(T(1) / v.x, T(1) / v.y);
}

template <typename T>
Vec2<T>& Vec2<T>::operator /= (const Vec2<T>& v) {
    assert(v.x != T(0) && v.y != T(0));
    x /= v.x;
    y /= v.y;
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
const T& Vec2<T>::operator[] (unsigned char i) const {
    assert(i < 2);
    return (&x)[i];
}

template<typename T>
T& Vec2<T>::operator[] (unsigned char i) {
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