#include <iostream>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void GlTexture::bind(uint32_t textureUnit)
{	
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

TextureType GlTexture::getType() const
{
	return type;
}

GlTexture::GlTexture(const char* texturePath, GLenum textureWrapping, GLenum textureFiltering, TextureType type, bool flipTexture) 
	: type(type), textureID(0), texWidth(0), texHeight(0), nrChannels(0)
{
	//when flipTexture is false we actually want to flip the image because of
	//opengl's texture coordinates (0.0 on the bottom side of the image, 
	//when stb_image has 0.0 on the top side of the image),
	//when flipTexture is true we don't flip the image,
	//because opengl would display it upside-down anyway
	stbi_set_flip_vertically_on_load(!flipTexture);
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapping);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFiltering);

	unsigned char* data = stbi_load(texturePath, &texWidth, &texHeight, &nrChannels, 0);

	if (data)
	{
		GLenum format;
		if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		else //if (nrChannels == 4)
		{
			format = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::ofstream log("log.txt", std::ios_base::app);

		std::cout << "[OpenGL] ERROR: Failed to load texture \"" << texturePath << "\"!\n";
		log << "[OpenGL] ERROR: Failed to load texture \"" << texturePath << "\"!\n";

		log.close();
	}

	stbi_image_free(data);
}

GlTexture::GlTexture(float r, float g, float b, TextureType type)
	: type(type), textureID(0), texWidth(0), texHeight(0), nrChannels(0)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	unsigned char* data = new unsigned char[4 * sizeof(unsigned char)];
	data[0] = (unsigned char)(r * 255.0f);
	data[1] = (unsigned char)(g * 255.0f);
	data[2] = (unsigned char)(b * 255.0f);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	delete[] data;
}

GlTexture::~GlTexture()
{
	glDeleteTextures(1, &textureID);
}