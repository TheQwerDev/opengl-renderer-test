#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class GlShader {
public:
	void use();
	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int32_t value) const;
	void setFloat(const char* name, float value) const;

	void setVec2(const char* name, const glm::vec2& value) const;
	void setVec2(const char* name, float x, float y) const;

	void setVec3(const char* name, const glm::vec3& value) const;
	void setVec3(const char* name, float x, float y, float z) const;

	void setVec4(const char* name, const glm::vec4& value) const;
	void setVec4(const char* name, float x, float y, float z, float w) const;

	void setMat2(const char* name, const glm::mat2& mat) const;
	void setMat3(const char* name, const glm::mat3& mat) const;
	void setMat4(const char* name, const glm::mat4 &mat) const;
	
	GlShader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~GlShader();

private:
	void compileShader(uint32_t  &shaderObject, const char* shaderSource, GLenum shaderType);
	void shaderCompilationCheck(uint32_t shader, const char* shaderPath);
	void shaderProgramLinkCheck(uint32_t shaderProgram);
	char* readShaderSource(const char* filePath);

	uint32_t programID;
};