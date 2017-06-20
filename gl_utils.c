#include <stdlib.h>
#include <math.h>
#include "gl_utils.h"

void setPixel(GLint x, GLint y) {
    glBegin(GL_POINTS);
      glVertex2f(x, y);
    glEnd();
}

inline GLint round(const GLfloat a) {
    return GLint(a + 0.5);
}

//DDA画线算法
void lineDDA(GLint x0, GLint y0, GLint xEnd, GLint yEnd) {
    GLint dx = xEnd - x0, dy = yEnd - y0;
    GLint steps, k;
    GLfloat xInc, yInc, x = x0, y = y0;
    if (fabs(dx) > fabs(dy)) {
        steps = fabs(dx);
    }
    else
    {
        steps = fabs(dy);
    }

    xInc = GLfloat(dx) / GLfloat(steps);
    yInc = GLfloat(dy) / GLfloat(steps);
    setPixel(round(x), round(y));
    for (k = 0; k < steps; k++) {
        x += xInc;
        y += yInc;
        setPixel(round(x), round(y));
    }
}