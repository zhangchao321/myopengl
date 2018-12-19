#version 330 core
layout (location =0) in vec3 pos;
layout (location =1) in vec3 normal;
layout (location =2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
out vec3 v_fragPos;
out vec3 v_normal;
out vec2 v_texCoords;
out vec4 v_fragPosLightSpace;
void main()
{
	v_texCoords =texcoord;
    v_normal = transpose(inverse(mat3(model))) * normal;
	v_fragPos = vec3(model * vec4(pos, 1.0));
    v_fragPosLightSpace = lightSpaceMatrix * vec4(v_normal, 1.0);
	gl_Position =projection*view*model*vec4(pos,1);
}