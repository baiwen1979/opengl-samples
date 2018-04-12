// CG数据类型定义文件

#ifndef _CG_TYPES_H_
#define _CG_TYPES_H_

#include <GL/cgl.h>

namespace cg {

// 模板类：RGB颜色
template<typename T>
class Color3 {
public:
    Color3(): r(0), g(0) {}
    Color3(T red, T green, T blue): r(red), g(green), b(blue) {}

    T r, g, b;
};

// 模板类：RGBA颜色
template<typename T>
class Color4 {
public:    
    Color4(): r(0), g(0), b(0), a(1) {}
    Color4(T rr, T gg, T bb, T aa):r(rr), g(gg), b(bb), a(aa) {}

    bool operator == (const Color4<T>& c) const {
        return r == c.r && g == c.g && b == c.b;
    }

    bool operator != (const Color4<T>& c) const {
        return !(*this == c);
    }

    T r, g, b, a;
};

// 模板类：矩形
template<typename T>
class Rect {
public:
    Rect(T ww, T hh): x(0), y(0), w(ww), h(hh) {}
    Rect(T xx, T yy, T ww, T hh): x(xx), y(yy), w(ww), h(hh) {}
    T x, y;
    T w, h;
    T area() const { return w * h; }
};

/**
 * 浮点型RGBA颜色值
 * @member r {GLfloat} 红色分量
 * @member g {GLfloat} 绿色分量
 * @member b {GLfloat} 蓝色分量
 * @usage: Color3f c;
 */
typedef Color3<GLfloat> Color3f;

/**
 * 整型RGBA颜色值
 * @member r {GLubyte} 红色分量
 * @member g {GLubyte} 绿色分量
 * @member b {GLubyte} 蓝色分量
 * @usage: Color3i c;
 */
typedef Color3<GLubyte> Color3i;

/**
 * 浮点型RGBA颜色值
 * @member r {GLfloat} 红色分量
 * @member g {GLfloat} 绿色分量
 * @member b {GLfloat} 蓝色分量
 * @member a {GLfloat} alpha分量
 * @usage: Color4f c;
 */
typedef Color4<GLfloat> Color4f;

/**
 * 整型RGBA颜色值
 * @member r {GLubyte} 红色分量
 * @member g {GLubyte} 绿色分量
 * @member b {GLubyte} 蓝色分量
 * @member a {GLubyte} alpha分量
 * @usage: Color4i c;
 */
typedef Color4<GLubyte> Color4i;

/**
 * 浮点型矩形
 * @member w {GLfloat} 高度
 * @member h {GLfloat} 宽度
 * @usage: Rectf r;
 */
typedef Rect<GLfloat> Rectf;

/**
 * 整型矩形
 * @member w {GLint} 高度
 * @member h {GLint} 宽度
 * @usage: Recti r;
 */
typedef Rect<GLint> Recti;

} //namespace cg

#endif //_CG_TYPES_H_