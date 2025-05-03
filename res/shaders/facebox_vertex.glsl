#version 330 core
layout (location = 0) in vec3 aModelPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_ModelMat;
uniform mat4 u_ProjMat;
out vec2 texCoord;

void main()
{
    gl_Position = u_ProjMat * u_ModelMat * vec4(aModelPos, 1.0);
    texCoord = aTexCoord;  
}