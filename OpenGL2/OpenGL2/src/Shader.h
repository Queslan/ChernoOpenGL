#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	unsigned int m_RenderId;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCashe;

	int GetUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);
	ShaderProgramSource ParseShader(const std::string filepath);
	unsigned int CompileShader(unsigned int type, const std::string & source);
};

