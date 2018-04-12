#ifndef _CG_MAT3_INL_
#define _CG_MAT3_INL_

#include <iomanip>
#include <iostream>

#include "cg_mat3.hpp"

namespace cg {

template <typename T>
Mat3<T>::Mat3() {
    m[0][0] = T(1); m[0][1] = T(0); m[0][2] = T(0);
    m[1][0] = T(0); m[1][1] = T(1); m[1][2] = T(0);
    m[2][0] = T(0); m[2][1] = T(0); m[2][2] = T(1);
}

template <typename T>
Mat3<T>::Mat3(const T& v) {
    m[0][0] = v;    m[0][1] = T(0);    m[0][2] = T(0);
    m[1][0] = T(0); m[1][1] = v;       m[1][2] = T(0);
    m[2][0] = T(0); m[2][1] = T(0);    m[2][2] = v;
}

template <typename T>
Mat3<T>::Mat3(
    const T& a, const T& b, const T& c,
    const T& d, const T& e, const T& f,
    const T& g, const T& h, const T& i) {
    m[0][0] = a; m[0][1] = b; m[0][2] = c;
    m[1][0] = d; m[1][1] = e; m[1][2] = f;
    m[2][0] = g; m[2][1] = h; m[2][2] = i;
}

template <typename T>
Mat3<T>::Mat3(const Mat3<T>& b) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m[i][j] = b[i][j];
        }
    }
}

template <typename T>
Mat3<T> Mat3<T>::operator + (const Mat3<T>& b) const {
    return Mat3(
        m[0][0] + b[0][0], m[0][1] + b[0][1], m[0][2] + b[0][2],
        m[1][0] + b[1][0], m[1][1] + b[1][1], m[1][2] + b[1][2],
        m[2][0] + b[2][0], m[2][1] + b[2][1], m[2][2] + b[2][2]
    );
}

template <typename T>
Mat3<T> Mat3<T>::operator - (const Mat3<T>& b) const {
    return Mat3(
        m[0][0] - b[0][0], m[0][1] + b[0][1], m[0][2] + b[0][2],
        m[1][0] - b[1][0], m[1][1] + b[1][1], m[1][2] + b[1][2],
        m[2][0] - b[2][0], m[2][1] + b[2][1], m[2][2] + b[2][2]
    );
}

template <typename T>
Mat3<T> Mat3<T>::operator * (const Mat3<T>& b) const {
    Mat3<T> c;
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            c[i][j] = m[i][0] * b[0][j] + m[i][1] * b[1][j] + 
                m[i][2] * b[2][j];
        }
    }
    return c;
}

template <typename T>
Mat3<T> Mat3<T>::operator * (const T& b) const {
    Mat3<T> c;
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            c[i][j] = m[i][j] * b;
        }
    }
    return c; 
}

template <typename T>
Vec2<T> Mat3<T>::operator * (const Vec2<T>& v) const {
    T vx = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
    T vy = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
    T vh = m[2][0] * v.x + m[2][1] * v.y + m[2][2];
    return Vec2<T>(vx / vh, vy / vh);
}

template <typename T>
const T* Mat3<T>::operator[] (uint8_t i) const {
    assert (i < 3);
    return m[i];
}

template <typename T>
T* Mat3<T>::operator[] (uint8_t i) {
    assert (i < 3);
    return m[i];
}

template <typename T>
Mat3<T>& Mat3<T>::transpose() {
    *this = transposed();;
    return *this;
}

template <typename T>
Mat3<T> Mat3<T>::transposed() const {
    Mat3 t; 
    for (uint8_t i = 0; i < 3; ++i) { 
        for (uint8_t j = 0; j < 3; ++j) { 
            t[i][j] = m[j][i]; 
        } 
    } 
    return t;
}

template <typename T>
Mat3<T> Mat3<T>::translate(const Mat3<T>& m, const T& tx, const T& ty) {
    // 创建平移矩阵
    Mat3<T> tm (
        T(1), T(0), tx,
        T(0), T(1), ty,
        T(0), T(0), T(1)
    );
    return tm * m;
}

template <typename T>
Mat3<T> Mat3<T>::translate(const Mat3<T>& m, const Vec2<T>& tv) {
    return translate(m, tv.x, tv.y);
}

template <typename T>
Mat3<T> Mat3<T>::rotate(const Mat3<T>& m, const T& angle) {
    const T cos0 = cos(toRadian(angle));
    const T sin0 = sin(toRadian(angle));
    // 创建旋转矩阵
    Mat3<T> rm (
        cos0, -sin0, T(0),
        sin0, cos0,  T(0),
        T(0), T(0),  T(1)
    );
    return rm * m; 
}

template <typename T>
Mat3<T> Mat3<T>::rotate(const Mat3<T>& m, const T& angle, const T& px, const T& py) {
    //（1）移动轴点到原点
    Mat3<T> t = translate(m, -px, -py);
    //（2）绕原点旋转
    t = rotate(t, angle);
    //（3）移回
    t = translate(t, px, py);
    return t;
}

template <typename T>
Mat3<T> Mat3<T>::rotate(const Mat3<T>& m, const T& angle, const Vec2<T>& pv) {
    return rotate(m, angle, pv.x, pv.y);
}

template <typename T>
Mat3<T> Mat3<T>::scale(const Mat3<T>& m, const T& sx, const T& sy) {
    // 创建缩放矩阵
    Mat3<T> sm(
        sx,   T(0), T(0),
        T(0), sy,   T(0),
        T(0), T(0), T(1)
    );
    return sm * m;
}

template <typename T>
Mat3<T> Mat3<T>::scale(const Mat3<T>& m, const Vec2<T>& sv) {
    return scale(m, sv.x, sv.y);
}

template<typename T>
std::ostream& operator << (std::ostream &s, const Mat3<T> &m) { 
    std::ios_base::fmtflags oldFlags = s.flags(); 
    int width = 12; // total with of the displayed number 
    s.precision(5); // control the number of displayed decimals 
    s.setf (std::ios_base::fixed); 
    
    s << 
        " " << std::setw (width) << m[0][0] << 
        " " << std::setw (width) << m[0][1] << 
        " " << std::setw (width) << m[0][2] << "\n" << 

        " " << std::setw (width) << m[1][0] << 
        " " << std::setw (width) << m[1][1] << 
        " " << std::setw (width) << m[1][2] << "\n" << 

        " " << std::setw (width) << m[2][0] << 
        " " << std::setw (width) << m[2][1] << 
        " " << std::setw (width) << m[2][2] << "\n";

    s.flags (oldFlags); 
    return s; 
}


} // namespace

#endif // _CG_MAT3_INL_