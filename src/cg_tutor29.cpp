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
static Shader * pShader = NULL;
static Shader * pScreenShader = NULL;
// 纹理
static Texture * pCubeTexture = NULL;  // 纹理用于立方体
static Texture * pFloorTexture = NULL; // 纹理用于平面

// 帧缓存
static unsigned int framebuffer;
// 纹理颜色缓存
static unsigned int textureColorbuffer;
// 渲染缓存对象
static unsigned int renderBufferObject;

unsigned int quadVAO, quadVBO;

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
    float xoffset = lastX - x;
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

static void renderFrameQuad() {
    pScreenShader -> use();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void renderModel() {
    // 旋转角度
    static float angle = 0.0f;
    // 旋转增量
    angle += 0.1f; 

    pShader -> use();
    pShader -> setMat4f("view", pCamera -> getViewMatrix());
    pShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    
    // 渲染立方体
    Mat4f model;
    model = Mat4f::rotateY(model, angle);
    model = Mat4f::translate(model, Vec3f(-1.0f, 0.0f, -3.0f));
    pShader -> setMat4f("model", model);
    pCubeTexture -> apply(GL_TEXTURE0);
    pModelCube -> render(pShader);

    model = Mat4f::rotateY(Mat4f(), angle);
    model = Mat4f::translate(model, Vec3f(1.0f, 0.0f, 0.0f));
    pShader -> setMat4f("model", model);
    pModelCube -> render(pShader);

    // 渲染地面
    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(8.0f));
    model = Mat4f::translate(model, Vec3f(0.0f, -1.0f, 0.0f));
    pShader -> setMat4f("model", model);
    pFloorTexture -> apply(GL_TEXTURE0);
    pModelFloor -> render(pShader); 
}

/* 渲染回调函数 */
static void renderSceneCB() {
    /* 第一遍 */
    // 绑定新的帧缓存并启用深度测试
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 渲染模型
    renderModel();

    /* 第二遍 */
    // 绑定到默认帧缓存并禁用深度测试
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 清除颜色缓存
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // 清楚颜色缓存
    glClear(GL_COLOR_BUFFER_BIT);
    // 禁用深度测试
    glDisable(GL_DEPTH_TEST);
    // 渲染帧缓存到四方形
    renderFrameQuad();
    
    // 交换前后缓存
    glutSwapBuffers();
}

/* 模型初始化 */
static void initModel() {
    pModelCube = new Model("models/cube.obj");
    pModelFloor= new Model("models/plane.obj");
}

static void initTextures() {
    pCubeTexture = new Texture("textures/container.jpg", Texture::Diffuse);
    pFloorTexture = new Texture("textures/test.png", Texture::Diffuse);
}

/* 初始化着色器 */
static void initShaders() {
    pShader = new Shader("GLSL/vs29.framebuffers.glsl", "GLSL/fs29.framebuffers.glsl");
    pScreenShader = new Shader("GLSL/vs29.screenTexture.glsl", "GLSL/fs29.screenTexture.glsl");
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 0.0f, 3.0f));
}

/* 初始化灯光 */
static void initLight() {
}

/* 初始化材质 */
static void initMaterial() {
    pShader -> use();
    pShader -> setInt("texture1", 0);
    pScreenShader -> use();
    pScreenShader -> setInt("screenTexture", 0);
}

static void initFrameQuad() {
    Vec2f quadVertices[] = {
        // 位置              // 纹理坐标
        Vec2f(-1.0f,  1.0f), Vec2f(0.0f, 1.0f),
        Vec2f(-1.0f, -1.0f), Vec2f(0.0f, 0.0f),
        Vec2f( 1.0f, -1.0f), Vec2f(1.0f, 0.0f),
        Vec2f(-1.0f,  1.0f), Vec2f(0.0f, 1.0f),
        Vec2f( 1.0f, -1.0f), Vec2f(1.0f, 0.0f),
        Vec2f( 1.0f,  1.0f), Vec2f(1.0f, 1.0f)
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(Vec2f), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(Vec2f), (void*)(sizeof(Vec2f)));
}

static void initFramebuffer() {
    // 创建帧缓存
    glGenFramebuffers(1, &framebuffer);
    // 绑定帧缓存
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // 创建纹理缓存
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIN_RECT.w, WIN_RECT.h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // 创建渲染缓存
    glGenRenderbuffers(1, &renderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIN_RECT.w, WIN_RECT.h); // 一个渲染缓存对象用作深度和蒙版缓存
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject); // 附加到当前绑定的帧缓存   
    // 已经创建好帧缓存并且添加了纹理和渲染缓存，现在检查帧缓存的完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    // 绑定默认帧缓存
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// 初始化OpenGL参数
static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 深度测试函数
    glDepthFunc(GL_LESS);
    // 启用背面剔除
    //glEnable(GL_CULL_FACE);
    // 设置前向面
    //glFrontFace(GL_CW);
    // 背面剔除
    //glCullFace(GL_BACK);
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

    initFramebuffer(); 
    initFrameQuad(); 

    initCamera();
    initLight();
    initMaterial();

    registerUIEvents();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 29 - Framebuffers and Post-processing", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/