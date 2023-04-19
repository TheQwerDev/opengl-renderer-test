#include <iostream>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glShader.h"

GlShader::GlShader(const char* vertexShaderPath, const char* fragmentShaderPath) 
{
	uint32_t vertexShader, fragmentShader;

	compileShader(vertexShader, vertexShaderPath, GL_VERTEX_SHADER);
	compileShader(fragmentShader, fragmentShaderPath, GL_FRAGMENT_SHADER);

	programID = glCreateProgram();

	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);

	glLinkProgram(programID);

	shaderProgramLinkCheck(programID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GlShader::~GlShader()
{
	glDeleteProgram(programID);
}

void GlShader::compileShader(uint32_t &shaderObject, const char* shaderPath, GLenum shaderType)
{
	shaderObject = glCreateShader(shaderType);
	const char* shaderSource = readShaderSource(shaderPath);

	glShaderSource(shaderObject, 1, &shaderSource, NULL);
	glCompileShader(shaderObject);

	shaderCompilationCheck(shaderObject, shaderPath);
}

void GlShader::shaderCompilationCheck(uint32_t shader, const char* shaderPath)
{
	int32_t success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		std::ofstream log("log.txt", std::ios_base::app);

		std::cout << infoLog;
		log << infoLog;

		std::cout << "[OpenGL] ERROR: Shader compilation failed for shader \"" << shaderPath << "\"!\n\n";
		log << "[OpenGL] ERROR: Shader compilation failed for shader \"" << shaderPath << "\"!\n\n";

		log.close();
	}
}

void GlShader::shaderProgramLinkCheck(uint32_t shaderProgram)
{
	int32_t success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		std::ofstream log("log.txt", std::ios_base::app);

		std::cout << infoLog;
		log << infoLog;

		std::cout << "[OpenGL] ERROR: Shader program failed!\n\n";
		log << "[OpenGL] ERROR: Shader program failed!\n\n";

		log.close();
	}
}

char* GlShader::readShaderSource(const char* filePath)
{
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		std::ofstream log("log.txt", std::ios_base::app);

		std::cout << "ERROR: Failed to read shader source \"" << filePath << "\"!\n\n";
		log << "ERROR: Failed to read shader source \"" << filePath << "\"!\n\n";

		log.close();
	}

	size_t fileSize = (size_t)file.tellg();

	char* buffer = new char[fileSize + 1];
	buffer[fileSize] = '\0';

	file.seekg(0);
	file.read(buffer, fileSize);

	file.close();

	return buffer;
}

void GlShader::use()
{
	glUseProgram(programID);
}

void GlShader::setBool(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name), (int32_t)value);
}

void GlShader::setInt(const char* name, int32_t value) const
{
	glUniform1i(glGetUniformLocation(programID, name), value);
}

void GlShader::setFloat(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(programID, name), value);
}

void GlShader::setVec2(const char* name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(programID, name), 1, &value[0]);
}

void GlShader::setVec2(const char* name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(programID, name), x, y);
}

void GlShader::setVec3(const char* name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(programID, name), 1, &value[0]);
}

void GlShader::setVec3(const char* name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(programID, name), x, y, z);
}

void GlShader::setVec4(const char* name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(programID, name), 1, &value[0]);
}

void GlShader::setVec4(const char* name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(programID, name), x, y, z, w);
}

void GlShader::setMat2(const char* name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &mat[0][0]);
}

void GlShader::setMat3(const char* name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &mat[0][0]);
}

void GlShader::setMat4(const char* name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &mat[0][0]);
}