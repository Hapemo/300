/*!*****************************************************************************
\file FontManager.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 14-10-2022
\brief
This file contains a class FontRenderer, which is a tool for renderering fonts.
*******************************************************************************/
#pragma once
#include <pch.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <Logger.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Graphics/GLShader.h>
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"
/*!*****************************************************************************
\brief
Struct that encapsulates what is needed for rendering a single character.
*******************************************************************************/
struct Character
{
	unsigned int textureID; //ID of the glyph
	Math::Vec2 size;		//size of the glyph
	Math::Vec2 bearing;		//offset from baseline to left/top of glyph
	unsigned int advanceX;	//offset to advance to next glyph
};

/*!*****************************************************************************
\brief
Struct that encapsulates a string.
*******************************************************************************/
struct Paragraph
{
	std::vector<std::string> words;
	std::vector<float> wordWidth;
	Math::Vec2 pos;
	float scale;
	glm::vec4 color;
	float renderWidth;
	float camZoom;
};

/*!*****************************************************************************
\brief
Class that encapsulates a FontRenderer tool.
*******************************************************************************/
class FontRenderer
{
public:
	/*!*****************************************************************************
	\brief
	Default constructor for FontRenderer class.
	*******************************************************************************/
	FontRenderer() : FontRenderer("BPtypewrite.ttf") {}
	/*!*****************************************************************************
	\brief
	Non-default constructor for FontRenderer class.

	\param const std::string& fontfile
	String containing name of the font file.
	*******************************************************************************/
	FontRenderer(const std::string& _fontfile);
	/*!*****************************************************************************
	\brief
	Adds a paragraph to the font renderer to be rendered.

	\param const std::string& _text
	String containing text to be rendered.

	\param const Math::Vec2& _pos
	Position to render the string.

	\param float _scale
	Scale of the font.

	\param const Math::Vec3& _color
	Color of the font.
	*******************************************************************************/
	void AddParagraph(const std::string& _text, const Math::Vec2& _pos, float _scale, const glm::vec4& _color, int layer, float _width, float _camZoom);
	/*!*****************************************************************************
	\brief
	Renders all paragraphs stored in mParagraphs.
	*******************************************************************************/
	void DrawParagraphs(int _layer);

	/*!*****************************************************************************
	\brief
	Clears paragraph vector.
	*******************************************************************************/
	void Clear();

	/*!*****************************************************************************
	\brief
	Checks if the manager is initialized properly.
	
	\return bool
	true if initialized. false otherwise.
	*******************************************************************************/
	bool IsInitialized() { return mInitialized; }

	/*!*****************************************************************************
	\brief
	Passes the window dimension pointer to the camera object.

	\param int* _windowWidth
	Pointer to the window width

	\param int* _windowHeight
	Pointer to the window height
	*******************************************************************************/
	void SetWindowPtr(int* _windowWidth, int* _windowHeight) 
	{ 
		mWindowWidth = *_windowWidth; 
		mWindowHeight = *_windowHeight;
	}

	/*!*****************************************************************************
	\brief
	Gets the number of lines for each text component.

	\param const std::string& _text
	String containing text to be rendered.

	\param const Math::Vec2& _pos
	Position to render the string.

	\param float _scale
	Scale of the font.

	\param const Math::Vec3& _color
	Color of the font.

	\param float
	Width of the sprite that the text is rendered on.

	\param float
	Camera's zoom
	
	\return 
	the number of lines.
	*******************************************************************************/
	int GetLineCount(const std::string& text, const Math::Vec2& _pos, float scale, const glm::vec4& color, float _width, float camZoom);

private:
	/*!*****************************************************************************
	\brief
	Initializes the FontRenderer

	\param const std::string& fontfile
	String containing name of the font file.
	*******************************************************************************/
	bool Init(const std::string& _fontfile);
	GLuint mVAO, mVBO;
	std::unordered_map<char, Character> mGlyphs;
	std::map<int, std::vector<Paragraph>> mParagraphs;
	GLShader mFontProgram;
	GLint mTextColorLocation, mMatrixLocation, mZValueLocation;
	bool mInitialized;
	int mWindowWidth, mWindowHeight;
	float mMaxYSize;
public:
	static float magicNumber;
};