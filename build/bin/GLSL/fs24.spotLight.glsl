#version 330 core
out vec4 FragColor;

// 材质
struct Material {
    sampler2D texture_diffuse; // 漫反射纹理
    sampler2D texture_specular; // 高光纹理   
    float shininess; // 光泽度
}; 
// 光照
struct Light {
    // 聚光灯属性
    vec3 position; // 位置
    vec3 direction; // 方向
    float cutOff; // 内光锥角
    float outerCutOff; // 外光锥角
    // 基本光照属性
    vec3 ambient; // 环境光
    vec3 diffuse; // 漫反射
    vec3 specular; // 高光
	// 点光源属性
    float constant; // 0次衰减系数（常量）
    float linear; // 1次衰减系数（线性）
    float quadratic; // 2次衰减系数（平方）
};

in vec3 FragPos; // 片元位置
in vec3 Normal; // 法线 
in vec2 TexCoords; // 纹理坐标
  
uniform vec3 viewPos; // 视点位置
uniform Material material; // 材质
uniform Light light; // 光照

void main()
{
    // 计算片元的环境光照颜色
    vec3 ambient = light.ambient * texture(material.texture_diffuse, TexCoords).rgb;
    
    // 计算片元的漫反射光照颜色 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoords).rgb;  
    
    // 计算片元的高光颜色
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular, TexCoords).rgb;  
    
    // 计算片元的聚光灯高照强度 (模糊边缘)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 聚光灯强度影响漫反射和高光
    diffuse  *= intensity;
    specular *= intensity;
    
    // 距离衰减系数
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 衰减系数影响环境光、漫反射和高光   
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
    // 最终片元颜色（环境光+漫反射+高光）   
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 