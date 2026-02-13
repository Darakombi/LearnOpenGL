#pragma once

#include "glad/glad.h"

#include <iostream>

struct ShaderProgramSource {
	std::string vertex;
	std::string fragment;
};

class Shader
{
private:
	unsigned int ID;
public:
	Shader(const char* shaderPath);
	~Shader();

	void Bind() const;
	void Unbind() const;
private:
	ShaderProgramSource ParseShader(const char* shaderPath) const;
	void AddShaders(const ShaderProgramSource src) const;
	unsigned int CompileShader(GLenum type, std::string src) const;
	void LinkProgram(const unsigned int vertexShader, unsigned int fragmentShader) const;

public:
	int GetUniformLocation(const char* name) const;
	void Uniform1i(const char* name, int v1) const;
	void Uniform4f(const char* name, float v1, float v2, float v3, float v4) const;
	void UniformMatrix4fv(const char* name, float* v1, bool transpose = false) const;
};