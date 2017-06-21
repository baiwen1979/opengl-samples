#include <GL/glut.h>

typedef struct vec2 {
    GLfloat x, y;
} Vec2;

typedef struct color {
    GLfloat r, g, b, a;
} Color;

void setPixel(GLint x, GLint y);
void lineDDA(GLint x0, GLint y0, GLint xEnd, GLint yEnd);
void lineBres(GLint x0, GLint y0, GLint xEnd, GLint yEnd);

