#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform Light light;
uniform Material material;

in vec2 TexCoords; // view
in vec3 FragPos; // view
in vec3 LightPos; // view
in vec3 Normal;


out vec4 FragColor;

void main() {

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); 

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular =  light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 emission = texture(material.emission, TexCoords).rgb;

    vec3 result = (ambient + diffuse + specular + emission);
    
    FragColor =  vec4(result, 1.0);
}