#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <vector>
#include <assert.h>

struct GlVertexBufferElement
{
	uint32_t type;
	uint32_t count;
	GLboolean normalized;

	static uint32_t getSizeOfType(uint32_t type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:   return 4;
			case GL_UNSIGNED_BYTE:  return 1;
		}
		return 0;
	}
};

class GlVertexBufferLayout
{
public:
	GlVertexBufferLayout()
		: stride(0) {};

	template<typename T>
	void push(uint32_t count)
	{
		assert(false);
	}

	template<>
	void push<float>(uint32_t count)
	{
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * GlVertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<uint32_t>(uint32_t count)
	{
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * GlVertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(uint32_t count)
	{
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * GlVertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	const std::vector<GlVertexBufferElement>& getElements() const { return elements; }
	uint32_t getStride() const { return stride; }

private:
	std::vector<GlVertexBufferElement> elements;
	uint32_t stride;

};