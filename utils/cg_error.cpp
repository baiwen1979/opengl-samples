#include <cg_utils.h>

namespace cg {
// 错误检查
GLenum errorCheck () {
    GLenum code;
    const GLubyte *errDesc;
    code = glGetError ();
    if (code != GL_NO_ERROR) {
        errDesc = gluErrorString (code);
        fprintf(stderr, "OpenGL error: %s\n", errDesc);
    }
    return code;
}

} //namespace cg