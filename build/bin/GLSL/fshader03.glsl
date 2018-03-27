#version 330  //告诉编译器我们的目标GLSL编译器版本是3.3

out vec4 outColor;  // 片段着色器的输出颜色变量

// 着色器的唯一入口函数
void main()
{
    // 定义输出颜色值
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}