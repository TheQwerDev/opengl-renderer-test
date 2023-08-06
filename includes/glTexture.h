#pragma once

enum TextureType
{
	NONE,
	DIFFUSE,
	SPECULAR,
	NORMAL
};

class GlTexture {
public:
	void bind(uint32_t textureUnit);
	TextureType getType() const;

	GlTexture(const char* texturePath, GLenum textureWrapping, GLenum textureFiltering, TextureType type = NONE, bool flipTexture = false); //load texture from image
	GlTexture(float r, float g, float b, TextureType type = NONE); //create solid color
	~GlTexture();

private:
	uint32_t textureID;

	int32_t texWidth;
	int32_t texHeight;
	int32_t nrChannels;

	TextureType type;
};