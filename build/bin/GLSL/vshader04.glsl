#version 330

layout (location = 0) in vec3 Position;

// 平移变换聚矩阵一致变量
uniform mat4 glWorld;

void main()
{
    // 用平移变换矩阵乘以图形顶点位置对应的4X4矩阵相乘，完成平移变换
    gl_Position = glWorld * vec4(Position, 1.0);
}