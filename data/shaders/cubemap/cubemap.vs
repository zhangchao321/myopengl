#version 330 core
layout (location = 0) in vec3 pos; 
out vec3 texCoords;
uniform mat4 model; 
uniform mat4 view;
uniform mat4 projection; 
void main()
{
    texCoords = pos;    
	mat3 moveView =mat3(view*model);
	mat4 mv =mat4(moveView);
    gl_Position = projection * mv*vec4(pos, 1.0);
}