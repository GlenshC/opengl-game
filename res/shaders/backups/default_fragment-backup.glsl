#version 330 core


uniform vec3 u_lightColor;
uniform vec3 u_toyColor;
uniform vec3 u_cameraPos; // worldpos

in vec3 FragPos; // view
in vec3 LightPos; // view
in vec3 Normal;


out vec4 FragColor;

void main() {
    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    vec3 ambient = ambientStrength * u_lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos); // good

    float diff = max(dot(norm, lightDir), 0.0); // 
    vec3 diffuse = diff * u_lightColor;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 specular = specularStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_toyColor;
    
    FragColor =  vec4(result, 1.0);
}