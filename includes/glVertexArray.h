#pragma once

#include "glVertexBuffer.h"
#include "glVertexBufferLayout.h"

class GlVertexArray {
public:
	void addBuffer(const GlVertexBuffer& vertexBuffer, const GlVertexBufferLayout& layout);

	void bind() const;
	void unbind() const;

	GlVertexArray();
	~GlVertexArray();

private:
	uint32_t arrayID;
};