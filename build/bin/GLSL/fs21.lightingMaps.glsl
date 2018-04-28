#version 330 core
// 片元颜色输出向量
out vec4 FragColor;

// 材质
struct Material {
    sampler2D texture_diffuse; // 漫反射纹理贴图
    sampler2D texture_specular; // 高光纹理贴图
    sampler2D texture_emission; // 自发光纹理贴图
    float shininess; // 光泽度
}; 
// 光照
struct Light {
    vec3 position; // 位置

    vec3 ambient; // 环境光颜色
    vec3 diffuse; // 漫反射颜色
    vec3 specular; // 高光颜色
};

// 片元信息输入变量
in vec3 FragPos; // 位置
in vec3 Normal; // 法向量
in vec2 TexCoords; // 纹理坐标

// 应用程序可访问的统一变量
uniform vec3 viewPos; // 视点坐标
uniform Material material; // 材质
uniform Light light; // 光照

void main()
{
    // 环境光
    vec3 ambient = light.ambient * texture(material.texture_diffuse, TexCoords).rgb;
  	
    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoords).rgb;  
    
    // 高光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular, TexCoords).rgb;

    // 自发光
    // vec3 emission = texture(material.texture_emission, TexCoords).rgb;
        
    vec3 result = ambient + diffuse + specular ; //+ emission;
    FragColor = vec4(result, 1.0);
} 