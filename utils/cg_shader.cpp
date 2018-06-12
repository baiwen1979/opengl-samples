#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <cg_utils.h>

namespace cg {

Shader::Shader(const GLchar *vsPath, const GLchar *fsPath, const GLchar *gsPath) {
	id = LoadShaders(vsPath, fsPath, gsPath);
}

void Shader::use() const {
	glUseProgram(id);
}

unsigned int Shader::getId() const {
	return id;
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2f(const std::string& name, const Vec2f& value) const {
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2f(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::setVec3f(const std::string& name, const Vec3f& value) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3f(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);	
}

void Shader::setVec3fv(const std::string& name, const Vec3f* value, unsigned int size) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), size, &value[0][0]);
}

void Shader::setMat3f(const std::string& name, const Mat3f& value) const {
	GLuint location = glGetUniformLocation(id, name.c_str());
	glUniformMatrix3fv(location, 1, GL_TRUE, &value[0][0]);
}

void Shader::setMat4f(const std::string& name, const Mat4f& value) const {
	GLuint location = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_TRUE, &value[0][0]);
}

void Shader::setColor3f(const std::string& name, const Color3f& value) const {
	GLuint location = glGetUniformLocation(id, name.c_str());
	glUniform3f(location, value.r, value.g, value.b);
}

void Shader::setColor4f(const std::string& name, const Color4f& value) const {
	GLuint location = glGetUniformLocation(id, name.c_str());
	glUniform4f(location, value.r, value.g, value.b, value.a);
}

void checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cout << "ERROR: Shader Compilation Error of type: " << type << "\n" << infoLog 
				 << "\n ------------------------------------------------------- " << endl;
        }
		else {
			cout << type << " Shader Complied Successfully." << endl;
		}		
    }
	else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cout << "ERROR: Program Linking Error of type: " << type << "\n" << infoLog 
				 << "\n ------------------------------------------------------- " << endl;
        }
		else {
			cout << "Shader Program Linked Successfully." << endl;
		}
    }
}

GLuint LoadShaders(
	const char * vertexPath, 
	const char * fragmentPath,
	const char * geometryPath
){
	unsigned int programId;
	// 1. 从指定的文件中加载着色器源码
    string vertexCode;
	string fragmentCode;
    string geometryCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    ifstream gShaderFile;
    // 确保输入流可以抛出异常
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
		// 文本流对象
        stringstream vShaderStream, fShaderStream;
        // 读取文件内容到文本流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 将文本流转为字符串
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();			
        // 如果指定了几何着色器，则读取之
        if(geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            geometryCode = gShaderStream.str();
        }
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		// 2. 编译着色器
		unsigned int vertex, fragment;
		// 编译顶点着色器
		cout << "Compiling Vertex Shader: " << vertexPath << endl;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// 编译片元着色器
		cout << "Compiling Fragment Shader: " << fragmentPath << endl;
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// 如果指定了几何着色器，则编译之
		unsigned int geometry;
		if(geometryPath != NULL) {
			const char * gShaderCode = geometryCode.c_str();
			cout << "Compiling Geometry Shader: " << geometryPath << endl;
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}
		// 创建并链接着色器程序
		programId = glCreateProgram();
		glAttachShader(programId, vertex);
		glAttachShader(programId, fragment);
		if(geometryPath != NULL) {
			glAttachShader(programId, geometry);
		}
		glLinkProgram(programId);
		checkCompileErrors(programId, "PROGRAM");
		// 链接后，删除着色器对象
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if(geometryPath != NULL) {
			glDeleteShader(geometry);
			gShaderFile.close();
		}
		// 关闭文件
        vShaderFile.close();
        fShaderFile.close();
    }
    catch (ifstream::failure e) {
        cout << "ERROR: Failed to Open Shader file: ";
		if (!vShaderFile.is_open()) {
			cout << vertexPath << endl;
		}
		else if (!fShaderFile.is_open()) {
			cout << fragmentPath << endl;
		}
		else if (!gShaderFile.is_open()) {
			cout << geometryPath << endl;
		}
    }
    
	// 返回着色程序的ID
	return programId;
}

} //namespace cg