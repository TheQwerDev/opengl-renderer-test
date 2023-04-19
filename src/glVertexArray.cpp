#pragma warning(disable:4312)

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glVertexArray.h"

GlVertexArray::GlVertexArray()
{
	glGenVertexArrays(1, &arrayID);
	glBindVertexArray(arrayID);
}

GlVertexArray::~GlVertexArray()
{
	glDeleteVertexArrays(1, &arrayID);
}

void GlVertexArray::bind() const
{
	glBindVertexArray(arrayID);
}

void GlVertexArray::unbind() const
{
	glBindVertexArray(0);
}

void GlVertexArray::addBuffer(const GlVertexBuffer& vertexBuffer, const GlVertexBufferLayout& layout)
{
	bind();
	vertexBuffer.bind();

	const auto& elements = layout.getElements();

	uint32_t offset = 0;
	for (uint32_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * GlVertexBufferElement::getSizeOfType(element.type);
	}

	vertexBuffer.unbind();
	unbind();
}