#ifndef _CG_MAT4_INL_
#define _CG_MAT4_INL_

#include <iomanip>
#include <iostream>
#include "cg_mat4.hpp"

namespace cg {

template <typename T>
Mat4<T>::Mat4() {
    x[0][0] = 1; x[0][1] = 0; x[0][2] = 0; x[0][3] = 0;
    x[1][0] = 0; x[1][1] = 1; x[1][2] = 0; x[1][3] = 0;
    x[2][0] = 0; x[2][1] = 0; x[2][2] = 1; x[2][3] = 0;
    x[3][0] = 0; x[3][1] = 0; x[3][2] = 0; x[3][3] = 1;
}

template <typename T>
Mat4<T>::Mat4(T v) {
    x[0][0] = v; x[0][1] = 0; x[0][2] = 0; x[0][3] = 0;
    x[1][0] = 0; x[1][1] = v; x[1][2] = 0; x[1][3] = 0;
    x[2][0] = 0; x[2][1] = 0; x[2][2] = v; x[2][3] = 0;
    x[3][0] = 0; x[3][1] = 0; x[3][2] = 0; x[3][3] = v;
}

template <typename T>
Mat4<T>::Mat4(
    const T& a, const T& b, const T& c, const T& d, 
    const T& e, const T& f, const T& g, const T& h, 
    const T& i, const T& j, const T& k, const T& l, 
    const T& m, const T& n, const T& o, const T& p) {    
    x[0][0] = a; x[0][1] = b; x[0][2] = c; x[0][3] = d;
    x[1][0] = e; x[1][1] = f; x[1][2] = g; x[1][3] = h;
    x[2][0] = i; x[2][1] = j; x[2][2] = k; x[2][3] = l; 
    x[3][0] = m; x[3][1] = n; x[3][2] = o; x[3][3] = p;
}

template <typename T>
Mat4<T>::Mat4(const Mat4<T>& m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            x[i][j] = m[i][j];
        }
    }
}

template <typename T>
const T* Mat4<T>::operator [] (uint8_t i) const {
    assert(i < 4);
    return x[i];
}

template <typename T>
T* Mat4<T>::operator[] (uint8_t i) {
    assert(i < 4);
    return x[i];
}

template <typename T>
Mat4<T> Mat4<T>::operator * (const Mat4<T>& v) const {
    Mat4<T> ret;
    multiply(*this, v, ret);
    return ret;
}

template <typename T>
Mat4<T> Mat4<T>::transposed() const {
    Mat4 t; 
    for (uint8_t i = 0; i < 4; ++i) { 
        for (uint8_t j = 0; j < 4; ++j) { 
            t[i][j] = x[j][i]; 
        } 
    } 
    return t;
}

template <typename T>
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

template <typename T>
template <typename S>
void Mat4<T>::multPos(const Vec3<S> &src, Vec3<S> &dst) const {
    S a, b, c, w; 
 
    a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0]; 
    b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1]; 
    c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2]; 
    w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3]; 
 
    dst.x = a / w; 
    dst.y = b / w; 
    dst.z = c / w; 

}

template <typename T>
template <typename S>
void Mat4<T>::multDir(const Vec3<S> &src, Vec3<S> &dst) const {
    S a, b, c; 
 
    a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0]; 
    b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1]; 
    c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2]; 
 
    dst.x = a; 
    dst.y = b; 
    dst.z = c; 

}

template <typename T>
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

template <typename T>
void Mat4<T>::multiply(const Mat4<T> &a, const Mat4& b, Mat4 &c) {
    for (uint8_t i = 0; i < 4; ++i) { 
        for (uint8_t j = 0; j < 4; ++j) { 
            c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + 
                a[i][2] * b[2][j] + a[i][3] * b[3][j]; 
        } 
    } 
}

template <typename T>
Mat4<T> Mat4<T>::translate(const Mat4<T>& m, T tx, T ty, T tz) {
    Mat4<T> mt(
        T(1), T(0), T(0), T(tx),
        T(0), T(1), T(0), T(ty),
        T(0), T(0), T(1), T(tz),
        T(0), T(0), T(0), T(1)
    );
    return mt * m;
}

template <typename T>
Mat4<T> Mat4<T>::translate(const Mat4<T>& m, const Vec3<T>& v) {
    return translate(m, v.x, v.y, v.z);
}

template <typename T>
Mat4<T> Mat4<T>::scale(const Mat4<T>& m, T sx, T sy, T sz) {
    Mat4<T> ms(
        T(sx), T(0),  T(0),  T(0),
        T(0),  T(sy), T(0),  T(0),
        T(0),  T(0),  T(sz), T(0),
        T(0),  T(0),  T(0),  T(1)
    );
    return ms * m;
}

template <typename T>
Mat4<T> Mat4<T>::scale(const Mat4<T>& m, const Vec3<T>& v) {
    return scale(m, v.x, v.y, v.z);
}

template <typename T>
Mat4<T> Mat4<T>::rotateX(const Mat4<T>& m, T angle) {
    const T rx = toRadian(angle);
    const T cos0 = cos(rx);
    const T sin0 = sin(rx);
    Mat4<T> mrx(
        T(1), T(0),    T(0),     T(0),
        T(0), T(cos0), T(-sin0), T(0),
        T(0), T(sin0), T(cos0),  T(0),
        T(0), T(0),    T(0),     T(1)
    );
    return mrx * m;
}

template <typename T>
Mat4<T> Mat4<T>::rotateY(const Mat4<T>& m, T angle) {
    const T ry = toRadian(angle);
    const T cos0 = cos(ry);
    const T sin0 = sin(ry);
    Mat4<T> mry(
        T(cos0),  T(0), T(sin0), T(0),
        T(0),     T(1), T(0),    T(0),
        T(-sin0), T(0), T(cos0), T(0),
        T(0),     T(0), T(0),    T(1)
    );
    return mry * m;
}

template <typename T>
Mat4<T> Mat4<T>::rotateZ(const Mat4<T>& m, T angle) {    
    const T rz = toRadian(angle);  
    const T cos0 = cos(rz);
    const T sin0 = sin(rz);
    Mat4<T> mrz(
        T(cos0), T(-sin0), T(0), T(0),
        T(sin0), T(cos0),  T(0), T(0),
        T(0),    T(0),     T(1), T(0),
        T(0),    T(0),     T(0), T(1)       
    );
    return mrz * m;
}

template <typename T>
Mat4<T> Mat4<T>::rotate(const Mat4<T>&m, T angle, const Vec3<T>& v) {
    const T a = toRadian(angle);
	const T cos0 = cos(a);
	const T sin0 = sin(a);

    Vec3<T> axis = Vec3<T>::normalize(v);
    const T rx = axis.x;
    const T ry = axis.y;
    const T rz = axis.z; 

    Mat4<T> mr(
        cos0 + rx * rx * (1 - cos0),      rx * ry * (1 - cos0) - rz * sin0, rx * rz * (1 - cos0) + ry * sin0, 0,
        ry * rx * (1 - cos0) + rz * sin0, cos0 + ry * ry * (1 - cos0),      ry * rz * (1 - cos0) - rx * sin0, 0,
        rz * rx * (1 - cos0) - ry * sin0, rz * ry * (1 - cos0) + rx * sin0, cos0 + rz * rz * (1 - cos0),      0,
        0,                                0,                                0,                                1
    );

    return mr * m;
}

template <typename T>
Mat4<T> Mat4<T>::ortho(T left, T right, T top, T bottom, T zNear, T zFar) {
    return Mat4<T>(
        T(2 / (right - left)), 0,                     0,                   (right + left) / (left - right),
        0,                     T(2 / (top - bottom)), 0,                   (top + bottom) / (bottom - top),
        0,                     0,                     T(2/(zNear - zFar)), (zFar + zNear) / (zNear - zFar),
        0,                     0,                     0,                   1
    );
}

template <typename T>
Mat4<T> Mat4<T>::perspective(T fov, T aspect, T zNear, T zFar) {
    const T zRange = zNear - zFar;
    const T tanHalfFov = tan(toRadian(fov / 2.0));
    Mat4<T> mp(
        T(aspect / tanHalfFov),  T(0),              T(0),                        T(0),
        T(0),                    T(1 / tanHalfFov), T(0),                        T(0),
        T(0),                    T(0),              T((-zNear - zFar) / zRange), T(2 * zNear * zFar / zRange),
        T(0),                    T(0),              T(1),                        T(0)
    );
    return mp;
}

template <typename T>
Mat4<T> Mat4<T>::lookAt(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up) {
    const Vec3<T> n(Vec3<T>::normalize(target - eye));
    const Vec3<T> u(Vec3<T>::normalize(Vec3<T>::cross(up, n)));
    const Vec3<T> v(Vec3<T>::cross(n, u));

    Mat4<T> vm(
        T(u.x),   T(u.y),  T(u.z),  T(0),
        T(v.x),   T(v.y),  T(v.z),  T(0),
        T(n.x),   T(n.y),  T(n.z),  T(0),
        T(0),     T(0),    T(0),    T(1)
    );
    return vm;
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