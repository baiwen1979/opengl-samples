#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 1.0; 
float far  = 20.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{    
    FragColor = texture(texture1, TexCoords);
    // 深度缓存可视化
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    // float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    // FragColor = vec4(vec3(depth), 1.0);
}