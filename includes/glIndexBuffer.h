#pragma once

class GlIndexBuffer {
public:
	void bind() const;
	void unbind() const;

	GlIndexBuffer(const uint32_t* data, uint32_t count);
	~GlIndexBuffer();

	uint32_t getCount() const { return count; }

private:
	uint32_t bufferID;
	uint32_t count;
};