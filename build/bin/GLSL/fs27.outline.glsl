#version 330 core
out vec4 FragColor;

// 轮廓颜色
uniform vec3 outlineColor;

void main()
{
    FragColor = vec4(outlineColor, 1.0);
}