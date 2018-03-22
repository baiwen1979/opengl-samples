#include <cg_utils.h>

#include "cg_glw.hpp"

using namespace cg;
using namespace glw;

void renderLab1() {
    cout << "Running lab1..." << endl;

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