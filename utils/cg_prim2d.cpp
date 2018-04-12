#include <cg_utils.h>

namespace cg {

// 绘制像素
void setPixel(GLint x, GLint y) {
    glBegin(GL_POINTS);
      glVertex2f(x, y);
    glEnd();
}

void setPixel(GLint x, GLint y, Color4i color) {
    glColor4ubv((GLubyte*)&color);
    setPixel(x, y);
}

// 获取像素颜色
Color4i getPixel(GLint x, GLint y) {
    Color4i color;
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color.r);
    return color;
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

// 逐点比较法画线算法
void linePPC(GLint x0, GLint y0, GLint xEnd, GLint yEnd) {
    // 平移后的画笔坐标（从原点开始）
    GLint x = 0, y = 0;
    // 平移后的终点坐标
    GLint xA, yA;
    // 画线时的起点坐标
    GLint xS, yS;
    // 平移坐标，使y值较小的点位于坐标原点
    if (y0 > yEnd) {
        yA = y0 - yEnd;
        xA = x0 - xEnd;

        xS = xEnd;
        yS = yEnd;
    }
    else {
        yA = yEnd - y0;
        xA = xEnd - x0;

        xS = x0;
        yS = y0;
    }
    
    GLint F = 0;

    int n = abs(xA) + abs(yA);

    if (xA > 0) { // 斜率为正
        for (int i = 0; i < n; i++) {
            if (F < 0) {
                y++;
                F += xA;
            }
            else {
                x++;
                F -= yA;
            }
            setPixel(xS + x, yS + y);
        }
    }
    else { // 斜率为负
        for (int i = 0; i < n; i++) {
            if (F < 0) {
                x--;
                F += yA;
            }
            else {
                y++;
                F += xA;
            }
            setPixel(xS + x, yS + y);
        }
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

void circleBres(GLint xc, GLint yc, GLint radius) {
    // （递推公式的）初始像素为(0, radius)
    int x = 0, y = radius;
    // 决策参数（判别式）的初始值: d = 3 - 2r;
    GLint d = 3 - 2 * radius;
    // 先绘制八分圆的初始像素
    circlePlotPoints(xc, yc, x, y);
    while (x < y) {
        if (d < 0) {
            d += 4 * x + 6;
        }
        else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
        // 绘制当前像素及所有对称点
        circlePlotPoints(xc, yc, x, y);
    }
}

// 中点画圆算法
void circleMidPoint(GLint xc, GLint yc, GLint radius) {
    // 先以圆心在原点的圆进行绘制，从点(0, radius)开始
    GLint x = 0, y = radius;
    // 计算决策参数的初始值
    GLint d = 1 - radius;
    // 绘制所有八分圆的起始点
    circlePlotPoints(xc, yc, x, y);

    // 计算1/8圆弧的其余点的坐标并绘制其所有对称点,直到x>=y
    while (x < y) {
        // 如果当前决策参数p的值为负(<0)
        if (d < 0) {
            d += 2 * x + 3; // 并更新决策参数的值为d=d+2x+3
            //下一个要绘制的像素点的y坐标不变，即y=y
        }
        // 否则
        else {
            d += 2 * (x - y) + 5; //并更新决策参数的值为d=d+2x-2y+5
            y--; // 下一个要绘制的像素点的y坐标为当前值减一，即y=y-1
        }
        x++; //下一个要绘制的像素点的x坐标,x=x+1
        // 绘制当前像素点及其所有对称点
        circlePlotPoints(xc, yc, x, y);
    }
}

// 使用角度离散法绘制圆弧
void arcAngDiscrete(GLint xc, GLint yc, GLint radius, GLfloat radStart, GLfloat radEnd) {
    // 如果终止角比起始角还小，则终止角加2*PI
    if (radEnd < radStart) {
        radEnd += 2 * PI;
    }
    GLfloat dt = 0.4 / radius; // 角度离散值，及步长
    GLint n = (GLint)((radEnd - radStart) / dt + 0.5);
    GLfloat rad = radStart;
    GLint x = xc + (GLint) (radius * cos(radStart));
    GLint y = yc + (GLint) (radius * sin(radStart));
    // 开始绘制短直线
    if (abs(radEnd - radStart) < 2 * PI) {
        glBegin(GL_LINE_STRIP);
    }
    else {
        glBegin(GL_LINE_LOOP);
    }    
    glVertex2f(x, y);
    for (GLint i = 0; i <= n; i++) {
        rad += dt;
        GLfloat cos_rad = cos(rad);
        GLfloat sin_rad = sin(rad);
        x = (GLint) (xc + radius * cos_rad);
        y = (GLint) (yc + radius * sin_rad);
        glVertex2f(x, y);
    }
    glEnd();
}

// 以指定中心点坐标绘制所有1/4椭圆弧上点(x,y)及其对称点
inline void ellipsePlotPoints(GLint xc, GLint yc, GLint x, GLint y) {
    setPixel(xc + x, yc + y);
    setPixel(xc - x, yc + y);
    setPixel(xc + x, yc - y);
    setPixel(xc - x, yc - y);
}

// 中点椭圆绘制算法
void ellipseMidPoint(GLint xc, GLint yc, GLint rx, GLint ry) {
    //为了减少计算量，预先计算的迭代不变量
    GLint rx2 = rx * rx;  // rx^2
    GLint ry2 = ry * ry;  // ry^2
    GLint dblRx2 = 2 * rx2; // 2rx^2
    GLint dblRy2 = 2 * ry2; // 2ry^2
    GLint p; //决策参数
    // (x0,y0) = (0, ry)
    GLint x = 0; 
    GLint y = ry;
    //用于增量计算，对于起始位置(0,ry),px=2(ry^2)*x=0, py=2(rx^2)*y=2(rx^2)*ry
    GLint px = 0; 
    GLint py = dblRx2 * y;
    //绘制每1/4椭圆的第一个点
    ellipsePlotPoints(xc, yc, x, y);

    //区域1
    p = round (ry2 - (rx2 * ry) + (0.25 * rx2));
    while (px < py) {
        x++;
        px += dblRy2;
        if (p < 0) {
            p += ry2 + px;
        }
        else {
            y--;
            py -= dblRx2;
            p += ry2 + px - py;
        }
        ellipsePlotPoints(xc, yc, x, y);
    }

    //区域2
    p = round(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
    while (y > 0) {
        y--;
        py -= dblRx2;
        if (p > 0) {
            p += rx2 - py;
        }
        else {
            x++;
            px += dblRy2;
            p += rx2 - py + px;
        }
        ellipsePlotPoints(xc, yc, x, y);
    }
}

// 辅助函数：生成正多边形显示列表
GLuint genRegPolyList(Vec2i center, GLint radius, GLint numEdges, Color4f fillColor) {
    const GLdouble PI = 3.1415926;
    GLdouble theta = 0.0;
    GLuint listId = glGenLists(1); //获取可用的显示列表的ID
    GLint x, y; //顶点坐标
    glNewList(listId, GL_COMPILE);
        glColor3f(fillColor.r, fillColor.g, fillColor.b); //设置填充颜色
        glBegin(GL_POLYGON);
            for (int i = 0; i < numEdges; i++) {
                theta = 2 * PI * i / numEdges;
                x = center.x + radius * cos(theta);
                y = center.y + radius * sin(theta);
                glVertex2i(x, y);
            }
        glEnd();
    glEndList();
    return listId;
}

// 绘制正多边形
void regPolygon(Vec2i center, GLint radius, GLint numEdges, Color4f fillColor) {
    GLuint listId = genRegPolyList(center, radius, numEdges, fillColor);
    glCallList(listId);
}

// 种子填充算法（漫水法）
void floodFill(GLint x, GLint y, const Color4i& targetColor, const Color4i& fillColor) {
    Color4i color = getPixel(x, y);

    if (fillColor == targetColor) return;
    if (color != targetColor) return;

    setPixel(x, y, fillColor);

    floodFill(x, y + 1, targetColor, fillColor);     // 北
    //floodFill(x + 1, y + 1, targetColor, fillColor); // 东北
    floodFill(x + 1, y, targetColor, fillColor);     // 东
    //floodFill(x + 1, y - 1, targetColor, fillColor); // 东南
    floodFill(x, y - 1, targetColor, fillColor);     // 南
    //floodFill(x - 1, y - 1, targetColor, fillColor); // 西南
    floodFill(x - 1, y, targetColor, fillColor);     // 西   
    //floodFill(x - 1, y + 1, targetColor, fillColor); // 西北         
}

} //namespace cg
