#include <stdlib.h>
#include <math.h>
#include "gl_utils.h"

void setPixel(GLint x, GLint y) {
    glBegin(GL_POINTS);
      glVertex2f(x, y);
    glEnd();
}

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
}