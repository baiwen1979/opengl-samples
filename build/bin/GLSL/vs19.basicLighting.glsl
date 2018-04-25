#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    
    FragPos = vec3(model * vec4(aPos, 1.0)); // 计算片元的世界坐标位置
    mat3 normalMatrix = mat3(transpose(inverse(model))); // 计算法线矩阵
    Normal =  normalMatrix * aNormal; // 计算法向量
    gl_Position = projection * view * vec4(FragPos, 1.0);
}