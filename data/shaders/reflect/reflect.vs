#version 330 core
layout (location = 0) in vec3 pos; 
out vec3 texCoords;
uniform mat4 model; 
uniform mat4 view;
uniform mat4 projection; 
uniform vec3 eyePos;
void main()
{
	vec3 start =pos-eyePos;
	vec3 r =reflect(start,pos);
    texCoords = r;
    gl_Position = projection * view*model*vec4(pos, 1.0);
}