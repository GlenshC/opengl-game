#version 330 core
layout (location = 0) in vec3 aModelPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
// out vec3 LightPos;
out vec2 TexCoords;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;

// uniform vec3 u_lightPos;

void main()
{
    FragPos = vec3(u_ViewMat * u_ModelMat * vec4(aModelPos, 1.0));
    Normal =  mat3(transpose(inverse(u_ViewMat * u_ModelMat))) * aNormal;
    TexCoords = aTexCoord;
    
    // LightPos = vec3(u_ViewMat * vec4(u_lightPos, 1.0));
    gl_Position = u_ProjMat * vec4(FragPos, 1.0);
}