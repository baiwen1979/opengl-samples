#include <cg_utils.h>
#include <cgm/cg_math.h>

#include <vector>
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

// 网格对象
static Model * pModelCube = NULL;
static Model * pModelSphere = NULL;
// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pLightingShader = NULL;
static Shader * pLampShader = NULL;

// 灯光位置
static Vec3f lightPos(-3.0f, 2.0f, -2.0f);
// 灯光颜色
static Vec3f lightColor(1.0f, 1.0f, 1.0f);
// 纹理
static Texture * pDiffuseMap = NULL;
static Texture * pSpecularMap = NULL;
static Texture * pEmissionMap = NULL;

/* 渲染回调函数 */
static void renderSceneCB() {
    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 旋转角度
    static float angle = 0.0f;
    // 旋转增量
    angle += 0.1f;

    // 环境光和漫反射
    Vec3f diffuseColor = lightColor * Vec3f(0.5f);
    Vec3f ambientColor = lightColor * Vec3f(0.2f);  

    // 光照
    pLightingShader -> use();
    pLightingShader -> setVec3f("light.position", lightPos);
    pLightingShader -> setVec3f("viewPos", pCamera -> getPosition());

    pLightingShader -> setVec3f("light.ambient", ambientColor);
    pLightingShader -> setVec3f("light.diffuse", diffuseColor);
    
    // 被照射模型
    Mat4f model = Mat4f::scale(Mat4f(), Vec3f(1.0f));
    model = Mat4f::rotateX(model, angle);
    model = Mat4f::rotateY(model, angle);
    model = Mat4f::translate(model, Vec3f(0.0f, 0.0f, -4.0f));

    pLightingShader -> setMat4f("model", model);
    pLightingShader -> setMat4f("view", pCamera -> getViewMatrix());
    pLightingShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    // 应用纹理到纹理单元
    pDiffuseMap -> apply(GL_TEXTURE0);
    pSpecularMap -> apply(GL_TEXTURE1);
    pEmissionMap -> apply(GL_TEXTURE2);
    // 绘制立方体模型
    pModelCube->render(pLightingShader);

    // 灯具模型
    model = Mat4f::scale(Mat4f(), Vec3f(0.15f));
    model = Mat4f::translate(model, lightPos);

    pLampShader -> use();
    pLampShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    pLampShader -> setMat4f("view", pCamera -> getViewMatrix());
    pLampShader -> setMat4f("model", model);
    pModelSphere->render(pLampShader);

    // 交换前后缓存
    glutSwapBuffers();
}

/* 模型初始化 */
static void initModel() {
    pModelCube = new Model("models/cube.obj");
    pModelSphere = new Model("models/sphere.obj");
}

static void initTextures() {
    pDiffuseMap = new Texture("textures/container2.png", Texture::Diffuse);
    pSpecularMap = new Texture("textures/container2_specular.png", Texture::Specular);
    pEmissionMap = new Texture("textures/matrix.jpg", Texture::Diffuse);
}

/* 初始化着色器 */
static void initShaders() {
    pLightingShader = new Shader("GLSL/vs21.lightingMaps.glsl", "GLSL/fs21.lightingMaps.glsl");
    pLampShader = new Shader("GLSL/vs20.lamp.glsl", "GLSL/fs20.lamp.glsl");
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 0.0f, 3.0f));
}

/* 初始化灯光 */
static void initLight() {
    pLightingShader -> use();
    pLightingShader -> setColor3f("light.specular", Color3f(1.0f, 1.0f, 1.0f));
    pLampShader -> use();
    pLampShader -> setVec3f("lightColor", lightColor);
}

/* 初始化材质 */
static void initMaterial() {
    pLightingShader -> use();
    pLightingShader -> setInt("material.texture_diffuse", 0);
    pLightingShader -> setInt("material.texture_specular", 1);
    pLightingShader -> setInt("material.texture_emission", 2);
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

    // 启用背面剔除
    glEnable(GL_CULL_FACE);
    // 设置前向面
    glFrontFace(GL_CCW);
    // 背面剔除
    glCullFace(GL_BACK);

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
        "OpenGL Tutorial 21 - Lighting: Lighting Maps", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/
