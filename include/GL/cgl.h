#ifndef __CGL_H__
#define __CGL_H__

#include <GL/glew.h>

#ifdef _WIN32
    #include <windows.h>
    #include <GL/glut.h>
    #define GLUT_3_2_CORE_PROFILE 0x00
#elif __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#endif //__CGL_H__
