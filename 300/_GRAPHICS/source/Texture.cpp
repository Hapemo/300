/*!*****************************************************************************
\file Texture.cpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Class for Texture. Consists of functions to load and deserialize texture assets.
Loaded texture data pre-compressed and stored in the GPU's memory.
*******************************************************************************/

#include "Texture.hpp"

unsigned int GFX::Texture::Load(const char* filename)
{
	unsigned char* buffer{ nullptr };
	int imgSize = DeserializeImageFile(filename, buffer);

	// Store data in GPU as glTexture
	//glCreateTextures(GL_TEXTURE_2D, 1, &mID);		// Create texture
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load onto GPU storage
#if TEST_COMPRESSOR
	//int blockSize = mInternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;
	//int size = blockSize * glm::ceil(mWidth / 4.f) * glm::ceil(mHeight / 4.f);

	glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, imgSize, reinterpret_cast<void*>(buffer));
#else
	if (mChannels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if (mChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
#endif

	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbinding
	glBindTexture(GL_TEXTURE_2D, 0);

	// Free memory from buffer
	delete[] buffer;

	return mID;
}

int GFX::Texture::DeserializeImageFile(const char* filename, unsigned char* &buffer)
{
	std::ifstream ifs{ filename, std::ios_base::binary };

	int dataSize{};
	// Read in header of texture file
	ifs >> mWidth >> mHeight >> mChannels >> mInternalFormat >> dataSize;

	std::string tmp{};
	std::getline(ifs, tmp);

	// Allocate memory for buffer
	buffer = new unsigned char[dataSize];

	// Read data from opened file into buffer
	ifs.read(reinterpret_cast<char*>(buffer), dataSize);

	return dataSize;
}
