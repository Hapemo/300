#include "Font.hpp"
#include <fstream>
#include <string>

unsigned int GFX::Font::Load(const char* fontFile)
{
	unsigned char* buffer{ nullptr };
	DeserializeFontFile(fontFile, buffer);

	// Store data in GPU as glTexture
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Delete buffer allocated in deserialize function
	delete[] buffer;

	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbinding
	glBindTexture(GL_TEXTURE_2D, 0);

	return mID;
}

void GFX::Font::DeserializeFontFile(const char* fontFile, unsigned char* &buffer)
{
	// Create custom file
	std::ifstream infile{ fontFile, std::ios_base::binary };
	if (!infile)
		throw std::runtime_error("Font file could not be opened!");

	int dataSize{}, numOfChars;
	infile >> mAtlasWidth >> mAtlasHeight >> dataSize >> numOfChars;

	std::string strTemp{};
	std::getline(infile, strTemp);

	// Read in each character's data
	for (int i = 0; i < numOfChars; ++i)
	{
		int ch{};
		Character charData;

		infile >> ch;
		infile >> charData.mAdvance.x >> charData.mAdvance.y;
		infile >> charData.mSize.x >> charData.mSize.y;
		infile >> charData.mLeft >> charData.mTop >> charData.mTextCoord;

		// Read finish the remaining line till the end of line
		std::getline(infile, strTemp);

		// Store character and its render data
		mChars[(char)ch] = charData;
	}

	// Allocate memory for buffer
	buffer = new unsigned char[dataSize];

	// Read data from opened file into buffer
	infile.read(reinterpret_cast<char*>(buffer), dataSize);
}
