/*!*****************************************************************************
\file Fbo.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
FBO class implementation. Creates the framebuffer object. Consists of functions
to handle resizing of framebuffers and object picking
*******************************************************************************/

#ifndef FBO_HPP
#define FBO_HPP

#include "Common.hpp"
#include <xkeycheck.h>

namespace GFX
{
	class FBO
	{
	public:

/*!*****************************************************************************
Creates the framebuffer object and the colorAttachment, Entity ID attachment
*******************************************************************************/
		void Create(int width, int height, bool editorMode = true);

/*!*****************************************************************************
Prepares the Framebuffer to be drawn by selecting the attachments to be bound
and rendered to
*******************************************************************************/
		void PrepForDraw();

/*!*****************************************************************************
Reads the entity ID in the entityIDAttachment at the posX and posY. posX and 
posY are normalized to be from [0.f, 1.f]
*******************************************************************************/
		unsigned int ReadEntityID(float posX, float posY);

/*!*****************************************************************************
Binds the framebuffer to be rendered to
*******************************************************************************/
		void Bind()		{ glBindFramebuffer(GL_FRAMEBUFFER, mID); }

/*!*****************************************************************************
Unbinds the framebuffer
*******************************************************************************/
		void Unbind()	{ glBindFramebuffer(GL_FRAMEBUFFER, 0); }

		
/*!*****************************************************************************
Returns the ID of the color attachment
*******************************************************************************/
		unsigned int GetColorAttachment()		{ return mColorAttachment; }
		
/*!*****************************************************************************
Returns the ID of the entity ID attachment
*******************************************************************************/
		unsigned int GetEntityIDAttachment()	{ return mEntityIDAttachment; }
		
/*!*****************************************************************************
Returns the width of the framebuffer
*******************************************************************************/
		int GetWidth()							{ return mWidth; }
		
/*!*****************************************************************************
Returns the height of the framebuffer
*******************************************************************************/
		int GetHeight()							{ return mHeight; }

/*!*****************************************************************************
Resizes the size of the framebuffer
*******************************************************************************/
		void Resize(int width, int height);
	
/*!*****************************************************************************
Destructor of the FBO class. Deletes the resources allocated on the GPU
*******************************************************************************/
		~FBO()
		{
			glDeleteFramebuffers(1, &mID);
			glDeleteRenderbuffers(1, &mRboID);
			glDeleteTextures(1, &mColorAttachment);
			glDeleteTextures(1, &mEntityIDAttachment);
		}

	private:
		unsigned int mID{};						// Framebuffer Object ID
		unsigned int mRboID{};					// Renderbuffer Object ID
		unsigned int mColorAttachment{};		// Color Attachment of scene
		unsigned int mEntityIDAttachment{};		// Attachment that contains Entity ID of each corresponding pixel

		int mWidth{};
		int mHeight{};

		// -- Flags --
		bool mEditorMode;		// True: Writes to all attachments
	};
}

#endif	//	FBO_HPP