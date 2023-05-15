/*!*****************************************************************************
\file FontManager.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 14-10-2022
\brief
This file contains a class FontRenderer, which is a tool for renderering fonts.
*******************************************************************************/
#include <FontManager.h>
#include <sstream>
#include <RenderProps.h>
float FontRenderer::magicNumber = 0.7f;
/*!*****************************************************************************
\brief
Default constructor for FontRenderer class.
*******************************************************************************/
FontRenderer::FontRenderer(const std::string& fontfile) 
: mFontProgram("shaders/font.vert", "shaders/font.frag"), mVAO(), mVBO(), 
mWindowWidth(0), mWindowHeight(0)
{
    mFontProgram.CompileLinkShaders();
    mFontProgram.Validate();
    mMatrixLocation = glGetUniformLocation(mFontProgram.GetID(), "projection");
    mTextColorLocation = glGetUniformLocation(mFontProgram.GetID(), "textColor");
    mZValueLocation = glGetUniformLocation(mFontProgram.GetID(), "zValue");
    mMaxYSize = 0;
    mInitialized = Init(fontfile);
}
/*!*****************************************************************************
\brief
Initializes the FontRenderer

\param const std::string& fontfile
String containing name of the font file.
*******************************************************************************/
bool FontRenderer::Init(const std::string& _fontfile)
{
    //free type
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        LOG_ERROR("ERROR::FREETYPE: failed to initialize FreeType Library\n");
        return false;
    }
    std::string filepath = "../resources/Fonts/" + _fontfile;

    FT_Face face;
    if (FT_New_Face(ft, filepath.c_str(), 0, &face))
    {
        LOG_ERROR("ERROR::FREETYPE: Failed to load font\n");
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        LOG_ERROR("ERROR::FREETYPE: Failed to load font\n");
        return false;
    }

    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; ++c)
    {
        // load character glyph into opengl
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            LOG_ERROR("ERROR::FREETYTPE: Failed to load Glyph\n");
            return false;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // generate texture for the glyph
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // store character for later use
        Character character = {
            texture,
            Math::Vec2((float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows),
            Math::Vec2((float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        mGlyphs.insert(std::pair<char, Character>(c, character));
        mMaxYSize = (float)face->glyph->bitmap.rows > mMaxYSize ? (float)face->glyph->bitmap.rows : mMaxYSize;
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    //create buffers for characters
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
 }
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
void FontRenderer::AddParagraph(const std::string& text, const Math::Vec2& _pos, float scale, const glm::vec4& color, int layer, float _width, float camZoom)
{
    if (!mInitialized) return;
    std::vector<std::string> strings;
    std::istringstream iss(text);
    std::string intermediate;
    while (std::getline(iss, intermediate, ' ')) {
        strings.push_back(intermediate);
    }
    std::vector<float> wordWidth;
    for (std::string& str : strings)
    {
        float width{};
        if (str != strings.back())
        {
            str += " ";
            width += 20 * scale;
        }
        for (char ch : str)
        {
          if (ch == '$') {
                width += 1000;
                continue;
            }
            width += 20 * scale;
        }
        wordWidth.push_back(width);
    }

    mParagraphs[layer].push_back(Paragraph(strings, wordWidth, _pos, scale, color, _width, camZoom));
}
/*!*****************************************************************************
\brief
Renders all paragraphs stored in mParagraphs.
*******************************************************************************/
void FontRenderer::DrawParagraphs(int _layer)
{
    if (!mInitialized) return;
    if (mParagraphs.find(_layer) == mParagraphs.end()) return;
    glm::mat4 _projection = glm::ortho(0.0f, (float) mWindowWidth, 0.0f, (float)mWindowHeight);

    float layer = (_layer + 1 - (MAX_LAYERS_PER_SCENE * MAX_SCENE_LAYERS) / 2.f) / ((MAX_LAYERS_PER_SCENE * MAX_SCENE_LAYERS) / 2.f);
    layer = layer > 1.f ? 1.f : layer;

    for (Paragraph& para : mParagraphs[_layer])
    {
        float currWidth{};
        Math::Vec2 pos = para.pos;
        float initialX = pos.x;
        mFontProgram.Bind();
        glUniformMatrix4fv(mMatrixLocation, 1, GL_FALSE, glm::value_ptr(_projection));
        glUniform4f(mTextColorLocation, para.color.r, para.color.g, para.color.b, para.color.a);
        glUniform1f(mZValueLocation, layer);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(mVAO);
        int lines = 1;

        // iterate through characters in paragraph
        for (size_t i = 0; i < para.words.size(); ++i)
        {
            currWidth += para.wordWidth[i];
            std::string wordddd = para.words[i];
            if (i && currWidth > para.renderWidth * magicNumber / para.camZoom)
            {
                currWidth -= 20 * para.scale;
                if (currWidth > para.renderWidth * magicNumber / para.camZoom)
                {
                    ++lines;
                    pos.x = initialX;
                    pos.y -= (mMaxYSize)*para.scale * 1.1f;
                    currWidth = para.wordWidth[i];
                }
                else
                    currWidth += 20 * para.scale;

            }
            for (auto itr = para.words[i].begin(); itr != para.words[i].end(); ++itr)
            {
                if (*itr == '$') continue;
                Character ch = mGlyphs[*itr];
                float w = ch.size.x * para.scale;
                float h = ch.size.y * para.scale;
                
                float xpos = pos.x + ch.bearing.x * para.scale;
                float ypos = pos.y - (ch.size.y - ch.bearing.y) * para.scale;

                // update VBO for each character
                float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }
                };
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.textureID);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, mVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                pos.x += (ch.advanceX >> 6) * para.scale; // bitshift by 6 to get value in pixels (2^6 = 64)

            }
        }


        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        mFontProgram.Unbind();
    }
}
/*!*****************************************************************************
\brief
Clears paragraph vector.
*******************************************************************************/
void FontRenderer::Clear()
{
    //clear paragraph for next frame
    mParagraphs.clear();
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
int FontRenderer::GetLineCount(const std::string& text, const Math::Vec2& _pos, float scale, const glm::vec4& color, float _width, float camZoom)
{
    if (!mInitialized) return 0;
    std::vector<std::string> strings;
    std::istringstream iss(text);
    std::string intermediate;
    while (std::getline(iss, intermediate, ' ')) {
        strings.push_back(intermediate);
    }
    std::vector<float> wordWidth;
    for (std::string& str : strings)
    {
        float width{};
        if (str != strings.back())
        {
            str += " ";
            width += 20 * scale;
        }
        for (char ch : str)
        {
            if (ch == '$')
                width += 1000;
            width += 20 * scale;
        }
        wordWidth.push_back(width);
    }
    
    Paragraph para = Paragraph(strings, wordWidth, _pos, scale, color, _width, camZoom);

    float currWidth{};
    int lines = 1;
    for (size_t i = 0; i < para.words.size(); ++i)
    {
        currWidth += para.wordWidth[i];
        if (i && currWidth > para.renderWidth * magicNumber / para.camZoom)
        {
            currWidth -= 20 * scale;
            if (currWidth > para.renderWidth * magicNumber / para.camZoom)
            {
                ++lines;
                currWidth = para.wordWidth[i];
            }
            else
                currWidth += 20 * scale;
        }
    }

    return lines;

}