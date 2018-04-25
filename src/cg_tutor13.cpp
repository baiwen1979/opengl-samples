#include <cg_utils.h>
#include <cgm/cg_math.h>
#include <cassert>

#include "cg_glw.hpp"

using namespace cg;

// 窗口设置
static const Recti WIN_RECT = Recti(100, 100, 1024, 768);

// 网格对象
static Mesh* pMesh = NULL;
// 透视变换参数设置
static PersProjParams persProjParams;
// 摄像机
static CameraQuat* pCamera = NULL;
// 着色器
static Shader* pShader = NULL;

// 着色器文件路径
static const char* vShaderFileName = "GLSL/vshader13.glsl";
static const char* fshaderFileName = "GLSL/fshader13.glsl";

 /**
 * 渲染回调函数
 */
static void renderSceneCB() {
    pCamera -> onRender();
    // 旋转角度
    static float scale = 0.0f;
    // 旋转增量
    scale += 0.5f;
 
    Pipeline p;
    p.posit(0.0f, 0.0f, 3.0f);
    p.rotate(0.0f, scale, 0.0f);
    // 设置摄像机
    p.setCamera(*pCamera);
    // 设置透视投影变换参数
    p.setPersProjParams(persProjParams);
    // 获得世界视图坐标透视投影变换矩阵
    const Mat4f& world = p.getWVPTransform();

    pShader -> setMat4f("gWVP", world);

    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 渲染网格对象
    pMesh->render(*pShader);
    // 交换前后缓存
    glutSwapBuffers();
}

static void initMesh() {
    // 金字塔（4面体）的4个顶点
    Vertex vertices[] = {
        Vertex(Vec3f(-1.0f, -1.0f, 0.5773f), Vec3f(0), Vec2f(0.0f, 0.0f)),
        Vertex(Vec3f(0.0f, -1.0f, -1.15475f), Vec3f(0), Vec2f(0.5f, 0.0f)),
        Vertex(Vec3f(1.0f, -1.0f, 0.5773f), Vec3f(0), Vec2f(1.0f, 0.0f)),
        Vertex(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0), Vec2f(0.5f, 1.0f))
    };

    // 金字塔（4面体）的4个面
    GLuint indices[] = { 
        0, 3, 1, 
        1, 3, 2, 
        2, 3, 0, 
        0, 1, 2
    };

    GLuint indexCount = sizeof(indices)/sizeof(GLuint);
    GLuint vertexCount = sizeof(vertices)/sizeof(Vertex);

    calcNormals(indices, indexCount, vertices, vertexCount);
    //printVertices(vertices, vertexCount);

    vector<Texture> textures;
    Texture tex = Texture::load("textures/test.png", Texture::Diffuse);
    textures.push_back(tex);

    pMesh = new Mesh(vector<Vertex>(vertices, vertices + vertexCount), vector<GLuint>(indices, indices + indexCount), textures);

}

static void initShaders() {
    pShader = new Shader(vShaderFileName, fshaderFileName);
    pShader -> use();
}

static void initPersProjParams() {
    persProjParams.aspectRatio = WIN_RECT.w / WIN_RECT.h;
    persProjParams.fieldOfView = 60.0f;
    persProjParams.zNear = 1.0f;
    persProjParams.zFar = 100.0f;
}

static void initCamera() {
    pCamera = new CameraQuat(WIN_RECT.w, WIN_RECT.h);
}

static void specialKeyboardCB(int key, int x, int y) {
    pCamera -> onKeyboard(key);
}

static void keyboardCB(unsigned char key, int x, int y) {
    switch(key) {
        case 'q':
        case 'Q':
        case 27:
            exit(0);
    }
}

static void passiveMouseCB(int x, int y) {
    pCamera -> onMouse(x, y);
}

static void registerUIEvents() {
    glutSpecialFunc(specialKeyboardCB);
    glutPassiveMotionFunc(passiveMouseCB);
    glutKeyboardFunc(keyboardCB);
}

static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 设置前向面
    glFrontFace(GL_CW);
    // 背面剔除
    glCullFace(GL_BACK);
    // 启用背面剔除
    glEnable(GL_CULL_FACE);
    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

static void init() {
    initGl();
    initMesh();
    initShaders();
    initPersProjParams();
    initCamera();

    registerUIEvents();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 13 - Basic Texture Mapping ", 
        init, 
        NULL, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/






