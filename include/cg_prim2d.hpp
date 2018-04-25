#ifndef _CG_PRIM2D_HPP_
#define _CG_PRIM2D_HPP_

#include <cg_types.h>
#include <cgm/cg_math.h>

namespace cg {

const float PI = 3.14159265358;

/**
 * 在屏幕的指定坐标位置绘制像素点
 * @param x {GLint} 像素点的x坐标
 * @param y {GLint} 像素点的y坐标
 * @return {void} 无
 */
void setPixel(GLint x, GLint y);

/*
 * 在屏幕的指定坐标位置绘制指定颜色的像素
 * @param x {GLint} 像素点的x坐标
 * @param y {GLint} 像素点的y坐标
 * @param color {Color4i} 像素点的颜色
 * @return {void} 无
*/
void setPixel(GLint x, GLint y, Color4i color);

/**
 * 获取屏幕的指定坐标位置的像素颜色
 * @param x {GLint} 像素点的x坐标
 * @param y {GLint} 像素点的y坐标
 * @return {Color4i} RGBA颜色
 */
Color4i getPixel(GLint x, GLint y);

/**
 * 使用DDA算法在屏幕上绘制直线段
 * @param x0 {GLint} 起始端点的x坐标
 * @param y0 {GLint} 起始端点的y坐标
 * @param x1 {GLint} 结束端点的x坐标
 * @param y1 {GLint} 结束端点的y坐标
 * @return {void} 无 
 */
void lineDDA(GLint x0, GLint y0, GLint x1, GLint y1);

/**
 * 使用PPC（逐点比较）算法在屏幕上绘制直线段    
 * @param x0 {GLint} 起始端点的x坐标
 * @param y0 {GLint} 起始端点的y坐标
 * @param x1 {GLint} 结束端点的x坐标
 * @param y1 {GLint} 结束端点的y坐标
 * @return {void} 无 
 */
void linePPC(GLint x0, GLint y0, GLint x1, GLint y1);

/**
 * 使用Bresenham算法在屏幕上绘制斜率小于1的直线段
 * @param x0 {GLint} 起始端点的x坐标
 * @param y0 {GLint} 起始端点的y坐标
 * @param x1 {GLint} 结束端点的x坐标
 * @param y1 {GLint} 结束端点的y坐标
 * @return {void} 无 
 */
void lineBres(GLint x0, GLint y0, GLint x1, GLint y1);

/**
 * 使用Bresenham算法在屏幕上绘制圆
 * @param xc {GLint} 圆心的x坐标
 * @param yc {GLint} 圆心的y坐标
 * @param radius {GLint} 半径
 * @return {void} 无 
 */ 
void circleBres(GLint xc, GLint yc, GLint radius);

/**
 * 使用中点画圆算法在屏幕上绘制圆形
 * @param xc {GLint} 圆心的x坐标
 * @param yc {GLint} 圆心的y坐标
 * @param radius {GLint} 半径
 * @return {void} 无 
 */
void circleMidPoint(GLint xc, GLint yc, GLint radius);

/**
 * 使用角度离散法绘制圆弧
 * @param xc {GLint} 圆弧所在圆心的x坐标
 * @param yc {GLint} 圆弧所在圆心的y坐标
 * @param radius {GLint} 半径
 * @param radStart {GLfloat} 起始弧度
 * @param radEnd {GLfloat} 结束弧度
 * @return {void} 无
 */
void arcAngDiscrete(GLint xc, GLint yc, GLint radius, GLfloat radStart = 0, GLfloat radEnd = 2 * PI);

/**
 * 使用中点椭圆算法在屏幕上绘制椭圆
 * @param xc {GLint} 椭圆中心的x坐标
 * @param yc {GLint} 椭圆中心的y坐标
 * @param rx {GLint} x轴半径
 * @param ry {GLint} y轴半径
 * @return {void} 无 
 */
void ellipseMidPoint(GLint xc, GLint yc, GLint rx, GLint ry);

/**
 * 绘制正多边形
 * @param center {Vec2i} 中心点（坐标）
 * @param radius {GLint} 半径（顶点距离中心点的距离）
 * @param numEdges {GLint} 边数（默认6条边）
 * @param fillColor {Color4f} 填充颜色（默认为红色）
 * @return {void} 无
 */
void regPolygon(Vec2i center, GLint radius, GLint numEdges = 6, 
    Color4f fillColor = Color4f(1.0, 0.0, 0.0, 1.0));

/**
 * 种子区域填充算法（漫水法）
 * @param x {GLint} 种子像素点的x坐标
 * @param y {GLint} 种子像素点的y坐标
 * @param targetColor {Color4i} 目标颜色
 * @param fillColor {Color4i} 填充颜色
 */
void floodFill(GLint x, GLint y, const Color4i& targetColor, const Color4i& fillColor);

/**
 * 二维顶点(x,y)区域编码算法
 * @param x {GLfloat} 顶点的x坐标
 * @param y {GLfloat} 顶点的y坐标
 * @param xl {GLfloat} 区域的左边界
 * @param xr {GLfloat} 区域的右边界
 * @param yb {GLfloat} 区域的下边界
 * @param yt {GLfloat} 区域的上边界
 * @return {GLubyte} 区域编码
 */
GLubyte encodeVertex2(GLfloat x, GLfloat y, GLfloat xl, GLfloat xr, GLfloat yb, GLfloat yt);

/** 
 * 三维顶点(x, y, z)的区域编码算法
 * @param x {GLfloat} 顶点的x坐标
 * @param y {GLfloat} 顶点的y坐标
 * @param z {GLfloat} 顶点的z坐标
 * @param xl {GLfloat} 区域的左边界
 * @param xr {GLfloat} 区域的右边界
 * @param yb {GLfloat} 区域的下边界
 * @param yt {GLfloat} 区域的上边界
 * @param zf {GLfloat} 区域的前边界
 * @param zb {GLfloat} 区域的后边界
 * @return {GLubyte} 区域编码
 */
GLubyte encodeVertex3(GLfloat x, GLfloat y, GLfloat z, GLfloat xl, GLfloat xr, GLfloat yb, GLfloat yt, GLfloat zf, GLfloat zb);

/**
 * Cohen-Sutherland编码线段裁剪算法
 * @param x0 {GLfloat&} 线段起始端点的x坐标，将被替换为裁剪后起始端点的x坐标
 * @param y0 {GLfloat&} 线段起始端点的y坐标，将被替换为裁剪后起始端点的y坐标
 * @param x1 {GLfloat&} 线段结束端点的x坐标，将被替换为裁剪后结束端点的x坐标
 * @param y1 {GLfloat&} 线段结束端点的y坐标，将被替换为裁剪后结束端点的y坐标
 * @param xl {GLfloat} 裁剪区域的左边界
 * @param xr {GLfloat} 裁剪区域的右边界
 * @param yb {GLfloat} 裁剪区域的下边界
 * @param yt {GLfloat} 裁剪区域的上边界
 * @return {bool} 取/弃
 */
bool lineClipCB(GLfloat& x0, GLfloat& y0, GLfloat& x1, GLfloat& y1, GLfloat xl, GLfloat xr, GLfloat yb, GLfloat yt);

/**
 * Liang-Barsky参数化线段裁剪算法
 * @param x0 {GLfloat&} 线段起始端点的x坐标，将被替换为裁剪后起始端点的x坐标
 * @param y0 {GLfloat&} 线段起始端点的y坐标，将被替换为裁剪后起始端点的y坐标
 * @param x1 {GLfloat&} 线段结束端点的x坐标，将被替换为裁剪后结束端点的x坐标
 * @param y1 {GLfloat&} 线段结束端点的y坐标，将被替换为裁剪后结束端点的y坐标
 * @param xl {GLfloat} 裁剪区域的左边界
 * @param xr {GLfloat} 裁剪区域的右边界
 * @param yb {GLfloat} 裁剪区域的下边界
 * @param yt {GLfloat} 裁剪区域的上边界
 * @return {bool} 取/弃
 */
bool lineClipLB(GLfloat& x0, GLfloat& y0, GLfloat& x1, GLfloat& y1, GLfloat xl, GLfloat xr, GLfloat yb, GLfloat yt);

} //namespace cg

#endif //_CG_PRIM2D_HPP_

