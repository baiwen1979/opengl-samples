#include <cg_utils.h>
#include <vector>
#include <fstream>

#include "cg_glw.hpp"

using namespace cg;
using namespace glw;

/******** 绘图工具函数 ********/
// 绘制多边形
void drawPolygon(vector<Vec2f>& polygon, GLenum mode) {
    glBegin(mode);
    for (vector<Vec2f>::iterator it = polygon.begin(); it != polygon.end(); ++it) {
        glVertex2f(it->x, it->y);
    }
    glEnd();
}

void drawPolygon(Vec2f* vertices, GLint numOfVertices) {
    vector<Vec2f> polygon(vertices, vertices + numOfVertices);
    drawPolygon(polygon, GL_TRIANGLES);
}

/******* 编程实例 3.6 ********/

// 地图的坐标范围
static float xMin = 200, yMin = 200;
static float xMax = 0, yMax = 0;
// 地图内边距
const float PADDING = 0.1f;
// 多边形（县城地图）
struct Polygon {
    vector<Vec2f> vertices;
};
//多边形集合（各县地图）
static vector<Polygon*> mapOfCountries; 
// 加载地图
vector<Polygon*> loadMap(const char* filename) {
    // 每个县城的顶点个数
    int vertexCount;
    // 由多个县城（多边形）构成的整个地图（列表），初始为空
    vector<Polygon*> mapOfCountries;
    // 打开地图文件
    ifstream fs(filename);
    if (!fs.is_open()) {
        // 打开失败
        cout << "Failed to open file:" << filename << endl;
        // 返回空的地图（列表）
        return mapOfCountries;
    }
    // 读取地图文件
    while (!fs.eof()) {
        Polygon *country = new Polygon;
        // 读取当前县城（多边形）的顶点个数
        fs >> vertexCount;
        cout << "Vertex Count:" << vertexCount << endl;
        // 读取当前县城（多边形）的每个顶点的坐标（经纬度）
        for (int i = 0; i < vertexCount; i++) {
            Vec2f p;
            fs >> p.x >> p.y;
            // 统计地图坐标范围
            xMin = min(p.x, xMin);
            yMin = min(p.y, yMin);
            xMax = max(p.x, xMax);
            yMax = max(p.y, yMax);
            // 添加顶点到县城（多边形）的顶点序列
            country->vertices.push_back(p);
        }
        // 添加县城（多边形）到整个地图（列表）
        mapOfCountries.push_back(country);
    }
    // 返回整个地图（列表）
    return mapOfCountries;
}

void renderExample3() {
    cout << "Running Example3 ..." << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_BACK, GL_LINE);
    // 依次绘制整个地图中的每个县城（多边形）
    for (int i = 0; i < mapOfCountries.size(); i++) {
        drawPolygon(mapOfCountries[i]->vertices, GL_LINE_LOOP);
    }

    glutSwapBuffers();

}

void reshapeExample3(int w, int h) {
    cout << "Running reshape of Example3 ..." << endl;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xMin - PADDING, xMax + PADDING, yMin - PADDING, yMax + PADDING);

}

void initExample3() {
    cout << "Initializing OpenGL ..." << endl;
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    const char* filename = "sampleMap.txt";
    mapOfCountries = loadMap(filename);
}


/******** 编程实例 4.4 ********/
/* 绘制多边形 */

Vec2f getPolygonCenter(Vec2f vertices[], GLint numOfVertices) {
    Vec2f sum(0.0f);
    for (GLint i = 0; i < numOfVertices; i++) {
        sum += vertices[i];
    }
    return sum * (1.0f / numOfVertices);
}

void transformVertices(const Mat3f& tm, Vec2f vertices[], GLint numOfVertices) {
    for (GLint i = 0; i < numOfVertices; i++) {
        vertices[i] = tm * vertices[i];
    }
}

void renderExample4() {
    cout << "Testing Transformation" << endl;

    Vec2f vertices[] = {
        Vec2f(50.0f, 25.0f), 
        Vec2f(150.0f, 25.0f),
        Vec2f(100.0f, 100.0f)
    };

    GLint numOfVertices =  sizeof(vertices)/sizeof(Vec2f);

    Vec2f center = getPolygonCenter(vertices,numOfVertices);
    cout << "center = " << center << endl;

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);
    drawPolygon(vertices, numOfVertices);

    Mat3f m;
    m = Mat3f::scale(m, Vec2f(0.5f, 0.5f), center);
    m = Mat3f::rotate(m, 90.0f, center);
    m = Mat3f::translate(m, 0.0f, 100.0f);

    transformVertices(m, vertices, numOfVertices);

    glColor3f(1.0, 0.0, 0.0);
    drawPolygon(vertices, numOfVertices);

    glutSwapBuffers();
}

void initExample4() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void reshapeExample4(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 200.0, 0, 200.0);
}

/******** 编程实例5.5 ********/
void renderExample5() {
    Vec2f vertices[] = {
        Vec2f(-1.0, -0.58),
        Vec2f(1.0, -0.58),
        Vec2f(0.0, 1.15)
    };
    glClear(GL_COLOR_BUFFER_BIT);   
    glLoadIdentity();
    // 在窗口左边(0,0)位置创建400X400大小的视口,并在其中绘制蓝色三角形
    glColor3f(0.0f, 0.0f, 1.0f);
    glViewport(0, 0, 400, 400);
    drawPolygon(vertices, sizeof(vertices)/sizeof(Vec2f));

    glColor3f(1.0f, 0.0f, 0.0f);
    // 在窗口右边(400,0)位置创建400X400大小的视口，并在其中绘制旋转90度的三角形
    glViewport(400, 0, 400, 400);
    glRotatef(90.0, 0.0, 0.0, 1.0);
    drawPolygon(vertices, sizeof(vertices)/sizeof(Vec2f));

    glutSwapBuffers();
}

void initExample5() {
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

/* 测试课程实验 */
void testCGCourseLab() {
    // 测试编程实例3.6
    openGlWindow(renderExample3, "地图绘制", initExample3, reshapeExample3);
    // 测试编程实例4.4
    // openGlWindow(renderExample4, "三角形变换", initExample4, reshapeExample4);
    // 测试编程实例5.5
    // openGlWindow(renderExample5, "红蓝三角形", initExample5);
}