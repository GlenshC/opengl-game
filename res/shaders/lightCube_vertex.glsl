#version 330 core
layout (location = 0) in vec3 aModelPos;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;

void main()
{
    gl_Position = u_ProjMat * u_ViewMat * u_ModelMat * vec4(aModelPos, 1.0);
}