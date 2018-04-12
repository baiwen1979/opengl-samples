#include <cg_utils.h>
#include <cgm/cg_math.h>

#include <cassert>

#include "cg_glw.hpp"

using namespace cg;

// 平行光
struct DirectionalLight {
    Color3f color;    // 光的颜色
    float intensity;  // 光的强度
    Vec3f direction;  // 光的方向
    float diffuseIntensity; // 漫反射强度
};

// 带纹理坐标的顶点
struct Vertex {
    Vec3f pos;      // 顶点位置
    Vec2f texCoord; // 纹理坐标
    Vec3f normal;   // 顶点法线

    Vertex(Vec3f p, Vec2f tc): 
        pos(p), 
        texCoord(tc), 
        normal(Vec3f(0.0f, 0.0f, 0.0f)) {}
};

// 窗口设置
static const Recti WIN_RECT = Recti(100, 100, 1024, 768);

// 顶点数组对象VAO的ID
static GLuint vertexArray;
// 顶点缓存对象VBO的ID
static GLuint vertexBuffer;
// 索引缓存对象IBO的ID
static GLuint indexBuffer;
// 纹理对象
static GLuint texture;

// 着色器中世界坐标uniform的位置
static GLuint gWorldLocation;
// 着色器中世界观察投影坐标uniform的位置
static GLuint gWVPLocation;
// 着色器中纹理采样uniform的位置
static GLuint gSamplerLocation;
// 着色器中平行光颜色uniform的位置
static GLuint gDirLightColorLocation;
// 着色器中平行光强度uniform的位置
static GLuint gDirLightIntensityLocation;
// 着色器中平行光方向uniform的位置
static GLuint gDirLightDirectionLocation;
// 着色器中平行光漫反射强度uniform的位置
static GLuint gDirLightDiffuseIntensityLocation;

// 透视变换参数设置
static PersProjParams persProjParams;
// 摄像机
static Camera* pCamera = NULL;

// 着色器文件路径
static const char* vShaderFileName = "GLSL/vshader15.glsl";
static const char* fshaderFileName = "GLSL/fshader15.glsl";

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
    // 获得世界坐标变换矩阵  
    const Mat4f& world = p.getWorldTransform();
    // 获得世界视图坐标透视投影变换矩阵
    const Mat4f& wvp = p.getWVPTransform();

    // 将平移矩阵传递给着色器的uniform变量gWVP中
    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &wvp[0][0]);
    // 将世界坐标矩阵传递给着色器的uniform变量gWorld中
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &world[0][0]);

    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture);
    // 绑定到事先创建好的顶点数组对象
    glBindVertexArray(vertexArray);
 
    // 绘制图形元素
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    // 交换前后缓存
    glutSwapBuffers();
}

void calcNormals(const GLuint indices[], GLuint indexCount, Vertex vertices[], GLuint vertexCount)
{
    for (GLuint i = 0 ; i < indexCount ; i += 3) {
        GLuint index0 = indices[i];
        GLuint index1 = indices[i + 1];
        GLuint index2 = indices[i + 2];

        Vec3f v1 = vertices[index1].pos - vertices[index0].pos;
        Vec3f v2 = vertices[index2].pos - vertices[index0].pos;
        Vec3f normal = v1.cross(v2);

        normal.normalize();

        vertices[index0].normal += normal;
        vertices[index1].normal += normal;
        vertices[index2].normal += normal;
    }

    for (GLuint i = 0 ; i < vertexCount ; i++) {
        vertices[i].normal.normalize();
    }
}

/**
 * 创建顶点缓冲器
 */
static void createVertexBuffer(const Vertex vertices[], GLuint vertexCount) {
    // 生成1个顶点数组对象，并将其ID存储到vertexArray变量中
    glGenVertexArrays(1, &vertexArray);
    // 创建1个顶点缓冲对象，并将其ID保存到vertexBuffer中
    glGenBuffers(1, &vertexBuffer);
    // 先绑定顶点数组对象
    glBindVertexArray(vertexArray);

    // 绑定顶点缓冲对象GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // 拷贝顶点数组到顶点缓存对象
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // 顶点位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 开启顶点位置属性
    glEnableVertexAttribArray(0);
    // 顶点纹理坐标属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    // 开启纹理坐标属性
    glEnableVertexAttribArray(1);
    // 顶点法线属性
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
    // 开启顶点法线属性
    glEnableVertexAttribArray(2);

    // 暂时先解除绑定，绘制前重新绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// 创建索引缓冲器
static void createIndexBuffer(const GLuint indices[], GLuint indexCount) {
    // 创建索引缓冲对象
    glGenBuffers(1, &indexBuffer);
    // 绑定到顶点数组对象
    glBindVertexArray(vertexArray);
    // 绑定索引缓对象到GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    // 拷贝索引数组到索引缓冲
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

}

static void printVertices(const Vertex vertices[], int vertexCount) {
    for (int i = 0; i < vertexCount; i++) {
        cout << vertices[i].pos << vertices[i].normal << endl;
    }
}

// 创建纹理对象
static void createTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 设置当前绑定的纹理对象的纹理包裹和过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载纹理图像
    Texture tex = Texture::load("textures/container.jpg");
    if (tex.getData()) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.getWidth(), tex.getHeight(), 
            0, GL_RGB, GL_UNSIGNED_BYTE, tex.getData());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Failed to load Texture!" << endl;
    }
}

static void initScene() {
    // 金字塔（4面体）的4个顶点
    Vertex vertices[] = {
        Vertex(Vec3f(-1.0f, -1.0f, 0.5773f), Vec2f(0.0f, 0.0f)),
        Vertex(Vec3f(0.0f, -1.0f, -1.15475f), Vec2f(0.5f, 0.0f)),
        Vertex(Vec3f(1.0f, -1.0f, 0.5773f), Vec2f(1.0f, 0.0f)),
        Vertex(Vec3f(0.0f, 1.0f, 0.0f), Vec2f(0.5f, 1.0f))
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
    createVertexBuffer(vertices, vertexCount);
    createIndexBuffer(indices, indexCount);

    createTexture();

}

static void initShaders() {
    // 加载着色器
    GLuint program = LoadShaders(vShaderFileName, fshaderFileName);
    // 使用着色器
    glUseProgram(program);

    // 获取着色器中uniform变量的位置
    gWorldLocation = glGetUniformLocation(program, "gWorld");
    gWVPLocation = glGetUniformLocation(program, "gWVP");
    gSamplerLocation = glGetUniformLocation(program, "gSampler");
    gDirLightColorLocation = glGetUniformLocation(program, "gDirectionalLight.Color");
    gDirLightIntensityLocation = glGetUniformLocation(program, "gDirectionalLight.AmbientIntensity");
    gDirLightDirectionLocation = glGetUniformLocation(program, "gDirectionalLight.Direction");
    gDirLightDiffuseIntensityLocation = glGetUniformLocation(program, "gDirectionalLight.DiffuseIntensity");

    // 检查错误
    assert(gWorldLocation != 0xFFFFFFFF
        && gWVPLocation != 0xFFFFFFFF 
        && gSamplerLocation != 0xFFFFFFFF 
        && gDirLightColorLocation != 0xFFFFFFFF
        && gDirLightIntensityLocation != 0xFFFFFFFF
        && gDirLightDirectionLocation != 0xFFFFFFFF
        && gDirLightDiffuseIntensityLocation != 0xFFFFFFFF
        );
}

static void initPersProjParams() {
    persProjParams.aspectRatio = WIN_RECT.w / WIN_RECT.h;
    persProjParams.fieldOfView = 60.0f;
    persProjParams.zNear = 1.0f;
    persProjParams.zFar = 100.0f;
}

static void initCamera() {
    pCamera = new Camera(WIN_RECT.w, WIN_RECT.h);
    pCamera -> setPos(Vec3f(0.0f, 0.2f, 0.0f));
    pCamera -> setTarget(Vec3f(0.0f, 0.0f, 3.0f));
}

static void initDirectionalLight() {
    DirectionalLight dirLight;
    dirLight.color = Color3f(1.0f, 1.0f, 1.0f);
    dirLight.intensity = 0.1f;
    dirLight.direction = Vec3f(1.0f, 0.0f, 0.0f);
    dirLight.diffuseIntensity = 0.75f;

    glUniform3f(gDirLightColorLocation, dirLight.color.r, dirLight.color.g, dirLight.color.b);
    glUniform1f(gDirLightIntensityLocation, dirLight.intensity);

    glUniform3f(gDirLightDirectionLocation, dirLight.direction.x, dirLight.direction.y, dirLight.direction.z);
    glUniform1f(gDirLightDiffuseIntensityLocation, dirLight.diffuseIntensity);

}


// 事件处理函数
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

void testMat3() {
    Mat3f a(
        1, 1, 1, 
        2, 2, 2, 
        3, 3, 3
    );

    Mat3f b(
        1, 2, 3, 
        4, 5, 6, 
        7, 8, 9
    );

    cout << "a:\n" << a;
    cout << "b:\n" << b;

    cout << "a + b:\n" << a + b;
    cout << "b - a:\n" << b - a;
    cout << "a * b:\n" << a * b;
    cout << "a * 2:\n" << a * 2;

    cout << "a.transpose():\n" << a.transpose();
    cout << "a:\n" << a;
    cout << "b.transposed():\n" << b.transposed();
    cout << "b:\n" << b;

    Mat3f m;
    Vec2f p(1,1);
    cout << "p = " << p << endl;

    Mat3f sm = Mat3f::scale(m, 2, 2);
    cout << "scale(m):\n" << sm;
    cout << "p' = " << sm * p << endl;

    Mat3f rm = Mat3f::rotate(m, 45, 0, 0);
    cout << "rotate(m):\n" << rm;
    cout << "p' = " << rm * p << endl;
    cout << "rm * sm:\n" << rm * sm << endl;

    Mat3f tm = Mat3f::translate(m, 2, 3);
    cout << "translate(m):\n" << tm;
    cout << "p' = " << tm * p << endl;
    cout << "T(2,3) * R(45,0,0) * S(2,2)\n" 
         << tm * rm * sm << endl;

    cout << p << "->scale(2,2)->rotate(45,0,0)->tranlate(2,3)->"
         << tm * rm * sm * p << endl;

    m = Mat3f::scale(m, Vec2f(2, 2));
    cout << "m(scale):\n" << m;
    m = Mat3f::rotate(m, 45, Vec2f(0, 0));
    cout << "m(rotate):\n" << m;
    m = Mat3f::translate(m, Vec2f(2, 3));
    cout << "m(translate):\n" << m;
    cout << p << "->scale(2,2)->rotate(45,0,0)->tranlate(2,3)->"
         << m * p << endl;
}

static void init() {
    // 设置前向面
    glFrontFace(GL_CW);
    // 背面剔除
    glCullFace(GL_BACK);
    // 启用背面剔除
    glEnable(GL_CULL_FACE);
    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    initScene();
    initShaders();
    initPersProjParams();
    initCamera();
    initDirectionalLight();

    registerUIEvents();

    testMat3();
}


void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 15 - Diffuse Lighting ", 
        init, 
        NULL, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}








