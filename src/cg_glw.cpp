#include <cg_utils.h>

namespace glw {

using namespace cg;

static float g_zoom = 2000.f;  
static float g_rotx = 20;  
static float g_roty = 0;  
static float g_tx = 0;  
static float g_ty = 0;
static int g_lastx = 0;  
static int g_lasty = 0;  
static unsigned char g_mouseButtons[3] = {0};


// 鼠标移动事件回调函数/
static void onMotion(int x, int y)  
{  
    int diffx = x - g_lastx;  
    int diffy = y - g_lasty;  
    g_lastx = x;  
    g_lasty = y;  
      
    if(g_mouseButtons[2])  
    {  
        g_zoom -= (float)1 * diffx * 2;  
    }  
    else {
        if(g_mouseButtons[0])  
        {  
            g_rotx += (float)1 * diffy;  
            g_roty += (float)1 * diffx;  
        }  
        else {
            if(g_mouseButtons[1])  
            {  
                g_tx += (float)1 * diffx;  
                g_ty -= (float)1 * diffy;  
            }
        } 
    } 
    glutPostRedisplay();  
} 

// 鼠标按键事件回调函数/
static void onMouse(int b, int s, int x, int y)  
{  
    g_lastx = x;  
    g_lasty = y;  
    switch(b)  
    {  
        case GLUT_LEFT_BUTTON:  
            g_mouseButtons[0] = ((GLUT_DOWN == s)? 1: 0);  
            break;  
        case GLUT_MIDDLE_BUTTON:  
            g_mouseButtons[1] = ((GLUT_DOWN == s)? 1: 0);  
            break;  
        case GLUT_RIGHT_BUTTON:  
            g_mouseButtons[2] = ((GLUT_DOWN == s)? 1: 0);  
            break;  
        default:  
            break;  
    }  
    glutPostRedisplay();  
}   

// 按键事件回调函数/
static void onKeyboard(unsigned char key, int x, int y)  
{  
    switch(key) {  
        case 'q':  
        case 'Q':  
        case 27: // ESC或Q键退出  
            exit(0);  
            break;  
    }  
}

// 辅助函数：创建OpenGL窗口
inline void createGlWindow(const char* title, Recti rect, bool usingGL3Mode = false) {
    int argc = 1; 
    char* argv[] = { "CGLab" };
    // 初始化GLUT/
    glutInit(&argc, argv);
    // 显示模式
    GLuint displayMode = GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL;
    // 是否使用OpenGL3模式
    if (usingGL3Mode) {
        displayMode |= GLUT_3_2_CORE_PROFILE;
    }
    glutInitDisplayMode(displayMode);
    // 窗口设置/
    glutInitWindowPosition(rect.x, rect.y);  //窗口位置/
    glutInitWindowSize(rect.w, rect.h);      //窗口尺寸/
    glutCreateWindow(title);                 //窗口标题/
}

// 注册默认的UI时间处理函数
inline void registerDefaultUIEventHandlers() {
    // 注册键盘按键事件回调函数/
    glutKeyboardFunc(onKeyboard);
    // 注册鼠标按键事件回调函数/
    glutMouseFunc(onMouse);
    // 注册鼠标移动事件回调函数/
    glutMotionFunc(onMotion);
}

// 默认的初始化函数
inline void initDefault() {
    // 注册默认的UI事件处理函数
    registerDefaultUIEventHandlers();
}


// 初始化GLEW
inline bool initGLEW() {
    GLenum res = glewInit();   
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return false;
    }
    return true;
}

// 输出OpenGL的版本信息
inline void printOpenGLInfo() {
    cout << "-------------------------------------------" << endl;
    cout << "OpenGL Vender: " << glGetString(GL_VENDOR) << endl;
    cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    cout << "-------------------------------------------" << endl;
}

// 初始化OpenGL窗口
void initGlWindow (
    void(*renderCallback)(),
    void(*initGlCallback)(), 
    void(*reshapeCallback)(int, int), 
    void(*idleCallback)()) {
    // 初始化GLEW
    if (!initGLEW()) {
        return;
    }
    // 输出OpenGL的版本信息
    printOpenGLInfo();
    // 默认初始化
    initDefault();
    // 初始化GL渲染上下文
    if (initGlCallback) {
        initGlCallback();
    }
    // 注册渲染回调函数
    glutDisplayFunc(renderCallback);
    // 注册窗口大小改变回调函数
    if (reshapeCallback) {
        glutReshapeFunc(reshapeCallback);
    }
    // 注册空闲（动画）回调函数
    if (idleCallback) {
        glutIdleFunc(idleCallback);
    }
    // 开始GLUT内部消息循环/
    glutMainLoop();  
}

// 打开OpenGL窗口/
void openGlWindow (
    void(*renderCallback)(), const char* title, 
    void(*initGlCallback)(), void(*reshapeCallback)(int, int),
    void(*idleCallback)(),bool usingGL3, const Recti& rect) {
    createGlWindow(title, rect, usingGL3);   
    initGlWindow(renderCallback, initGlCallback, reshapeCallback, idleCallback);
}

} //namespace glw