#include <cg_utils.h>

#include "cg_glw.hpp"

using namespace cg;
using namespace glw;

void renderLab1() {
    cout << "Running lab1..." << endl;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);

    glutSwapBuffers();

}

void renderLab2() {
    cout << "Running lab2..." << endl;
}

void init() {
    cout << "Initializing OpenGL ..." << endl;
}

void testCGCourseLab() {
    openGlWindow(renderLab1, "Lab1", init);
}