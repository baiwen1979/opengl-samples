#ifndef _CG_UTILS_H_
#define _CG_UTILS_H_

#include "cg_types.h"
#include "cg_prim2d.hpp"

namespace cg {

/**
 * 检查并输出OpenGL内部错误
 * @return {GLenum} 错误编码
 */
GLenum errorCheck ();

/**
 * 加载着色器
 * @param vertexShaderFilePath {char*} 顶点着色器文件的路径
 * @param fragmentShaderFilePath {char*} 片元着色器文件的路径
 */
GLuint LoadShaders(const char * vertexShaderFilePath, 
    const char * fragmentShaderFilePath);


} //namespace cg

#endif //_CG_UTILS_H_