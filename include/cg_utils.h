#include <cg_types.h>
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


/**
 * 绘制正多边形
 * @param center {Vec2i} 中心点（坐标）
 * @param radius {GLint} 半径（顶点距离中心点的距离）
 * @param numEdges {GLint} 边数（默认6条边）
 * @param fillColor {Color4f} 填充颜色（默认为红色）
 * @return {void} 无
 */
void regPolygon(Vec2i center, GLint radius, GLint numEdges = 6, Color4f fillColor = Color4f(1.0, 0.0, 0.0, 1.0));


