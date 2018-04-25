#ifndef _CG_SHADER_HPP_
#define _CG_SHADER_HPP_

#include "cg_types.h"
#include <string>

namespace cg {

class Shader {
public:
    // 构造函数，从指定的文件中加载GLSL并创建着色器对象
    Shader(const GLchar* vsPath, const GLchar* fsPath);

    // 使用并激活着色器
    void use() const;

    // 设置着色器中uniform的值
    void setFloat(const std::string &name, float value) const;
    void setInt(const std::string &name, int value) const;
    void setBool(const std::string &name, bool value) const;

    void setVec2f(const std::string &name, float x, float y) const;
    void setVec2f(const std::string &name, const Vec2f& value) const;
    void setVec3f(const std::string &name, float x, float y, float z) const;
    void setVec3f(const std::string &name, const Vec3f& value) const;

    void setMat3f(const std::string &name, const Mat3f& value) const;
    void setMat4f(const std::string &name, const Mat4f& value) const;

    void setColor3f(const std::string &name, const Color3f& value) const;
    void setColor4f(const std::string &name, const Color4f& value) const;

private:
    unsigned int id;

};

/**
 * 加载着色器
 * @param vertexShaderFilePath {char*} 顶点着色器文件的路径
 * @param fragmentShaderFilePath {char*} 片元着色器文件的路径
 */
GLuint LoadShaders(const char * vertexShaderFilePath, 
    const char * fragmentShaderFilePath);

}
#endif