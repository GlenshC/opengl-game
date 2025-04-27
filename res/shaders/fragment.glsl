#version 330 core
out vec4 FragColor;

uniform vec3 u_lightColor;
uniform vec3 u_toyColor;
void main() {
    FragColor =  vec4(u_lightColor * u_toyColor, 1.0);
}