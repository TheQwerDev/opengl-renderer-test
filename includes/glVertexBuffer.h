#pragma once

class GlVertexBuffer {
public:
	void bind() const;
	void unbind() const;

	GlVertexBuffer(const void* data, uint32_t size);
	~GlVertexBuffer();

private:
	uint32_t bufferID;
};