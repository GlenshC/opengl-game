#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

uniform vec3 u_lightColor;
uniform vec3 u_toyColor;
uniform vec3 u_cameraPos; // worldpos

in vec3 FragPos; // view
in vec3 LightPos; // view
in vec3 Normal;


out vec4 FragColor;

void main() {
    // ambient
    vec3 ambient = material.ambient * u_lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos); // good
    float diff = max(dot(norm, lightDir), 0.0); // 
    vec3 diffuse = (diff * material.diffuse) * u_lightColor;

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specular =  u_lightColor * (spec * material.specular);

    vec3 result = (ambient + diffuse + specular);
    
    FragColor =  vec4(result, 1.0);
}