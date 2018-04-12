#include <cg_utils.h>
#include <cgm/cg_math.h>

#include "cg_test.hpp"

using namespace cg;

// 初始窗口大小
const GLsizei WIN_WIDTH = 800;
const GLsizei WIN_HEIGHT = 600;

//初始化2D绘图上下文/
void init2D() {
    // 设置显示窗口的颜色为白色/
    glClearColor(1.0, 1.0, 1.0, 0.0);
    // 设置矩阵模式为投影矩阵/
    glMatrixMode(GL_PROJECTION);
}

// 初始化3D绘图上下文/
void init3D() {
    // 设置显示窗口的颜色为黑色/
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 清除深度缓存/
    glClearDepth(1.0f);
    // 指定深度比较函数为小于等于/
    glDepthFunc(GL_LEQUAL);
    // 启用深度测试/
    glEnable(GL_DEPTH_TEST);  
    // 启用多边形（面片）剔除/ 
    glEnable(GL_CULL_FACE);  
    // 启用规格化/
    glEnable(GL_NORMALIZE); 
    // 设置矩阵模式为投影矩阵/
    glMatrixMode(GL_PROJECTION);
    // 使用正交投影矩阵/
    glOrtho(-8, 8, -5, 5, 1, 10);
    // 设置模型视图矩阵模式/
    glMatrixMode(GL_MODELVIEW);
    // 设置摄像机位置/
    gluLookAt(0,0,10, 0,0,0, 0,1,0);
}

void reshapeViewPort(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
}

// 窗口调整事件回调函数，2D模式/
void onReshape2D(int w, int h) {
    reshapeViewPort(w, h);
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void renderWireTeapot() {
    glShadeModel(GL_LIGHT0);
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3f(1.0, 1.0, 0);
    glutWireTeapot(3);
    //glutSolidTeapot(4);
    glutSwapBuffers();
}

void renderLineSegment() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glColor3f(1.0, 0.0, 0.0); //红色线段
    glBegin (GL_LINES); //开始绘制线段
      glVertex2i(WIN_WIDTH - 10, WIN_HEIGHT-10); //顶点1
      glVertex2i(10, 10); //顶点2
    glEnd(); //结束绘制
    glutSwapBuffers(); //交换前后缓存
    errorCheck();
}

void renderPoints() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    glPointSize(10.0);

    glBegin(GL_POINTS);
      glVertex2i(WIN_WIDTH / 2, WIN_HEIGHT - 100);
      glVertex2i(100, 100);
      glVertex2i(WIN_WIDTH - 100, 100);
    glEnd();

    glutSwapBuffers(); //交换前后缓存
}

void renderPointsV() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    glPointSize(20);

    int point1[] = {WIN_WIDTH / 2, WIN_HEIGHT - 20};
    int point2[] = {20, 20};
    int point3[] = {WIN_WIDTH - 20, 20};

    glBegin(GL_POINTS);
      glVertex2iv(point1);
      glVertex2iv(point2);
      glVertex2iv(point3);
    glEnd();

    glutSwapBuffers(); //交换前后缓存
}

void renderTriangle() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    int p1[] = {WIN_WIDTH / 2 , WIN_HEIGHT - 20};
    int p2[] = {20, 20};
    int p3[] = {WIN_WIDTH - 20, 20};


    /* GL_LINES: 连接每一对相邻顶点
    glBegin(GL_LINES);
      glVertex2iv(p1);
      glVertex2iv(p2);

      glVertex2iv(p1);
      glVertex2iv(p3);
      
      glVertex2iv(p2);
      glVertex2iv(p3);
    glEnd();
    */

    /* GL_LINE_STRIP: 生成折线，从第一个端点到最后一个端点依次首尾相连  
    glBegin(GL_LINE_STRIP);
      glVertex2iv(p1);
      glVertex2iv(p2);
      glVertex2iv(p3);
      glVertex2iv(p4);
    glEnd();
    */

    /* GL_LINE_LOOP: 生成封闭折线，自动连接最后和第一个端点 */  
    glBegin(GL_LINE_LOOP);
      glVertex2iv(p1);
      glVertex2iv(p2);
      glVertex2iv(p3);
    glEnd();
    
    glutSwapBuffers(); //交换前后缓存;
}

void renderLineDDA() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口

    glPointSize(10);

    glColor3f(0.0, 0.0, 1.0); //蓝色线段
    lineDDA(WIN_WIDTH - 20, 20, 20, WIN_HEIGHT - 20);
    glColor3f(1.0, 0.0, 0.0); //红色线段
    lineDDA(20, 20, WIN_WIDTH - 20, WIN_HEIGHT - 20);
    glutSwapBuffers(); //交换前后缓存
}

void renderLineBres() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glPointSize(4);
    glColor3f(0.0, 0.0, 0.0); //黑色线段
    lineBres(20, 20, WIN_WIDTH - 20, WIN_HEIGHT - 20);
    glutSwapBuffers(); //交换前后缓存
}

void renderLinePPC() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glPointSize(4);
    glColor3f(0.0, 0.0, 1.0); //蓝色线段
    linePPC(WIN_WIDTH - 20, 20, 20, WIN_HEIGHT - 20);
    glColor3f(0.0, 1.0, 0.0); //绿色线段
    lineBres(20, 20, WIN_WIDTH - 20, WIN_HEIGHT - 20);
    glutSwapBuffers(); //交换前后缓存
}

void renderCircleMidPoint() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glPointSize(2);
    glColor3f(1.0, 0.0, 1.0); //品红色线条
    circleMidPoint(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_HEIGHT / 2 - 10);
    glutSwapBuffers(); //交换前后缓存
}

void renderCircleBres() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glPointSize(4);
    glColor3f(0.0, 1.0, 0.0); //绿线条
    circleBres(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_HEIGHT / 2 - 10);
    glutSwapBuffers(); //交换前后缓存
}

void renderEllipseMidPoint() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glPointSize(8);
    glColor3f(0.5, 0.0, 0.0); //深红色线条
    ellipseMidPoint(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH / 4 , WIN_HEIGHT / 4);
    glutSwapBuffers(); //交换前后缓存
}

void renderArcAngDiscrete() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glLineWidth(2);
    glColor3f(0.0, 0.0, 0.0); //黑线条
    arcAngDiscrete(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_HEIGHT / 2 - 10);
    glutSwapBuffers(); //交换前后缓存
}

void renderRegPolygon() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(4);
    Vec2i c(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    Color4f color(1.0, 0.0, 1.0, 1.0);
    regPolygon(c, WIN_HEIGHT / 2, 12, color);
    glutSwapBuffers(); //交换前后缓存
}

void renderColoredTriangle() {
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    //填充模式(默认)
    glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 1.0);
        glVertex2i(50, 50);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2i(WIN_WIDTH / 2, WIN_HEIGHT - 50);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2i(WIN_WIDTH - 50, 50);
    glEnd();
    //线框模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(3.0);
    glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 0.0);
        //第一个顶点
        glVertex2i(WIN_WIDTH / 2, WIN_HEIGHT - 50);       
        //glColor3f(1.0, 0.0, 0.0);
        //消除第二条边
        glEdgeFlag(GL_FALSE);
        //第二个顶点 
        glVertex2i(50, 50);
        //glColor3f(0.0, 1.0, 0.0);
        //显示第三条边
        glEdgeFlag(GL_TRUE);
        //第三个顶点
        glVertex2i(WIN_WIDTH - 50, 50);
    glEnd();

    glutSwapBuffers(); //交换前后缓存
}

void testMath3d() {
    Vec3f v(0, 1, 2); 
    std::cerr << v << std::endl; 
    Mat4f a, b, c; 
    cout << a << endl;
    c = a * b; 
 
    Mat4f d(
        0.707107,  0,        -0.707107, 0, 
        -0.331295, 0.883452, -0.331295, 0, 
        0.624695,  0.468521, 0.624695,  0, 
        4.000574,  3.00043,  4.000574,  1
    );

    std::cerr << d << std::endl; 
    d.invert(); 
    std::cerr << d << std::endl; 
}

void testFloodFill() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glPointSize(2);
    glColor3f(1.0, 0.0, 0.0); //红线条
    // 先绘制要填充的三角形
    int p1[] = {WIN_WIDTH / 2 , WIN_HEIGHT - 20};
    int p2[] = {20, 20};
    int p3[] = {WIN_WIDTH - 20, 20};
    glBegin(GL_LINE_LOOP);
      glVertex2iv(p1);
      glVertex2iv(p2);
      glVertex2iv(p3);
    glEnd();
    // 目标颜色
    Color4i tColor(255, 255, 255, 255);
    // 填充颜色
    Color4i fColor(255, 0, 0, 255);
    // 种子填充
    floodFill(WIN_WIDTH / 2, WIN_HEIGHT / 2, tColor, fColor);
    glutSwapBuffers();
}

void testPrimitive2d() {
    openGlWindow(testFloodFill, "Primitive 2D", init2D, onReshape2D);
}