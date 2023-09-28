/**-------------------------------------------------------------------------------------/
 * @file
 *  font.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Camera.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

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
		unsigned Load(const char* fontFile);
		unsigned GetID() { return mID; }

	private:
		unsigned int mID;	// Texture handle of font atlas

		int mAtlasWidth;
		int mAtlasHeight;

		std::map<char, Character> mChars;	// Map of all supported characters and its render data

		// -- Private Helper --
		void DeserializeFontFile(const char* fontFile, unsigned char*& buffer);
	};
}

#endif	// FONT_HPP