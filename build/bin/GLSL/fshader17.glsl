#version 330 core
// 输出
out vec4 FragColor;

// 材质
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
    float shininess;

    sampler2D texture_diffuse1;
};

// 灯光
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 输入
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// 统一变量
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = texture(material.texture_diffuse1, TexCoords.xy) * vec4(result, 1.0);
} 