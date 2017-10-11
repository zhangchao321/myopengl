#pragma once
#include "commonexport.h" 
#include <string>
#include <glad/glad.h> 

class COMMON_EXPORT Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath );

public:
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

public:
	unsigned int programId;
};

