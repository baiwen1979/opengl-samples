#ifdef _WIN32
    #include <windows.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <cstdlib>
#include <cmath>
