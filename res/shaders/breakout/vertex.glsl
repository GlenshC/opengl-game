#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 u_ProjMat;
uniform mat4 u_ModelMat;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = u_ProjMat * u_ModelMat * vec4(vertex.xy, 0.0, 1.0);
}