#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Common.hpp"

#define TEST_COMPRESSOR 1

namespace GFX
{
	class Texture
	{
	public:
		unsigned int Load(const char* filename);

		// -- Getter --
		unsigned int ID() { return mID; }
		int Width() { return mWidth; }
		int Height() { return mHeight; }
		int Channels() { return mChannels; }
		
	private:
		unsigned int mID{};		// Texture handle in GPU

		// -- Stats --
		int mWidth{};
		int mHeight{};
		int mChannels{};
		int mInternalFormat{};

		int DeserializeImageFile(const char* filename, unsigned char* &buffer);
	};
}


#endif // !TEXTURE_HPP

