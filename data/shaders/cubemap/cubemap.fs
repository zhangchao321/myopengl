#version 330 core
out vec4 fragColor; 
in vec3 texCoords; 
uniform samplerCube cubeTexture; 
void main()
{    
    fragColor = texture(cubeTexture, texCoords); 
}