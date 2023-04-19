#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glIndexBuffer.h"

void GlIndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void GlIndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GlIndexBuffer::GlIndexBuffer(const uint32_t* data, uint32_t count)
	: count(count)
{
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

GlIndexBuffer::~GlIndexBuffer()
{
	glDeleteBuffers(1, &bufferID);
}
