#include <cg_utils.h>

void renderLab1() {
    cout << "Running lab1..." << endl;

}

void renderLab2() {
    cout << "Running lab2..." << endl;
}

void init(void * param) {
    cout << "Initializing OpenGL ..." << endl;
}

void testCGCourseLab() {
    openGlWindow(renderLab1, "Lab1", init);
}