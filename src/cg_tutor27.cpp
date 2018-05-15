#include <cg_utils.h>
#include <cgm/cg_math.h>

#include <vector>
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
static Model * pModelFloor = NULL;

// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pModelShader = NULL; // 模型着色器
static Shader * pOutlineShader = NULL; // 轮廓着色器
// 纹理
static Texture * pCubeTexture = NULL;  // 纹理用于立方体
static Texture * pFloorTexture = NULL; // 纹理用于地面

/* 渲染回调函数 */
static void renderSceneCB() {
    // 清空颜色、深度和蒙版缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // 旋转角度
    static float angle = 0.0f;
    // 旋转增量
    angle += 0.1f; 

    // 准备绘制轮廓
    pOutlineShader -> use();
    pOutlineShader -> setMat4f("view", pCamera -> getViewMatrix());
    pOutlineShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    // 准备绘制模型
    pModelShader -> use();
    pModelShader -> setMat4f("view", pCamera -> getViewMatrix());
    pModelShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());

    // 避免将地面写入蒙版缓存，因为地面不显示轮廓
    glStencilMask(0x00);
    /* 渲染地面 */
    Mat4f model = Mat4f();
    model = Mat4f::scale(model, Vec3f(8.0f));
    model = Mat4f::translate(model, Vec3f(0.0f, -1.0f, 0.0f));
    pModelShader -> setMat4f("model", model);
    pFloorTexture -> apply(GL_TEXTURE0);
    pModelFloor -> render(pModelShader); 
    
    // (1). 第一遍，以正常模式绘制立方体，并写入蒙版缓存
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    // 绘制立方体1
    model = Mat4f::rotateY(Mat4f(), angle);
    model = Mat4f::translate(model, Vec3f(-1.0f, 0.0f, -3.0f));
    pModelShader -> setMat4f("model", model);
    pCubeTexture -> apply(GL_TEXTURE0);
    pModelCube -> render(pModelShader);
    // 绘制立方体2
    model = Mat4f::rotateY(Mat4f(), angle);
    model = Mat4f::translate(model, Vec3f(1.0f, 0.0f, 0.0f));
    pModelShader -> setMat4f("model", model);
    pModelCube -> render(pModelShader);

    // (2). 第二遍，以轮廓着色器绘制稍微放大的立方体，并禁止模版缓存写入。
    // 因为蒙版缓存现在填充的都是1，正好对应于原来的立方体。 这次绘制时，蒙版缓存中为1的部分
    // 会被忽略，只绘制大小不同的部分，这就使得这部分看起来像轮廓
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //忽略不为1的部分
    glStencilMask(0x00); // 禁止更新蒙版缓存
    glDisable(GL_DEPTH_TEST); // 禁止更新深度缓存

    // 使用轮廓着色器重新绘制立方体
    // 绘制立方体1
    pOutlineShader -> use();
    float scale = 1.05f;
    model = Mat4f::rotateY(Mat4f(), angle);
    model = Mat4f::scale(model, Vec3f(scale)); // 放大一点点
    model = Mat4f::translate(model, Vec3f(-1.0f, 0.0f, -3.0f));
    pOutlineShader -> setMat4f("model", model);
    pModelCube -> render(pOutlineShader);
    // 绘制立方体2
    model = Mat4f::rotateY(Mat4f(), angle);
    model = Mat4f::scale(model, Vec3f(scale));
    model = Mat4f::translate(model, Vec3f(1.0f, 0.0f, 0.0f));
    pOutlineShader -> setMat4f("model", model);
    pModelCube -> render(pOutlineShader);
    // 允许更新蒙版缓存
    glStencilMask(0xFF);
    // 允许深度测试
    glEnable(GL_DEPTH_TEST);

    // 交换前后缓存
    glutSwapBuffers();
}

/* 模型初始化 */
static void initModel() {
    pModelCube = new Model("models/cube.obj");
    pModelFloor= new Model("models/plane.obj");
}

static void initTextures() {
    pCubeTexture = new Texture("textures/container2.png", Texture::Diffuse);
    pFloorTexture = new Texture("textures/test.png", Texture::Diffuse);
}

/* 初始化着色器 */
static void initShaders() {
    pModelShader = new Shader("GLSL/vs27.stencilTest.glsl", "GLSL/fs27.stencilTest.glsl");
    pOutlineShader = new Shader("GLSL/vs27.outline.glsl", "GLSL/fs27.outline.glsl");
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
    // 轮廓
    pOutlineShader -> use();
    pOutlineShader -> setColor3f("outlineColor", Color3f(1.0f, 0.6f, 0.0f));
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

    // 启用蒙版测试
    glEnable(GL_STENCIL_TEST);
    // 蒙版测试函数
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // 指定蒙版测试失败后的操作
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // 设置前向面
    //glFrontFace(GL_CCW);
    // 背面剔除
    //glCullFace(GL_BACK);
    // 启用背面剔除
    glEnable(GL_CULL_FACE);
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
        "OpenGL Tutorial 27 - Stencil Tesing", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/