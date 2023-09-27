/**-------------------------------------------------------------------------------------/
 * @file
 *  Fbo.hpp
 * @author
 *  Lee Fu Sheng Roy, 2101440, f.lee@digipen.edu
 * @date
 *  2023/09/06
 * @brief
 *  Implementation of the Fbo class.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#ifndef FBO_HPP
#define FBO_HPP

#include "Common.hpp"
#include <xkeycheck.h>

namespace GFX
{
	class FBO
	{
	public:

		// editorMode: False if running game only
		void Create(int width, int height, bool editorMode = true);

		// Selects which attachments to bind and draw to
		void PrepForDraw();

		unsigned int ReadEntityID(float posX, float posY);

		void Bind()		{ glBindFramebuffer(GL_FRAMEBUFFER, mID); }
		void Unbind()	{ glBindFramebuffer(GL_FRAMEBUFFER, 0); }

		// -- Getter --
		unsigned int GetGameAttachment()		{ return mGameAttachment; }
		unsigned int GetEditorAttachment()		{ return mEditorAttachment; }
		unsigned int GetEntityIDAttachment()	{ return mEntityIDAttachment; }
		int GetWidth()							{ return mWidth; }
		int GetHeight()							{ return mHeight; }

		// -- Setter --
		void Resize(int width, int height);
	
		~FBO()
		{
			glDeleteFramebuffers(1, &mID);
			glDeleteRenderbuffers(1, &mRboID);
			glDeleteTextures(1, &mGameAttachment);
			glDeleteTextures(1, &mEditorAttachment);
			glDeleteTextures(1, &mEntityIDAttachment);
		}

	private:
		unsigned int mID{};						// Framebuffer Object ID
		unsigned int mRboID{};					// Renderbuffer Object ID
		unsigned int mGameAttachment{};			// Color Attachment of game scene
		unsigned int mEditorAttachment{};		// Color Attachment of Editor scene
		unsigned int mEntityIDAttachment{};		// Attachment that contains Entity ID of each corresponding pixel

		int mWidth{};
		int mHeight{};

		// -- Flags --
		bool mEditorMode;		// True: Writes to all attachments
	};
}

#endif	//	FBO_HPP