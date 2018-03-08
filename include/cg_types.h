#include <GL/cgl.h>
// CG数据类型
/**
 * 整型二维向量
 * @member x {GLint} 向量的x值
 * @member y {GLint} 向量的y值
 * @usage: vec2i v(1, 2, 3);
 */
class Vec2i {
public:
    GLint x, y;
    Vec2i():x(0), y(0) {}
    Vec2i(GLint xx, GLint yy): x(xx), y(yy) {}
};

/**
 * 浮点型二维向量
 * @member x {GLfloat} 向量的x值
 * @member y {GLfloat} 向量的y值
 * @usage: vec2 v(1.0, 2.0, 3.0);
 */
class Vec2f {
    public:
    GLfloat x, y;
    Vec2f():x(0.0), y(0.0) {}
    Vec2f(GLfloat xx, GLfloat yy): x(xx), y(yy) {}
};

/**
 * 浮点型颜色值
 * @member r {GLfloat} 红色分量
 * @member g {GLfloat} 绿色分量
 * @member b {GLfloat} 蓝色分量
 * @member a {GLfloat} alpha分量
 * @usage: Color4f c;
 */
class Color4f {
    public:
    GLfloat r, g, b, a;
    Color4f(): r(0.0), g(0.0), b(0.0), a(1.0){}
    Color4f(GLfloat rr, GLfloat gg, GLfloat bb, GLfloat aa):r(rr), g(gg), b(bb), a(aa) {}
};