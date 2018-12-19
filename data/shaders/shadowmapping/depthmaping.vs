#version 330 core
layout (location =0) in vec3 pos;
layout (location =1) in vec3 normal;
layout (location =2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 lightSpaceMatrix; 
out vec2 v_texcoord;
void main()
{ 
	gl_Position =lightSpaceMatrix*model*vec4(pos,1);
}