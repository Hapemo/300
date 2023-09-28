/*!*****************************************************************************
\file Texture.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Class for Texture. Consists of functions to load and deserialize texture assets.
Loaded texture data pre-compressed and stored in the GPU's memory.
*******************************************************************************/

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Common.hpp"

#define TEST_COMPRESSOR 1

namespace GFX
{
	class Texture
	{
	public:
		
/*!*****************************************************************************
Loads and deserialize the texture asset and store the data in GPU's memory
*******************************************************************************/
		unsigned int Load(const char* filename);

/*!*****************************************************************************
Returns the ID of the texture
*******************************************************************************/
		unsigned int ID() { return mID; }

/*!*****************************************************************************
Returns the width of the texture
*******************************************************************************/
		int Width() { return mWidth; }

/*!*****************************************************************************
Returns the height of the texture
*******************************************************************************/
		int Height() { return mHeight; }

/*!*****************************************************************************
Returns the number of channels of the texture
*******************************************************************************/
		int Channels() { return mChannels; }
		
	private:
		unsigned int mID{};		// Texture handle in GPU

		// -- Stats --
		int mWidth{};
		int mHeight{};
		int mChannels{};
		int mInternalFormat{};

/*!*****************************************************************************
Deserialize the texture file and places the pixel data onto the buffer
*******************************************************************************/
		int DeserializeImageFile(const char* filename, unsigned char* &buffer);
	};
}


#endif // !TEXTURE_HPP

