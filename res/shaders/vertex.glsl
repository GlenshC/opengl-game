#version 330 core
layout (location = 0) in vec3 aModelPos;
layout (location = 1) in vec3 aWorldPos;
layout (location = 2) in vec3 aColor;

uniform mat4 u_Proj;
out vec4 ourColor;

void main()
{
    gl_Position = u_Proj * vec4(aWorldPos + aModelPos, 1.0);
    ourColor = vec4(aColor, 1.0); 
}