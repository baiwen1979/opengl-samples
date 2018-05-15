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
static Shader * pSimpleDepthShader = NULL; //深度着色器
static Shader * pDebugQuadShader = NULL; 
// 纹理
static Texture * pFloorTexture = NULL; // 纹理用于地面
static Texture * pCubeTexture = NULL; // 纹理用于立方体

// 深度贴图的帧缓存对象
static unsigned int depthMapFBO;
static unsigned int depthMap;
// 阴影贴图的分辨率
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

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

static Vec3f lightPos(-2.0f, 4.0f, -1.0f);
static Mat4f lightSpaceMatrix;
static float near_plane = 1.0f, far_plane = 7.5f;

// renders the 3D scene
// --------------------
static void renderModel(const Shader* pShader, float deltaTime) {
    // floor
    Mat4f model = Mat4f::scale(Mat4f(), Vec3f(8.0f));
    model = Mat4f::translate(model, Vec3f(0.0f, 0.0f, 0.0f));
    pShader -> setMat4f("model", model);
    pModelPlane -> render(pShader);
    // cubes
    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(0.5f));
    model = Mat4f::rotateY(model, deltaTime);
    model = Mat4f::translate(model, Vec3f(-3.0f, 1.0f, 0.0));
    pShader -> setMat4f("model", model);
    pCubeTexture -> apply(GL_TEXTURE0);
    pModelCube -> render(pShader);

    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(0.5f));
    model = Mat4f::rotateY(model, deltaTime);
    model = Mat4f::translate(model, Vec3f(1.0f, 1.2f, 1.0));  
    pShader -> setMat4f("model", model);
    pModelCube -> render(pShader);

    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(0.25));
    model = Mat4f::rotate(model, deltaTime, Vec3f::normalize(Vec3f(1.0, 0.0, 1.0)));
    model = Mat4f::translate(model, Vec3f(-1.0f, 1.0f, 2.0f));
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
    pModelShader -> setMat4f("lightSpaceMatrix", lightSpaceMatrix);
    pModelShader -> setVec3f("viewPos", pCamera -> getPosition());
    pFloorTexture -> apply(GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderModel(pModelShader, deltaTime);
}

// 渲染场景到深度贴图
static void renderDepthMap(float deltaTime) {
    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------
    Mat4f lightProjection, lightView;
    //lightProjection = Mat4f::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); 
    // note that if you use a perspective projection matrix you'll have to change the 
    // light position as the current light position isn't enough to reflect the whole scene
    lightProjection = Mat4f::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = Mat4f::lookAt(lightPos, Vec3f(0.0f), Vec3f(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    // render scene from light's point of view
    pSimpleDepthShader -> use();
    pSimpleDepthShader -> setMat4f("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderModel(pSimpleDepthShader, deltaTime);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);    
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
static unsigned int quadVAO = 0;
static unsigned int quadVBO;
static void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
static void renderDebugQuad() {
    // render Depth map to quad for visual debugging
    // ---------------------------------------------
    pDebugQuadShader -> use();
    pDebugQuadShader -> setFloat("near_plane", near_plane);
    pDebugQuadShader -> setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderQuad();
}

/* 渲染回调函数 */
static void renderSceneCB() {
    // 清空颜色、深度和蒙版缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    static float deltaTime = 0.0f;

    renderDepthMap(deltaTime);
    renderSceneAsNormal(deltaTime);
    //renderDebugQuad();
    // 交换前后缓存
    glutSwapBuffers();
    deltaTime += 0.1;
}

/* 模型初始化 */
static void initModel() {
    pModelCube = new Model("models/cube.obj");
    pModelPlane= new Model("models/plane.obj");
}

static void initTextures() {
    pFloorTexture = new Texture("textures/wood.png", Texture::Diffuse);
    pCubeTexture = new Texture("textures/container.jpg", Texture::Diffuse);
}

/* 初始化着色器 */
static void initShaders() {
    pModelShader = new Shader(
        "GLSL/vs40.shadowMapping.glsl", 
        "GLSL/fs40.shadowMapping.glsl"
    );
    pSimpleDepthShader = new Shader(
        "GLSL/vs40.shadowMappingDepth.glsl",
        "GLSL/fs40.shadowMappingDepth.glsl"
    );
    pDebugQuadShader = new Shader(
        "GLSL/vs40.debugQuad.glsl",
        "GLSL/fs40.debugQuad.glsl"
    );

    pModelShader -> use();
    pModelShader -> setInt("diffuseTexture", 0);
    pModelShader -> setInt("shadowMap", 1);

    pDebugQuadShader -> use();
    pDebugQuadShader -> setInt("depthMap", 0);
}

static void initDepthMapFBO() {
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float) WIN_RECT.h, Vec3f(-1.0f, 3.0f, 6.0f));
}

/* 初始化灯光 */
static void initLight() {
    pModelShader -> use();
    pModelShader -> setVec3f("lightPos", lightPos);
}

/* 初始化材质 */
static void initMaterial() {
    // 模型
    pModelShader -> use();
    pModelShader -> setInt("texture1", 0);
}

// 初始化
static void init() {
    initGl();
    initShaders();
    initTextures();
    initModel();
    initCamera();
    initLight();
    //initMaterial();
    initDepthMapFBO();
    registerUIEvents();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 40 - Advanced Lighting: Shadow Mapping", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/