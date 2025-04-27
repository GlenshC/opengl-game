#version 330 core


uniform vec3 u_lightColor;
uniform vec3 u_toyColor;
uniform vec3 u_lightPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    vec3 result = (ambient + diffuse) * u_toyColor;
    
    FragColor =  vec4(result, 1.0);
}