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

// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pModelShader = NULL; // 模型着色器
static Shader * pSimpleDepthShader = NULL; // 深度着色器
// 纹理
static Texture * pWoodTexture = NULL; // 木地板纹理

// 深度贴图的帧缓存对象
static unsigned int depthMapFBO;
static unsigned int depthCubemap;

// 阴影贴图的分辨率
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
// 是否产生阴影
bool shadows = true;

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
        case 's':
            shadows = !shadows;
            break;
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
    // 启用面剔除
    glEnable(GL_CULL_FACE);
    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

// 窗口调整回调函数
static void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

static Vec3f lightPos(0.0f, 0.0f, 0.0f);
static float near_plane = 1.0f, far_plane = 7.5f;
static vector<Mat4f> shadowTransforms(6);

static void setDepthCubemapTransforms() {
    Mat4f projection = Mat4f::perspective(90.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
    shadowTransforms[0] = projection * Mat4f::lookAt(lightPos, lightPos + Vec3f( 1.0f, 0.0f, 0.0f), Vec3f(0.0f, -1.0f, 0.0f));
    shadowTransforms[1] = projection * Mat4f::lookAt(lightPos, lightPos + Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(0.0f, -1.0f, 0.0f));
    shadowTransforms[2] = projection * Mat4f::lookAt(lightPos, lightPos + Vec3f(0.0f,  1.0f, 0.0f), Vec3f(0.0f,  0.0f, 1.0f));
    shadowTransforms[3] = projection * Mat4f::lookAt(lightPos, lightPos + Vec3f(0.0f, -1.0f, 0.0f), Vec3f(0.0f, 0.0f, -1.0f));
    shadowTransforms[4] = projection * Mat4f::lookAt(lightPos, lightPos + Vec3f(0.0f,  0.0f, 1.0f), Vec3f(0.0f, -1.0f, 0.0f));
    shadowTransforms[5] = projection * Mat4f::lookAt(lightPos, lightPos + Vec3f(0.0f,  0.0f,-1.0f), Vec3f(0.0f,  0.0f, 1.0f));
}

// renders the 3D scene
// --------------------
static void renderModel(const Shader* pShader, float deltaTime) {
    // room cube
    Mat4f model = Mat4f::scale(Mat4f(), Vec3f(5.0f));
    pShader -> setMat4f("model", model);
    glDisable(GL_CULL_FACE);
    pShader -> setBool("reverse_normals", true);
    pModelCube -> render(pShader);
    pShader -> setBool("reverse_normals", false);
    glEnable(GL_CULL_FACE);
    // cubes
    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(0.5f));
    model = Mat4f::rotateY(model, deltaTime);
    model = Mat4f::translate(model, Vec3f(4.0f, -3.5f, 0.0f));
    pShader -> setMat4f("model", model);
    pWoodTexture -> apply(GL_TEXTURE0);
    pModelCube -> render(pShader);

    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(0.75f));
    model = Mat4f::rotateY(model, deltaTime);
    model = Mat4f::translate(model, Vec3f(2.0f, 3.0f, 1.0f));  
    pShader -> setMat4f("model", model);
    pModelCube -> render(pShader);

    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(0.5f));
    model = Mat4f::rotateX(model, deltaTime);
    model = Mat4f::translate(model, Vec3f(-3.0f, -1.0f, 0.0));
    pShader -> setMat4f("model", model);
    pModelCube -> render(pShader);

    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(0.5f));
    model = Mat4f::rotateZ(model, deltaTime);
    model = Mat4f::translate(model, Vec3f(-1.5f, 1.0f, 1.5));
    pShader -> setMat4f("model", model);
    pModelCube -> render(pShader);

    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(0.75));
    model = Mat4f::rotate(model, deltaTime, Vec3f::normalize(Vec3f(1.0, 0.0, 1.0)));
    model = Mat4f::translate(model, Vec3f(-1.5f, 2.0f, -3.0f));
    pShader -> setMat4f("model", model);
    pModelCube -> render(pShader);
}

static void renderSceneAsNormal(float deltaTime) {
    glViewport(0, 0, WIN_RECT.w, WIN_RECT.h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mat4f projection = pCamera -> getPerspectiveMatrix();
    Mat4f view = pCamera -> getViewMatrix();

    pModelShader -> use();
    pModelShader -> setMat4f("projection", projection);
    pModelShader -> setMat4f("view", view);

    // 光照
    pModelShader -> setVec3f("lightPos", lightPos);
    pModelShader -> setVec3f("viewPos", pCamera -> getPosition());
    pModelShader -> setBool("shadows", shadows);
    pModelShader -> setFloat("far_plane", far_plane);
    pWoodTexture -> apply(GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    renderModel(pModelShader, deltaTime);
}

// 渲染场景到立方体深度贴图
static void renderSceneToDepthCubemap(float deltaTime) {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    pSimpleDepthShader -> use();
    for (unsigned int i = 0; i < 6; ++i){
        pSimpleDepthShader -> setMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    pSimpleDepthShader -> setFloat("far_plane", far_plane);
    pSimpleDepthShader -> setVec3f("lightPos", lightPos);
    renderModel(pSimpleDepthShader, deltaTime);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

/* 渲染回调函数 */
static void renderSceneCB() {
    // 清空颜色、深度和蒙版缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    static float deltaTime = 0.0f;

    renderSceneToDepthCubemap(deltaTime);
    renderSceneAsNormal(deltaTime);

    glutSwapBuffers();
    deltaTime += 0.1;
}

/* 模型初始化 */
static void initModel() {
    pModelCube = new Model("models/cube.obj");
}

static void initTextures() {
    pWoodTexture = new Texture("textures/wood.png", Texture::Diffuse);
}

/* 初始化着色器 */
static void initShaders() {
    pSimpleDepthShader = new Shader(
        "GLSL/vs41.pointShadowD.glsl",
        "GLSL/fs41.pointShadowD.glsl",
        "GLSL/gs41.pointShadowD.glsl"
    );

    pModelShader = new Shader(
        "GLSL/vs41.pointShadow.glsl", 
        "GLSL/fs41.pointShadow.glsl"
    );
}

static void initDepthCubeMap() {
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_2D, depthCubemap);
    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float) WIN_RECT.h, Vec3f(0.0f, 0.0f, 3.0f));
}

/* 初始化灯光 */
static void initLight() {
}

/* 初始化材质 */
static void initMaterial() {
    pModelShader -> use();
    pModelShader -> setInt("diffuseTexture", 0);
    pModelShader -> setInt("shadowMap", 1);
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
    initDepthCubeMap();
    registerUIEvents();
}


void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 41 - Advanced Lighting: Shadow Mapping", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
