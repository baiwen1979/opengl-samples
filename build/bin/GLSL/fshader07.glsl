#version 330

// 接受顶点着色器传来的插值后的颜色值
in vec4 Color;

out vec4 FragColor;

void main()
{
    // 颜色值作为片元着色器的输出
    FragColor = Color;
}