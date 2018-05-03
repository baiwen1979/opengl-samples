#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{             
    vec4 texColor = texture(texture1, TexCoords);
    // 丢弃透明部分片元
    if(texColor.a < 0.10)
        discard;
    
    FragColor = texColor;
}