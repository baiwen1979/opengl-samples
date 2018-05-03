#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

// 折射率（玻璃）
const float ratio = 1.00 / 1.52;

void main()
{             
    vec3 I = normalize(Position - cameraPos);
    // 反射
    vec3 R = reflect(I, normalize(Normal));
    // 折射
    //R = refract(I, normalize(Normal), ratio);

    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}