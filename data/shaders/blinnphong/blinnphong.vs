#version 330 core
layout (location = 0) in vec3 pos;  
layout (location = 1) in vec3 normal;  
uniform mat4 model; 
uniform mat4 view;
uniform mat4 projection;  
uniform mat4 modelNormalMat; 
uniform vec3 eyePos;
uniform vec3 lightPos;
uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform vec3 matrialColor; 

out vec3 v_color;
void main()
{ 
	vec3 ambient =ambientColor*matrialColor;
	vec3 mpos =(model*vec4(pos, 1.0)).xyz;
	vec3 mnormal =normalize(modelNormalMat*vec4(normal,1.0f)).xyz;
	vec3 incidentRay =normalize(lightPos-mpos);
	float diffuseFactor =dot(incidentRay,mnormal);
	vec3 diffuse =diffuseColor*matrialColor*max(0.0f,diffuseFactor);

	vec3 spcular =vec3(0.0f);
	if(diffuseFactor>0)
	{
		vec3 eyeDir =normalize(eyePos-mpos);
		vec3 halfVec =normalize(incidentRay+eyeDir); 
		float specularFactor =max(0.0f,pow(dot(halfVec,mnormal),10));
		spcular=specularFactor*matrialColor*specularColor;
	}
	v_color =ambient+diffuse+spcular;
    gl_Position = projection * view*model*vec4(pos, 1.0);
}