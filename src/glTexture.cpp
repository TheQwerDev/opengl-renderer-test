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

GlTexture::GlTexture(const char* texturePath, GLenum textureWrapping, GLenum textureFiltering, bool flipTexture)
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

		std::cout << "[OpenGL] WARNING: Failed to load texture \"" << texturePath << "\"!\n";
		log << "[OpenGL] WARNING: Failed to load texture \"" << texturePath << "\"!\n";

		log.close();
	}

	stbi_image_free(data);
}

GlTexture::~GlTexture()
{
	glDeleteTextures(1, &textureID);
}