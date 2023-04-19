#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glVertexBuffer.h"

void GlVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
}

void GlVertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GlVertexBuffer::GlVertexBuffer(const void* data, uint32_t size)
{
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

GlVertexBuffer::~GlVertexBuffer()
{
	glDeleteBuffers(1, &bufferID);
}
