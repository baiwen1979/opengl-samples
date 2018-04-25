#version 330

in vec2 TexCoord0; 
in vec3 Normal0;
in vec3 WorldPos0;                                                                  

out vec4 FragColor;

struct DirectionalLight {                                                                                   
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
    vec3 Direction;
};

struct Material {
    float SpecularIntensity;
    float SpecularPower;
    sampler2D texture_diffuse1;
};

uniform DirectionalLight gDirectionalLight;
uniform Material material;
uniform vec3 gEyeWorldPos;
                                                                                    
void main()                                                                         
{
    // 计算环境光颜色
    vec4 AmbientColor = vec4(gDirectionalLight.Color * gDirectionalLight.AmbientIntensity, 1.0f);

    // 计算漫反射系数: df = n.l
    vec3 LightDirection = -gDirectionalLight.Direction; 
    vec3 Normal = normalize(Normal0);
    float DiffuseFactor = dot(Normal, LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);
    // 计算漫射光
    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(gDirectionalLight.Color * gDirectionalLight.DiffuseIntensity * DiffuseFactor, 1.0f);
    }
    // 计算高光系数
    vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
    vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
    float SpecularFactor = dot(VertexToEye, LightReflect);                    
    if (SpecularFactor > 0) {
        SpecularFactor = pow(SpecularFactor, material.SpecularPower);
        SpecularColor = vec4(gDirectionalLight.Color * material.SpecularIntensity * SpecularFactor, 1.0f);                                                                          
    }
    FragColor = texture(material.texture_diffuse1, TexCoord0.xy) * (AmbientColor + DiffuseColor + SpecularColor);
}
