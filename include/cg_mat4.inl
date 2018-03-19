#ifndef _CG_MAT4_INL_
#define _CG_MAT4_INL_

#include <iomanip>
#include <iostream>
#include "cg_mat4.hpp"

namespace cg {

template<typename T>
Mat4<T>::Mat4() {
    x[0][0] = 1; x[0][1] = 0; x[0][2] = 0; x[0][3] = 0;
    x[1][0] = 0; x[1][1] = 1; x[1][2] = 0; x[1][3] = 0;
    x[2][0] = 0; x[2][1] = 0; x[2][2] = 1; x[2][3] = 0;
    x[3][0] = 0; x[3][1] = 0; x[3][2] = 0; x[3][3] = 1;
}

template<typename T>
Mat4<T>::Mat4(T a, T b, T c, T d, T e, T f, T g, T h, 
              T i, T j, T k, T l, T m, T n, T o, T p) {
    x[0][0] = a; x[0][1] = b; x[0][2] = c; x[0][3] = d;
    x[1][0] = e; x[1][1] = f; x[1][2] = g; x[1][3] = h;
    x[2][0] = i; x[2][1] = j; x[2][2] = k; x[2][3] = l; 
    x[3][0] = m; x[3][1] = n; x[3][2] = o; x[3][3] = p; 
}

template<typename T>
const T* Mat4<T>::operator [] (uint8_t i) const {
    return x[i];
}

template<typename T>
T* Mat4<T>::operator[] (uint8_t i) {
    return x[i];
}

template<typename T>
Mat4<T> Mat4<T>::operator * (const Mat4<T>& v) const {
    Mat4 tmp;
    multiply(*this, v, tmp);
    return tmp;
}

template<typename T>
Mat4<T> Mat4<T>::transposed() const {
    Mat4 t; 
    for (uint8_t i = 0; i < 4; ++i) { 
        for (uint8_t j = 0; j < 4; ++j) { 
            t[i][j] = x[j][i]; 
        } 
    } 
    return t;
}

template<typename T>
Mat4<T>& Mat4<T>::transpose() {
    Mat4 tmp (
        x[0][0], x[1][0], x[2][0], x[3][0], 
        x[0][1], x[1][1], x[2][1], x[3][1], 
        x[0][2], x[1][2], x[2][2], x[3][2], 
        x[0][3], x[1][3], x[2][3], x[3][3]
    ); 
    *this = tmp; 
    return *this;    
}

template<typename T>
template<typename S>
void Mat4<T>::multVertex3Matrix(const Vec3<S> &src, Vec3<S> &dst) const {
    S a, b, c, w; 
 
    a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0]; 
    b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1]; 
    c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2]; 
    w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3]; 
 
    dst.x = a / w; 
    dst.y = b / w; 
    dst.z = c / w; 

}

template<typename T>
template<typename S>
void Mat4<T>::multVector3Matrix(const Vec3<S> &src, Vec3<S> &dst) const {
    S a, b, c; 
 
    a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0]; 
    b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1]; 
    c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2]; 
 
    dst.x = a; 
    dst.y = b; 
    dst.z = c; 

}

template<typename T>
Mat4<T> Mat4<T>::inverse() {
    int i, j, k; 
    Mat4 s; 
    Mat4 t (*this); 

    // Forward elimination
    for (i = 0; i < 3 ; i++) { 
        int pivot = i; 

        T pivotsize = t[i][i]; 

        if (pivotsize < 0) 
            pivotsize = -pivotsize; 

            for (j = i + 1; j < 4; j++) { 
                T tmp = t[j][i]; 

                if (tmp < 0) 
                    tmp = -tmp; 

                    if (tmp > pivotsize) { 
                        pivot = j; 
                        pivotsize = tmp; 
                    } 
            } 

        if (pivotsize == 0) { 
            // Cannot invert singular matrix
            return Mat4(); 
        } 

        if (pivot != i) { 
            for (j = 0; j < 4; j++) { 
                T tmp; 

                tmp = t[i][j]; 
                t[i][j] = t[pivot][j]; 
                t[pivot][j] = tmp; 

                tmp = s[i][j]; 
                s[i][j] = s[pivot][j]; 
                s[pivot][j] = tmp; 
            } 
        } 

        for (j = i + 1; j < 4; j++) { 
            T f = t[j][i] / t[i][i]; 

            for (k = 0; k < 4; k++) { 
                t[j][k] -= f * t[i][k]; 
                s[j][k] -= f * s[i][k]; 
            } 
        } 
    } 

    // Backward substitution
    for (i = 3; i >= 0; --i) { 
        T f; 

        if ((f = t[i][i]) == 0) { 
            // Cannot invert singular matrix
            return Mat4(); 
        } 

        for (j = 0; j < 4; j++) { 
            t[i][j] /= f; 
            s[i][j] /= f; 
        } 

        for (j = 0; j < i; j++) { 
            f = t[j][i]; 

            for (k = 0; k < 4; k++) { 
                t[j][k] -= f * t[i][k]; 
                s[j][k] -= f * s[i][k]; 
            } 
        } 
    }

    return s;
}

template<typename T>
const Mat4<T>& Mat4<T>::invert() {
    *this = inverse(); 
    return *this;
}

template<typename T>
void Mat4<T>::multiply(const Mat4<T> &a, const Mat4& b, Mat4 &c) {
    for (uint8_t i = 0; i < 4; ++i) { 
        for (uint8_t j = 0; j < 4; ++j) { 
            c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + 
                a[i][2] * b[2][j] + a[i][3] * b[3][j]; 
        } 
    } 
}

template<typename T>
std::ostream& operator << (std::ostream &s, const Mat4<T> &m) { 
    std::ios_base::fmtflags oldFlags = s.flags(); 
    int width = 12; // total with of the displayed number 
    s.precision(5); // control the number of displayed decimals 
    s.setf (std::ios_base::fixed); 
    
    s << 
        "(" << std::setw (width) << m[0][0] << 
        " " << std::setw (width) << m[0][1] << 
        " " << std::setw (width) << m[0][2] << 
        " " << std::setw (width) << m[0][3] << "\n" << 

        " " << std::setw (width) << m[1][0] << 
        " " << std::setw (width) << m[1][1] << 
        " " << std::setw (width) << m[1][2] << 
        " " << std::setw (width) << m[1][3] << "\n" << 

        " " << std::setw (width) << m[2][0] << 
        " " << std::setw (width) << m[2][1] << 
        " " << std::setw (width) << m[2][2] << 
        " " << std::setw (width) << m[2][3] << "\n" << 

        " " << std::setw (width) << m[3][0] << 
        " " << std::setw (width) << m[3][1] << 
        " " << std::setw (width) << m[3][2] << 
        " " << std::setw (width) << m[3][3] << ")\n"; 

    s.flags (oldFlags); 
    return s; 
}

}

#endif //_CG_MAT4_INL_