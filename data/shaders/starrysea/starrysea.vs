#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in mat4 instanceMatrix;

out vec3 vColor;

uniform mat4 projection;
uniform mat4 view;
void main()
{
    vColor = color;
    gl_Position = projection * view * instanceMatrix * vec4(pos, 1.0f); 
}