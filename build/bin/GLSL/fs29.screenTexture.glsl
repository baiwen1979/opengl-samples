#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;
const int kernelSize = 9;

vec2 offsets[kernelSize] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
);

vec3 kernelFilter(float kernel[kernelSize], vec2 offsets[kernelSize]) {
    vec3 sampleTex[kernelSize];
    for(int i = 0; i < kernelSize; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < kernelSize; i++) {
        col += sampleTex[i] * kernel[i];
    }
    return col;
}

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;

    /* Post-processing */

    // 反相
    // col = vec3(1.0) - col;

    // 灰度
    // float average = (col.r + col.g + col.b) / 3.0;
    // average = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
    // col = vec3(average, average, average);

    // 锐化
    /*
    float sharpenKernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    col = kernelFilter(sharpenKernel, offsets);
    */

    // 模糊
    /*
    float blurKernel[9] = float[] (
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    col = kernelFilter(blurKernel, offsets);
    */

    // 边缘检测
    /*
    float edgeKernel[9] = float[](
        1,  1,  1,
        1, -8,  1,
        1,  1,  1
    );
    col = kernelFilter(edgeKernel, offsets);
    */
    
    FragColor = vec4(col, 1.0);
} 