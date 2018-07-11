#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

Shader::Shader(const std::string & filepath)
	:m_FilePath(filepath), m_RenderId(0){

	ShaderProgramSource source = ParseShader(filepath);
	m_RenderId = CreateShader(source.vertexSource, source.fragmentSource);

}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RenderId));
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_RenderId));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3) {
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}


int Shader::GetUniformLocation(const std::string & name) {
	if (m_UniformLocationCashe.find(name) != m_UniformLocationCashe.end())
		return m_UniformLocationCashe[name];

	GLCall(int location = glGetUniformLocation(m_RenderId, name.c_str()));
	m_UniformLocationCashe[name] = location;
	return location;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	GLCall(unsigned int program = glCreateProgram()); // Creates a program object
	GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
	GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

	GLCall(glAttachShader(program, vs)); // Attaches a shader object to a program object
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program)); // Links a program object
	GLCall(glValidateProgram(program)); // Validates a program object

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}



ShaderProgramSource Shader::ParseShader(const std::string filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	GLCall(unsigned int id = glCreateShader(type)); // Creates a shader object
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr)); // Replaces the source code in a shader object
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char)); // The way to get array of length length
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << '\n';
		std::cout << message << '\n';
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}