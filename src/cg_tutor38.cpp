#include <cg_utils.h>
#include <cgm/cg_math.h>

#include <vector>
#include <sstream>
#include <cg_model.hpp>
#include <cg_skybox.hpp>
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
static Model * pModelPlanet;
static Model * pModelRock;
// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pAsteroidShader = NULL;
static Shader * pPlanetShader = NULL;
// 模型变换矩阵
static Mat4f * pModelMatrices;
// 数量
static unsigned int amount = 100000;

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

static void renderModel(float deltaTime) {
    Mat4f projection = pCamera -> getPerspectiveMatrix();
    Mat4f view = pCamera -> getViewMatrix();

    pAsteroidShader->use();
    pAsteroidShader->setMat4f("projection", projection);
    pAsteroidShader->setMat4f("view", view);
    pPlanetShader->use();
    pPlanetShader->setMat4f("projection", projection);
    pPlanetShader->setMat4f("view", view);
        
    // draw planet
    Mat4f model;
    model = Mat4f::scale(model, Vec3f(4.0f, 4.0f, 4.0f));
    model = Mat4f::translate(model, Vec3f(0.0f, -3.0f, 0.0f));  
    pPlanetShader->setMat4f("model", model);
    pModelPlanet->render(pPlanetShader);
   
    // draw meteorites
    pAsteroidShader->use();
    pAsteroidShader->setInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pModelRock -> getTexturesLoaded()[0].getId());
    vector<Mesh> meshes = pModelRock -> getMeshes();
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        glBindVertexArray(meshes[i].getVAO());
        vector<unsigned int> indices = meshes[i].getIndices();
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    } 
     
}

/* 渲染回调函数 */
static void renderSceneCB() {
    static float deltaTime = 0.0f;
    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    // 渲染模型
    renderModel(deltaTime);
    
    // 交换前后缓存
    glutSwapBuffers();
    deltaTime += 0.1;
}

/* 模型初始化 */
static void initModel() {
    pModelPlanet = new Model("models/planet/planet.obj");
    pModelRock = new Model("models/rock/rock.obj");
}

static void initModelMatrices() {
    pModelMatrices = new Mat4f[amount];
    srand(0.1); // 初始化随机数生成器的种子	
    float radius = 150.0f;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        Mat4f model;       
        // 1. 缩放: 在0.5和0.25之间随机缩放
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = Mat4f::scale(model, Vec3f(scale));
        // 2. 旋转: 围绕指定的轴向量随机旋转一定角度
        float rotAngle = (rand() % 360);
        model = Mat4f::rotate(model, rotAngle, Vec3f(0.4f, 0.6f, 0.8f));
        // 3. 平移: 在区间 [-offset, offset]内沿圆的周围平移一定距离，形成行星带
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // 使行星带的高度小于宽度
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = Mat4f::translate(model, Vec3f(x, y, z));
        
        // 4. 然后讲模型变换矩阵存储到每个小行星实例的变换矩阵列表中。
        // Mat4f矩阵为行主序，而opengl默认为列主序，因此需要转置
        pModelMatrices[i] = model.transposed();
    }
}

static void initInstancedArray() {
    // 创建实例数组缓存，用于存放每个实例的变换矩阵
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(Mat4f), &pModelMatrices[0], GL_STATIC_DRAW);

    // 将变换矩阵设置为实例顶点属性（divisor为1）
    vector<Mesh> meshes = pModelRock -> getMeshes();
    GLsizei vec4Size = sizeof(float) * 4;
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        unsigned int VAO = meshes[i].getVAO();
        glBindVertexArray(VAO);
        // 设置矩阵的属性指针
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }   
}

/* 初始化着色器 */
static void initShaders() {
    pAsteroidShader = new Shader(
        "GLSL/vs38.asteroid.glsl", 
        "GLSL/fs38.asteroid.glsl"
    );
    pPlanetShader = new Shader(
        "GLSL/vs38.planet.glsl",
        "GLSL/fs38.planet.glsl"
    );
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 0.0f, 155.0f));
}

/* 初始化灯光 */
static void initLight() {
}

/* 初始化材质 */
static void initMaterial() {
}

// 初始化OpenGL参数
static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 深度测试函数
    glDepthFunc(GL_LESS);
}

// 窗口调整回调函数
static void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// 初始化
static void init() {
    initGl();
    
    initShaders();
    initModel();
    initModelMatrices();
    initInstancedArray();

    initCamera();
    initLight();
    initMaterial();

    registerUIEvents();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 38 - Instancing : An asteroid field with instancing", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/