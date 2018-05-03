#ifndef _CG_MAT3_HPP_
#define _CG_MAT3_HPP_

#include "cg_vec2.hpp"
#include "cg_mat4.hpp"

namespace cg {

template <typename T>
class Mat3 {
public:
    /*** 构造函数，创建3X3矩阵 ***/
    // 默认构造函数：创建单位矩阵
    Mat3();
    // 创建以指定参数V为对角元素值的对角矩阵
    Mat3(const T& v);
    // 创建指定参数为元素的矩阵
    Mat3(const T& a, const T& b, const T& c,
         const T& d, const T& e, const T& f,
         const T& g, const T& h, const T& i);
    // 拷贝构造函数
    Mat3(const Mat3<T>& m);
    Mat3(const Mat4<T>& m);
    
    /*** 成员运算符 ***/
    // 矩阵 +/- 矩阵， 如：Mat3 a,b; Mat3 c = a + b; Mat3 d = a - b;
    Mat3<T> operator + (const Mat3<T>& b) const;
    Mat3<T> operator - (const Mat3<T>& b) const;
    // 矩阵 * 矩阵，如：Mat3 a,b; Mat3 c = a * b;
    Mat3<T> operator * (const Mat3<T>& b) const;
    // 矩阵 * 数值，如：Mat3 a, float b; Mat3 c = a * b;
    Mat3<T> operator * (const T& b) const;
    // 矩阵 * 向量，如：Mat3 t, Vec2 p(0,0); Vec2 p1 = t * p;
    Vec2<T> operator * (const Vec2<T>& v) const;
    // 下标取值，如：Mat3 m, int a = m[0][0]
    const T* operator [] (uint8_t i) const;
    // 下标设值, 如：Mat3 m, m[0][0] = 2
    T* operator [] (uint8_t i);

    /*** 成员函数 ***/
    // 将当前矩阵转置
    Mat3<T>& transpose();
    // 返回当前矩阵的转置
    Mat3<T> transposed() const;  
    // 创建平移矩阵
    static Mat3<T> translate(const Mat3<T>& m, const T& tx, const T& ty);
    static Mat3<T> translate(const Mat3<T>& m, const Vec2<T>& tv);
    // 创建旋转矩阵
    static Mat3<T> rotate(const Mat3<T>& m, const T& angle);
    static Mat3<T> rotate(const Mat3<T>& m, const T& angle, const T& px, const T& py);
    static Mat3<T> rotate(const Mat3<T>& m, const T& angle, const Vec2<T>& pv);
    // 创建缩放矩阵
    static Mat3<T> scale(const Mat3<T>& m, const T& sx, const T& sy);
    static Mat3<T> scale(const Mat3<T>& m, const Vec2<T>& sv);
    static Mat3<T> scale(const Mat3<T>& m, const T& sx, const T& sy, const T& px, const T& py);
    static Mat3<T> scale(const Mat3<T>& m, const Vec2<T>& sv, const Vec2<T>& pv);

    /*** 成员变量 ***/
    T m[3][3];
}; 

}

#include "cg_mat3.inl"

#endif //_CG_MAT3_HPP_