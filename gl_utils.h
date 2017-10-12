#include "gl.h"

/**
 * 二维向量
 * @member x {GLfloat} 向量的x值
 * @member y {GLfloat} 向量的y值
 * @usage: vec2 v(1.0, 2,0, 3,0);
 */
class Vec2 {
    public:
    GLfloat x, y;
    Vec2():x(0.0), y(0.0) {}
    Vec2(GLfloat xx, GLfloat yy): x(xx), y(yy) {}
};

/**
 * 颜色值
 * @member r {GLfloat} 红色分量
 * @member g {GLfloat} 绿色分量
 * @member b {GLfloat} 蓝色分量
 * @member a {GLfloat} alpha分量
 * @usage: color c;
 */
class Color {
    public:
    GLfloat r, g, b, a;
    Color(): r(0.0), g(0.0), b(0.0), a(1.0){}
    Color(GLfloat rr, GLfloat gg, GLfloat bb, GLfloat aa):r(rr), g(gg), b(bb), a(aa) {}
};

/**
 * 检查并输出OpenGL内部错误
 * @return {GLenum} 错误编码
 */
GLenum errorCheck ();

/**
 * 在屏幕的指定坐标位置绘制像素点
 * @param x {GLint} 像素点的x坐标
 * @param y {GLint} 像素点的y坐标
 * @return {void} 无
 */
void setPixel(GLint x, GLint y);

/**
 * 使用DDA算法在屏幕上绘制直线段
 * @param x0 {GLint} 起始端点的x坐标
 * @param y0 {GLint} 起始端点的y坐标
 * @param xEnd {GLint} 结束端点的x坐标
 * @param yEnd {GLint} 结束端点的y坐标
 * @return {void} 无 
 */
void lineDDA(GLint x0, GLint y0, GLint xEnd, GLint yEnd);

/**
 * 使用Bresenham算法在屏幕上绘制斜率小于1的直线段
 * @param x0 {GLint} 起始端点的x坐标
 * @param y0 {GLint} 起始端点的y坐标
 * @param xEnd {GLint} 结束端点的x坐标
 * @param yEnd {GLint} 结束端点的y坐标
 * @return {void} 无 
 */
void lineBres(GLint x0, GLint y0, GLint xEnd, GLint yEnd);

/**
 * 使用中点画圆算法在屏幕上绘制圆形
 * @param xc {GLint} 圆心的x坐标
 * @param yc {GLint} 圆心的y坐标
 * @param radius {GLint} 半径
 * @return {void} 无 
 */
void circleMidPoint(GLint xc, GLint yc, GLint radius);

/**
 * 使用中点椭圆算法在屏幕上绘制椭圆
 * @param xc {GLint} 椭圆中心的x坐标
 * @param yc {GLint} 椭圆中心的y坐标
 * @param rx {GLint} x轴半径
 * @param ry {GLint} y轴半径
 * @return {void} 无 
 */
void ellipseMidPoint(GLint xc, GLint yc, GLint rx, GLint ry);

