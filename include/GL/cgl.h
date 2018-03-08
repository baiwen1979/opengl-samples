#ifndef __CGL_H__
#define __CGL_H__

#ifdef _WIN32
    #include <windows.h>
    #include <GL/glut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cmath>

#endif //__CGL_H__
