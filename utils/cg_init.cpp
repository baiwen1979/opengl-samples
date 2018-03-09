#include <cg_utils.h>

float zoom = 2000.f;  
float rotx = 20;  
float roty = 0;  
float tx = 0;  
float ty = 0;
int lastx = 0;  
int lasty = 0;  
unsigned char Buttons[3] = {0};
GLenum mode = GL_2D;

// 初始化2D绘图上下文
void init2D(GLsizei width, GLsizei height) {
    // 设置显示窗口的颜色为白色
    glClearColor(1.0, 1.0, 1.0, 0.0);
    // 设置矩阵模式为投影矩阵
    glMatrixMode(GL_PROJECTION);
    // 使用glu 2D正交投影
    gluOrtho2D(0.0, width, 0.0, height);
}

// 初始化3D绘图上下文
void init3D() {
    // 设置显示窗口的颜色为黑色
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 清除深度缓存
    glClearDepth(1.0f);
    // 指定深度比较函数为小于等于
    glDepthFunc(GL_LEQUAL);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);  
    // 启用多边形（面片）剔除  
    glEnable(GL_CULL_FACE);  
    // 启用规格化
    glEnable(GL_NORMALIZE); 
    // 设置矩阵模式为投影矩阵
    glMatrixMode(GL_PROJECTION);
    // 使用正交投影矩阵
    glOrtho(-5, 5, -5, 5, 5, 15);
    // 设置模型视图矩阵模式
    glMatrixMode(GL_MODELVIEW);
    // 设置摄像机位置
    gluLookAt(0,0,10, 0,0,0, 0,1,0);
}

// 窗口调整事件回调函数
void onReshape(int w, int h) {
    // 避免最小化时出现除零错误
    if (w = 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    if (mode == GL_2D) {
        gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
        //glClear(GL_COLOR_BUFFER_BIT);
    }
    else {
        gluPerspective(45, (float)w / h, 0.1, 5000);  
        glMatrixMode(GL_MODELVIEW);  
        glLoadIdentity(); 
    }   
}

// 鼠标移动事件回调函数
void onMotion(int x,int y)  
{  
    int diffx = x - lastx;  
    int diffy = y - lasty;  
    lastx = x;  
    lasty = y;  
      
    if(Buttons[2])  
    {  
        zoom -= (float)1 * diffx * 2;  
    }  
    else {
        if(Buttons[0])  
        {  
            rotx += (float)1 * diffy;  
            roty += (float)1 * diffx;  
        }  
        else {
            if(Buttons[1])  
            {  
                tx += (float)1 * diffx;  
                ty -= (float)1 * diffy;  
            }
        } 
    } 
    glutPostRedisplay();  
} 

// 鼠标按键事件回调函数
void onMouse(int b,int s,int x,int y)  
{  
    lastx = x;  
    lasty = y;  
    switch(b)  
    {  
        case GLUT_LEFT_BUTTON:  
            Buttons[0] = ((GLUT_DOWN == s)?1:0);  
            break;  
        case GLUT_MIDDLE_BUTTON:  
            Buttons[1] = ((GLUT_DOWN == s)?1:0);  
            break;  
        case GLUT_RIGHT_BUTTON:  
            Buttons[2] = ((GLUT_DOWN == s)?1:0);  
            break;  
        default:  
            break;  
    }  
    glutPostRedisplay();  
}   

// 按键事件回调函数
void onKeyboard(unsigned char key, int x, int y)  
{  
    switch(key) {  
        case 'q':  
        case 'Q':  
        case 27: // ESC或Q键退出  
            exit(0);  
            break;  
    }  
}  

// 打开窗口
void openWindow(const char* title, void(*renderCallback)(), 
                GLsizei width, GLsizei height, GLenum mode){
    int argc = 0;
    // 初始化GLUT
    glutInit(&argc, NULL);
    // 显示模式：双缓存、RGBA
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    // 窗口设置
    glutInitWindowPosition(100, 100);  //窗口位置
    glutInitWindowSize(width, height); //窗口尺寸
    glutCreateWindow(title);           //窗口大小
    // 开始渲染
    glutDisplayFunc(renderCallback);
    //glutReshapeFunc(onReshape);
    // 注册键盘按键事件回调函数
    glutKeyboardFunc(onKeyboard);
    // 注册鼠标按键事件回调函数
    glutMouseFunc(onMouse);
    // 注册鼠标移动事件回调函数
    glutMotionFunc(onMotion);
    if (mode == GL_2D) {
        init2D(width, height);
    }
    else {
        init3D();
    }
    // 开始GLUT内部消息循环
    glutMainLoop();
}