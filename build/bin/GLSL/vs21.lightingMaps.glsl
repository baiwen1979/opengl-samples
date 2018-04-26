#version 330 core
// 顶点属性输入变量
layout (location = 0) in vec3 aPos; // 顶点位置
layout (location = 1) in vec3 aNormal; // 顶点法线
layout (location = 2) in vec2 aTexCoords; // 纹理坐标
// 片元信息输出变量
out vec3 FragPos; // 片元位置
out vec3 Normal; // 片元法线
out vec2 TexCoords; // 片元纹理坐标
// 变换矩阵统一变量
uniform mat4 model; // 模型变换矩阵
uniform mat4 view; // 视图变换矩阵
uniform mat4 projection; // 投影变换矩阵

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); // 输出顶点位置世界坐标
    Normal = mat3(transpose(inverse(model))) * aNormal; // 输出顶点法线
    TexCoords = aTexCoords; // 输出纹理坐标
    
    gl_Position = projection * view * vec4(FragPos, 1.0); // 输出最终顶点的变换结果
}