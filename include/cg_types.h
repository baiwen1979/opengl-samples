// CG数据类型定义文件

#ifndef _CG_TYPES_H_
#define _CG_TYPES_H_

#include <GL/cgl.h>
#include <cmath>
// 模板类：二维向量
namespace cg {

template<typename T>
class Vec2 {
public:
    Vec2(): x(0), y(0) {}
    Vec2(T xx, T yy): x(xx), y(yy) {}
    T x, y;
};

// 模板类：RGBA颜色
template<typename T>
class Color4 {
public:    
    Color4(): r(0), g(0), b(0), a(1) {}
    Color4(T rr, T gg, T bb, T aa):r(rr), g(gg), b(bb), a(aa) {}
    T r, g, b, a;
};

// 模板类：矩形
template<typename T>
class Rect {
public:
    Rect(T w, T h): x(0), y(0), w(w), h(h) {}
    Rect(T xx, T yy, T ww, T hh): x(xx), y(yy), w(ww), h(hh) {}
    T x, y;
    T w, h;
    T area() const { return w * h; }
};
/**
 * 整型二维向量
 * @member x {GLint} 向量的x值
 * @member y {GLint} 向量的y值
 * @usage: vec2i v(1, 2, 3);
 */
typedef Vec2<GLint> Vec2i;

/**
 * 浮点型二维向量
 * @member x {GLfloat} 向量的x值
 * @member y {GLfloat} 向量的y值
 * @usage: vec2 v(1.0, 2.0, 3.0);
 */
typedef Vec2<GLfloat> Vec2f;

/**
 * 浮点型颜色值
 * @member r {GLfloat} 红色分量
 * @member g {GLfloat} 绿色分量
 * @member b {GLfloat} 蓝色分量
 * @member a {GLfloat} alpha分量
 * @usage: Color4f c;
 */
typedef Color4<GLfloat> Color4f;

/**
 * 整型颜色值
 * @member r {GLubyte} 红色分量
 * @member g {GLubyte} 绿色分量
 * @member b {GLubyte} 蓝色分量
 * @member a {GLubyte} alpha分量
 * @usage: Color4i c;
 */
typedef Color4<GLubyte> Color4i;

/**
 * 浮点型矩形
 * @member width {GLfloat} 高度
 * @member height {GLfloat} 宽度
 * @usage: Rectf r;
 */
typedef Rect<GLfloat> Rectf;

/**
 * 整型矩形
 * @member r {GLint} 高度
 * @member g {GLint} 宽度
 * @usage: Recti r;
 */
typedef Rect<GLint> Recti;

} //namespace cg

#endif //_CG_TYPES_H_