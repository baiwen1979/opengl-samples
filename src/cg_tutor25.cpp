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
static Model * pModel = NULL;
static Model * pLamp = NULL;

// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pLightingShader = NULL;
static Shader * pLampShader = NULL;
// 纹理
static Texture * pDiffuseMap = NULL;
static Texture * pSpecularMap = NULL;

// 模型位置
static Vec3f modelPositions[] = {
    Vec3f( 0.0f,  0.0f,  0.0f),
    Vec3f( 2.0f,  5.0f, -15.0f),
    Vec3f(-1.5f, -2.2f, -2.5f),
    Vec3f(-3.8f, -2.0f, -12.3f),
    Vec3f( 2.4f, -0.4f, -3.5f),
    Vec3f(-1.7f,  3.0f, -7.5f),
    Vec3f( 1.3f, -2.0f, -2.5f),
    Vec3f( 1.5f,  2.0f, -2.5f),
    Vec3f( 1.5f,  0.2f, -1.5f),
    Vec3f(-1.3f,  1.0f, -1.5f)
};

// 点光源位置
static Vec3f pointLightPositions[] = {
    Vec3f( 0.7f,  0.2f,  2.0f),
    Vec3f( 2.3f, -3.3f, -4.0f),
    Vec3f(-4.0f,  2.0f, -12.0f),
    Vec3f( 0.0f,  3.0f, -3.0f)
};

/* 渲染回调函数 */
static void renderSceneCB() {
    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 旋转角度
    static float angle = 0.0f;
    // 旋转增量
    angle += 0.1f; 

    pLightingShader -> use();
    pLightingShader -> setVec3f("spotLight.position", pCamera -> getPosition());
    pLightingShader -> setVec3f("spotLight.direction", pCamera -> getFront());
    pLightingShader -> setVec3f("viewPos", pCamera -> getPosition());

    pLightingShader -> setMat4f("view", pCamera -> getViewMatrix());
    pLightingShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());

    // 应用纹理到纹理单元
    pDiffuseMap -> apply(GL_TEXTURE0);
    pSpecularMap -> apply(GL_TEXTURE1);
    
    // 渲染模型
    Mat4f model;
    for (unsigned int i = 0; i < 10; i++) {        
        model = Mat4f::translate(Mat4f(), modelPositions[i]);
        //float angle = 20.0f * i;
        model = Mat4f::rotate(model, angle * (i + 1), Vec3f(1.0f, 0.3f, 0.5f));
        model = Mat4f::translate(model, -1.0f * pCamera -> getPosition());
        pLightingShader -> setMat4f("model", model);
        // 绘制模型
        pModel->render(*pLightingShader);
    }

    // 渲染光源
    pLampShader -> use();
    pLampShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    pLampShader -> setMat4f("view", pCamera -> getViewMatrix());
    for (unsigned int i = 0; i < 4; i++)
    {
        model = Mat4f();
        model = Mat4f::scale(model, Vec3f(0.4f)); // Make it a smaller cube
        model = Mat4f::translate(model, pointLightPositions[i]);      
        model = Mat4f::translate(model, -1.0f * pCamera -> getPosition());
        pLampShader -> setMat4f("model", model);
        pLamp -> render(*pLampShader);
    }
    // 交换前后缓存
    glutSwapBuffers();
}

/* 模型初始化 */
static void initModel() {
    pModel = new Model("models/cube.obj");
    pLamp = new Model("models/sphere.obj");
}

static void initTextures() {
    pDiffuseMap = new Texture("textures/container2.png", Texture::Diffuse);
    pSpecularMap = new Texture("textures/container2_specular.png", Texture::Specular);
}

/* 初始化着色器 */
static void initShaders() {
    pLightingShader = new Shader("GLSL/vs25.multiLights.glsl", "GLSL/fs25.multiLights.glsl");
    pLampShader = new Shader("GLSL/vs20.lamp.glsl", "GLSL/fs20.lamp.glsl");
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 0.0f, 3.0f));
}

/* 初始化灯光 */
static void initLight() {
    pLightingShader -> use();
    // 平行光
    pLightingShader -> setVec3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
    pLightingShader -> setVec3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    pLightingShader -> setVec3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    pLightingShader -> setVec3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // 点光源   
    for (int i = 0; i < 4; i++) {
        stringstream ss;
        ss << "pointLights[" << i << "]";
        pLightingShader -> setVec3f("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
        pLightingShader -> setVec3f("pointLights[" + std::to_string(i) + "].ambient", Vec3f(0.05f, 0.05f, 0.05f));
        pLightingShader -> setVec3f("pointLights[" + std::to_string(i) + "].diffuse", Vec3f(0.8f, 0.8f, 0.0f));
        pLightingShader -> setVec3f("pointLights[" + std::to_string(i) + "].specular", Vec3f(1.0f, 1.0f, 1.0f));
        pLightingShader -> setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
        pLightingShader -> setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
        pLightingShader -> setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032);
    }
    // 聚光灯
    pLightingShader -> setVec3f("spotLight.ambient", Vec3f(0.0f, 0.0f, 0.0f));
    pLightingShader -> setVec3f("spotLight.diffuse", Vec3f(1.0f, 1.0f, 1.0f));
    pLightingShader -> setVec3f("spotLight.specular", Vec3f(1.0f, 1.0f, 1.0f));
    pLightingShader -> setFloat("spotLight.constant", 1.0f);
    pLightingShader -> setFloat("spotLight.linear", 0.09);
    pLightingShader -> setFloat("spotLight.quadratic", 0.032);
    pLightingShader -> setFloat("spotLight.cutOff", cosf(toRadian(12.5f)));
    pLightingShader -> setFloat("spotLight.outerCutOff", cosf(toRadian(15.0f)));

    pLampShader -> use();
    pLampShader -> setColor3f("lightColor", Color3f(1.0f, 1.0f, 0.0f));
}

/* 初始化材质 */
static void initMaterial() {
    pLightingShader -> use();
    pLightingShader -> setInt("material.texture_diffuse", 0);
    pLightingShader -> setInt("material.texture_specular", 1);
    pLightingShader -> setFloat("material.shininess", 32.0f);
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

// 初始化OpenGL参数
static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 设置前向面
    glFrontFace(GL_CCW);
    // 背面剔除
    glCullFace(GL_BACK);
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
        "OpenGL Tutorial 25 - Lighting: Multiple Lights", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/