/*!*****************************************************************************
\file Font.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Font class implementation. Consists the loading and deserialization of the font
files
*******************************************************************************/

#ifndef FONT_HPP
#define FONT_HPP

#include <glm/glm.hpp>
#include <map>
#include "Common.hpp"


namespace GFX
{
	struct Character
	{
		glm::vec2	mAdvance;	// Offset to advance to the next glyph
		glm::vec2	mSize;		// width and height of bitmap
		float		mLeft;		// bitmap_left
		float		mTop;		// bitmap_top

		float		mTextCoord;	// x offset of glyph in texture coordinates
	};

	class Font
	{
	public:
		
/*!*****************************************************************************
Loads the font data into GPU memory
*******************************************************************************/
		unsigned Load(const char* fontFile);
		
/*!*****************************************************************************
returns the ID of the font texture atlas
*******************************************************************************/
		unsigned GetID() { return mID; }

	private:
		unsigned int mID;	// Texture handle of font atlas

		int mAtlasWidth;
		int mAtlasHeight;

		std::map<char, Character> mChars;	// Map of all supported characters and its render data

/*!*****************************************************************************
Deserializes the font file and stores the data in the given buffer
*******************************************************************************/
		void DeserializeFontFile(const char* fontFile, unsigned char*& buffer);
	};
}

#endif	// FONT_HPP