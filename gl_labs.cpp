#include "gl.h"

#include "gl_utils.h"
#include "gl_labs.h"

void init2D() {
    //设置显示窗口的颜色为白色
    glClearColor(1.0, 1.0, 1.0, 0.0);
    //设置矩阵模式为投影矩阵
    glMatrixMode(GL_PROJECTION);
    //使用glu 2D正交投影
    gluOrtho2D(0.0, 300.0, 0.0, 200.0);
}

void init3D() {
    //设置显示窗口的颜色为黑色
    glClearColor(0.1, 0.1, 0.1, 0.0);
    //设置矩阵模式为投影矩阵
    glMatrixMode(GL_PROJECTION);
    //使用正交投影矩阵
    glOrtho(-5,5,-5, 5,5,15);
    //设置模型视图矩阵模式
    glMatrixMode(GL_MODELVIEW);
    //设置摄像机位置
    gluLookAt(0,0,10, 0,0,0, 0,1,0);
}

void drawWireTeapot() {
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3f(1.0,1.0,0);
    glutWireTeapot(3);
    glFlush();
}

void drawLineSegment() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glColor3f(1.0, 0.0, 0.0); //红色线段
    glBegin (GL_LINES); //开始绘制线段
      glVertex2i(280, 20); //顶点1
      glVertex2i(20, 180); //顶点2
    glEnd(); //结束绘制
    glFlush(); //强制清空所有缓存，并处理OpenGL函数
    errorCheck();
}

void drawPoints() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_POINTS);
      glVertex2i(280, 20);
      glVertex2i(20, 20);
      glVertex2i(150, 180);
    glEnd();

    glFlush();
}

void drawPointsV() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    int point1[] = {280, 20};
    int point2[] = {20, 20};
    int point3[] = {150, 180};

    glBegin(GL_POINTS);
      glVertex2iv(point1);
      glVertex2iv(point2);
      glVertex2iv(point3);
    glEnd();

    glFlush();
}

void drawTriangle() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    int p1[] = {20, 20};
    int p2[] = {280, 20};
    int p3[] = {150, 180};


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
    
    glFlush();
}

void drawLineDDA() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glColor3f(0.0, 0.0, 1.0); //蓝色线段
    lineDDA(280, 20, 20, 180);
    glColor3f(0.0, 1.0, 0.0); //绿色线段
    lineDDA(20, 20, 280, 180);
    glFlush();
}

void drawLineBres() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glColor3f(0.0, 0.0, 0.0); //黑色线段
    lineBres(20, 20, 280, 180);
    glFlush();
}

void drawCircleMidPoint() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glColor3f(1.0, 0.0, 1.0); //品红色线条
    circleMidPoint(150, 100, 98);
    glFlush();
}

void drawEllipseMidPoint() {
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glColor3f(0.5, 0.0, 0.0); //深红色线条
    ellipseMidPoint(150, 100, 120, 80);
    glFlush();
}

void gl_lab() {
    int argc = 0;
    char** argv = NULL;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(600, 400);
    glutCreateWindow("OpenGL #D View");
    init2D();
    glutDisplayFunc(drawEllipseMidPoint);
    glutMainLoop();
}