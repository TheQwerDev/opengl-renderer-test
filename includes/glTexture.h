#pragma once

class GlTexture {
public:
	void bind(uint32_t textureUnit);

	GlTexture(const char* texturePath, GLenum textureWrapping, GLenum textureFiltering, bool flipTexture = false);
	~GlTexture();

private:
	uint32_t textureID;

	int32_t texWidth;
	int32_t texHeight;
	int32_t nrChannels;
};