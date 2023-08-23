#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/stb-master/stb_image.h"

unsigned int GFX::Texture::Load(const char* filename)
{
	//unsigned char* buffer{ nullptr };
	//DeserializeImageFile(filename, buffer);

	// Store data in GPU as glTexture
	//glCreateTextures(GL_TEXTURE_2D, 1, &mID);		// Create texture
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

#if 1

	// Load, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	unsigned char* buffer = stbi_load(filename, &mWidth, &mHeight, &mChannels, 0);

#endif

	// Load onto GPU storage
	if (mChannels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if (mChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbinding
	glBindTexture(GL_TEXTURE_2D, 0);

	// Free memory from buffer
	stbi_image_free(buffer);
	//delete[] buffer;

	return mID;
}

void GFX::Texture::DeserializeImageFile(const char* filename, unsigned char* buffer)
{
	std::ifstream ifs{ filename, std::ios_base::binary };

	int dataSize{};

	// Read in header of texture file
	ifs >> mWidth >> mHeight >> mChannels >> dataSize;

	// Allocate memory for buffer
	buffer = new unsigned char[dataSize];

	// Read data from opened file into buffer
	ifs.read(reinterpret_cast<char*>(buffer), dataSize);
}
