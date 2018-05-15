#include <cg_utils.h>
#include <cgm/cg_math.h>

#include <vector>
#include <map>
#include <sstream>
#include <cg_model.hpp>
#include <cassert>

#include "cg_glw.hpp"

using namespace cg;

// 窗口设置
static const Recti WIN_RECT = Recti(100, 100, 1024, 768);

// 鼠标设置
static bool firstMouse = true;
static float lastX = WIN_RECT.w / 2.0f;
static float lastY = WIN_RECT.h / 2.0f;

// 模型
static Model * pModelCube = NULL;
static Model * pModelPlane = NULL;

// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pModelShader = NULL; // 模型着色器
static Shader * pVegeShader = NULL; // 植物着色器 
// 纹理
static Texture * pCubeTexture = NULL;  // 纹理用于立方体
static Texture * pFloorTexture = NULL; // 纹理用于地面
static Texture * pVegeTexture = NULL;  // 植物纹理
static Texture * pWindowTexture = NULL; // 窗户纹理
// 植物位置
static Vec3f vegePositions[] = {
    Vec3f(-2.0f, 0.0f, -1.5f),
    Vec3f( 2.0f, 0.0f, 2.0f),
    Vec3f( 0.0f, 0.0f, 1.0f),
    Vec3f( 1.5f, 0.0f, 1.5f),
    Vec3f(-2.5f, 0.0f, 2.5f)
};

// 窗口位置
static Vec3f WindowPositions[] = {
    Vec3f(-2.0f, 0.0f, -1.3f),
    Vec3f( 1.5f, 0.0f, 2.7f),
    Vec3f( 2.0f, 0.0f, 2.2f),
    Vec3f( 0.0f, 0.0f, 1.2f),
    Vec3f( 1.5f, 0.0f, 1.7f)
};

// 排序后的窗口位置
static map<float, Vec3f> sorted;
// 根据观察距离对顶点进行排序
static void sortVerticesByViewDistance(const Vec3f vertices[], int numOfVertices, const Vec3f& viewPos) {
    for (unsigned int i = 0; i < numOfVertices; i++){
            float distance = (viewPos - vertices[i]).length();
            sorted[distance] = WindowPositions[i];
    }
}

/* 渲染回调函数 */
static void renderSceneCB() {
    // 清空颜色、深度和蒙版缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // 旋转角度
    static float angle = 0.0f;
    // 旋转增量
    // angle += 0.1f; 

    // 准备绘制模型
    pModelShader -> use();
    pModelShader -> setMat4f("view", pCamera -> getViewMatrix());
    pModelShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());

    /* 渲染地面 */
    Mat4f model = Mat4f();
    model = Mat4f::scale(model, Vec3f(8.0f));
    model = Mat4f::translate(model, Vec3f(0.0f, -1.0f, 0.0f));
    pModelShader -> setMat4f("model", model);
    pFloorTexture -> apply(GL_TEXTURE0);
    pModelPlane -> render(pModelShader);
    
    // 绘制立方体1
    model = Mat4f::rotateY(Mat4f(), angle);
    model = Mat4f::translate(model, Vec3f(-2.0f, 0.0f, -3.0f));
    pModelShader -> setMat4f("model", model);
    pCubeTexture -> apply(GL_TEXTURE0);
    pModelCube -> render(pModelShader);
    // 绘制立方体2
    model = Mat4f::rotateY(Mat4f(), angle);
    model = Mat4f::translate(model, Vec3f(2.0f, 0.0f, 0.0f));
    pModelShader -> setMat4f("model", model);
    pModelCube -> render(pModelShader);

    // 绘制植物
    pVegeShader -> use();
    pVegeShader -> setMat4f("view", pCamera -> getViewMatrix());
    pVegeShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    pVegeTexture -> apply(GL_TEXTURE0);
    model = Mat4f::scale(Mat4f(), 1.0f);
    model = Mat4f::rotateX(model, -90.0f);
    model = Mat4f::rotateY(model, 180.0f);
    for (unsigned int i = 0; i < 5; i++) {
        Mat4f m = Mat4f::translate(model, vegePositions[i]);
        pVegeShader -> setMat4f("model", m);
        pModelPlane -> render(pVegeShader);
    }

    // 绘制半透明窗口
    model = Mat4f::rotateX(Mat4f(), -90.0f);
    model = Mat4f::rotateY(model, 180.0f);
    sortVerticesByViewDistance(WindowPositions, 5, pCamera -> getPosition());
    pWindowTexture -> apply(GL_TEXTURE0);
    for (map<float, Vec3f>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
        Mat4f m = Mat4f::translate(model, it->second);
        pModelShader -> setMat4f("model", m);
        pModelPlane -> render(pModelShader);
    }

    // 交换前后缓存
    glutSwapBuffers();
}

/* 模型初始化 */
static void initModel() {
    pModelCube = new Model("models/cube.obj");
    pModelPlane= new Model("models/plane.obj");
}

static void initTextures() {
    pCubeTexture = new Texture("textures/container2.png", Texture::Diffuse);
    pFloorTexture = new Texture("textures/test.png", Texture::Diffuse);
    pVegeTexture = new Texture("textures/grass.png", Texture::Diffuse);
    pWindowTexture = new Texture("textures/blending_transparent_window.png", Texture::Diffuse);
}

/* 初始化着色器 */
static void initShaders() {
    pModelShader = new Shader("GLSL/vs28.blending.glsl", "GLSL/fs28.blending.glsl");
    pVegeShader = new Shader("GLSL/vs28.blending.glsl", "GLSL/fs28.vegetation.glsl");
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 1.0f, 6.0f));
}

/* 初始化灯光 */
static void initLight() {
}

/* 初始化材质 */
static void initMaterial() {
    // 模型
    pModelShader -> use();
    pModelShader -> setInt("texture1", 0);
    // 植物
    pVegeShader -> use();
    pVegeShader -> setInt("texture1", 0);
}

// 特殊按键处理函数
static void specialKeyboardCB(int key, int x, int y) {
    switch (key) {    
    case GLUT_KEY_UP:
        pCamera -> processKeyboardInput(Camera::MovementDirection::FORWARD, 0.1f);
        break;
    case GLUT_KEY_DOWN:
        pCamera -> processKeyboardInput(Camera::MovementDirection::BACKWARD, 0.1f);
        break;
    case GLUT_KEY_LEFT:
        pCamera -> processKeyboardInput(Camera::MovementDirection::LEFT, 0.1f);
        break;
    case GLUT_KEY_RIGHT:
        pCamera -> processKeyboardInput(Camera::MovementDirection::RIGHT, 0.1f);
    }
}

// 普通按键处理函数
static void keyboardCB(unsigned char key, int x, int y) {
    switch(key) {
        case 'q':
        case 'Q':
        case 27:
            exit(0);
    }
}

// 鼠标事件处理函数
static void passiveMouseCB(int x, int y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    float xoffset = x - lastX;
    float yoffset = lastY - y;

    lastX = x;
    lastY = y;

    pCamera -> processMouseMovement(xoffset, yoffset);
}

// 注册UI事件处理函数
static void registerUIEvents() {
    glutSpecialFunc(specialKeyboardCB);
    glutPassiveMotionFunc(passiveMouseCB);
    glutKeyboardFunc(keyboardCB);
    // 移动鼠标到指定位置
    glutWarpPointer(lastX, lastY);
}

// 初始化OpenGL状态参数
static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 深度测试函数
    glDepthFunc(GL_LESS);

    // 启用混合
    glEnable(GL_BLEND);
    // 混合函数：src.a * (1 - src.a)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 启用面剔除
    glEnable(GL_CULL_FACE);
    // 指定要剔除的面(背面)
    glCullFace(GL_BACK);
    // 设置前向面（逆时针）
    glFrontFace(GL_CCW);

    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

// 窗口调整回调函数
static void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// 初始化
static void init() {
    initGl();
    initShaders();
    initTextures();
    initModel();
    initCamera();
    initLight();
    initMaterial();
    registerUIEvents();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 28 - Blending and Face Culling", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/