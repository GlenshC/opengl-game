#version 330 core
layout (location = 0) in vec3 aModelPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;
uniform vec3 u_lightPos;

void main()
{
    vec4 viewPos = u_ViewMat * u_ModelMat * vec4(aModelPos, 1.0);

    gl_Position = u_ProjMat *  viewPos;
    FragPos = vec3(viewPos);
    Normal = mat3(transpose(inverse(u_ViewMat * u_ModelMat))) * aNormal;
    
    LightPos = vec3(u_ViewMat * vec4(u_lightPos, 1.0));
}