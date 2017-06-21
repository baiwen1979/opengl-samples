#include <stdlib.h>
#include <math.h>
#include "gl_utils.h"

// 绘制像素
void setPixel(GLint x, GLint y) {
    glBegin(GL_POINTS);
      glVertex2f(x, y);
    glEnd();
}

// 四舍五入
inline GLint round(const GLfloat a) {
    return GLint(a + 0.5);
}

// DDA画线算法
void lineDDA(GLint x0, GLint y0, GLint xEnd, GLint yEnd) {
    // 计算两端点之间的水平和垂直距离
    GLint dx = xEnd - x0, dy = yEnd - y0;
    // 步（迭代次）数和迭代变量（循环控制变量）
    GLint steps, k;
    // x和y坐标的增量
    GLfloat xInc, yInc;
    // 从第一个端点开始
    GLfloat x = x0, y = y0;

    // 以水平和垂直距离绝对值的较大值作为各点绘制的步数
    if (fabs(dx) > fabs(dy)) {
        steps = fabs(dx);
    }
    else
    {
        steps = fabs(dy);
    }
    // 计算x坐标和y坐标的迭代增量
    xInc = GLfloat(dx) / GLfloat(steps);
    yInc = GLfloat(dy) / GLfloat(steps);
    // 画出第一个像素点
    setPixel(round(x), round(y));
    // 画出其余各点
    for (k = 0; k < steps; k++) {
        // 计算下一个点的坐标
        x += xInc;
        y += yInc;
        // 画出当前像素点
        setPixel(round(x), round(y));
    }
}

// Bresenham 画线算法，斜率|m| < 1.0
void lineBres (GLint x0, GLint y0, GLint xEnd, GLint yEnd) {
    // 计算两个端点之间的水平和垂直距离的绝对值
    GLint dx = fabs(xEnd - x0), dy = fabs(yEnd - y0);
    // 计算用于决策参数计算的迭代不变量dx, dy, 2dy, 2dy-2dx, 以减少每次迭代的计算量
    GLint doubleDy = 2 * dy, doubleDyMinusDx = 2 * (dy - dx);
    // 计算决策参数的初始值
    GLint p = 2 * dy - dx;

    int x, y; 
    // 较小值端点作为起点,较大者作为终点
    if (x0 > xEnd) {
        x = xEnd;
        y = yEnd;
        xEnd = x0;
        yEnd = y0;
    }
    else {
        x = x0;
        y = y0;
    }
    // 画出第一个像素点
    setPixel(x, y);

    // 画出其余的所有像素点
    while (x < xEnd) {
        x++; // 下一个要画出的像素点的x坐标

        // 若当前决策参数的值小于0,则下一个要画出的像素点的y坐标和当前点一样，即y=y
        if (p < 0) {
            p += doubleDy; //更新决策参数为p=p+2dy
        }
        // 否则，下一个要画出的像素点的y坐标为当前值增一,即y=y+1
        else {
            y++;
            p += doubleDyMinusDx; //更新决策参数为p=2dy-2dx
        }
        // 画出当前像素点
        setPixel(x, y);
    }

}

// 以指定的圆心坐标绘制圆的八个对称点
inline void circlePlotPoints(GLint xc, GLint yc, GLint px, GLint py) {
    setPixel(xc + px, yc + py);
    setPixel(xc - px, yc + py);
    setPixel(xc + px, yc - py);
    setPixel(xc - px, yc - py);
    setPixel(xc + py, yc + px);
    setPixel(xc - py, yc + px);
    setPixel(xc + py, yc - px);
    setPixel(xc - py, yc - px);
}

// 中点画园算法
void circleMidPoint(GLint xc, GLint yc, GLint radius) {
    // 先以圆心在原点的圆进行绘制，从点(0, radius)开始
    GLint x = 0, y = radius;
    // 计算决策参数的初始值
    GLint p = 1 - radius;
    // 绘制所有八分圆的起始点
    circlePlotPoints(xc, yc, x, y);

    // 计算1/8圆弧的其余点的坐标并绘制其所有对称点,直到x>=y
    while (x < y) {
        x++; //下一个要绘制的像素点的x坐标,x=x+1
        // 如果当前决策参数p的值为负(<0),下一个要绘制的像素点的y坐标不变，即y=y
        if (p < 0) {
            p += 2 * x + 1; //并更新决策参数的值为p=p+2(x+1)+1
        }
        // 否则，下一个要绘制的像素点的y坐标为当前值减一，即y=y-1
        else {
            y--;
            p += 2 * (x - y) + 1; //并更新决策参数的值为p=p+2(x+1)-2(y-1)+1
        }
        // 绘制当前像素点及其所有对称点
        circlePlotPoints(xc, yc, x, y);
    }

}