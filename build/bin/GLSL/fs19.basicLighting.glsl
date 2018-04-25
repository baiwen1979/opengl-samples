#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // 环境光
    float ambientStrength = 0.1; // 环境光强度
    vec3 ambient = ambientStrength * lightColor; // 环境光颜色
  	
    // 漫反射   
    vec3 norm = normalize(Normal); // 单位法向量
    vec3 lightDir = normalize(lightPos - FragPos); // 计算片元位置的光线方向
    float diff = max(dot(norm, lightDir), 0.0);    // 计算片元位置的光照强度
    vec3 diffuse = diff * lightColor; // 计算漫反射颜色
    
    // 高光
    float specularStrength = 1.0; // 高光强度
    float shininess = 32.0; // 光泽度
    vec3 viewDir = normalize(viewPos - FragPos); // 计算片元位置的视点方向
    vec3 reflectDir = reflect(-lightDir, norm);  // 计算反射光方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 计算视点方向的反射光（高光）强度系数
    vec3 specular = specularStrength * spec * lightColor;  // 计算高光颜色
        
    vec3 result = (ambient + diffuse + specular) * objectColor; // 计算Phong光照颜色
    FragColor = vec4(result, 1.0);
}