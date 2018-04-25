#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <cg_utils.h>

namespace cg {

Shader::Shader(const GLchar *vsPath, const GLchar *fsPath) {
	id = LoadShaders(vsPath, fsPath);
}

void Shader::use() const {
	glUseProgram(id);
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

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

	// 创建着色器
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);     //顶点着色器
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER); //片元着色器

	// 从文件中读取顶点着色器代码（GLSL）
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Can't open %s. Please check the path !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// 从文件中读取片元着色器代码
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// 编译顶点着色器
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// 检查顶点着色器是否编译错误
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// 编译片元着色器
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// 检查片元着色器是否有编译错误
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// 绑定并链接着色器程序
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// 检测是否有链接错误
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	// 分离着色器
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	// 删除着色器
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	// 返回着色程序的ID
	return ProgramID;
}

} //namespace cg


