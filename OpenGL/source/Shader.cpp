#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(const char* shaderPath) {
	ID = glCreateProgram();
	glUseProgram(ID);

	/*if (!std::ifstream(shaderPath).good()) {
		std::cout << "Invalid shader path" << std::endl;
		return;
	}*/

	AddShaders(ParseShader(shaderPath));
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

ShaderProgramSource Shader::ParseShader(const char* shaderPath) const {
	std::ifstream stream(shaderPath);

	enum ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType st = NONE;

	std::string line;
	std::stringstream ss[2];
	while (std::getline(stream, line)) {
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos) {
				st = VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				st = FRAGMENT;
			}
		}
		else {
			if (st != NONE)
				ss[(int)st] << line << '\n';
		}
	}

	return{ ss[0].str(), ss[1].str() };
}

void Shader::AddShaders(const ShaderProgramSource src) const {
	std::string vs = src.vertex;
	const char* vertexSource = vs.c_str();
	std::string fs = src.fragment;
	const char* fragmentSource = fs.c_str();

	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	LinkProgram(vertexShader, fragmentShader);
}

unsigned int Shader::CompileShader(GLenum type, std::string src) const {
	unsigned int shader = glCreateShader(type);
	const char* source = src.c_str();

	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		char log[512];
		glGetShaderInfoLog(shader, sizeof(compiled), nullptr, log);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" :
				(type == GL_FRAGMENT_SHADER ? "fragment" : " "))
			<< "shader\n" << log << std::endl;
	}
	return shader;
}

void Shader::LinkProgram(const unsigned int vertexShader, const unsigned int fragmentShader) const {
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);
	int linked;
	glGetProgramiv(ID, GL_LINK_STATUS, &linked);
	if (!linked) {
		char log[512];
		glGetProgramInfoLog(ID, sizeof(log), nullptr, log);
		std::cout << "Failed to link program" << log << std::endl;
	}
	glValidateProgram(ID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Bind() const {
	glUseProgram(ID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}

int Shader::GetUniformLocation(const char* name) const {
	int location = glGetUniformLocation(ID, name);
	if (location == -1) {
		std::cout << "Invalid uniform location for " << name << std::endl;
	}
	return location;
}

void Shader::Uniform1i(const char* name, int v1) const {
	glUniform1i(GetUniformLocation(name), v1);
}

void Shader::Uniform4f(const char* name, float v1, float v2, float v3, float v4) const {
	glUniform4f(GetUniformLocation(name), v1, v2, v3, v4);
}

void Shader::UniformMatrix4fv(const char* name, float* v1, bool transpose) const {
	glUniformMatrix4fv(GetUniformLocation(name), 1, false, v1);
}