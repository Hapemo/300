/**-------------------------------------------------------------------------------------/
 * @file
 *  Fbo.cpp
 * @author
 *  Lee Fu Sheng Roy, 2101440, f.lee@digipen.edu
 * @date
 *  2023/09/06
 * @brief
 *  Function definition of the Fbo Class
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#include "Fbo.hpp"

void GFX::FBO::Create(int width, int height, bool editorMode)
{
	mWidth = width;
	mHeight = height;
	mEditorMode = editorMode;

	// Create and bind framebuffer
	glGenFramebuffers(1, &mID);
	glBindFramebuffer(GL_FRAMEBUFFER, mID);

	// Creating Game Attachment
	glGenTextures(1, &mGameAttachment);
	glBindTexture(GL_TEXTURE_2D, mGameAttachment);
	// Specifying texture size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Creating Editor Attachment
	glGenTextures(1, &mEditorAttachment);
	glBindTexture(GL_TEXTURE_2D, mEditorAttachment);
	// Specifying texture size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Creating Entity ID Attachment
	glGenTextures(1, &mEntityIDAttachment);
	glBindTexture(GL_TEXTURE_2D, mEntityIDAttachment);
	// Specifying size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach all Attachments to currently bound framebuffer
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT0, mGameAttachment, 0);			// 0
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT1, mEditorAttachment, 0);			// 1
	glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT2, mEntityIDAttachment, 0);		// 2

	// Creating renderbuffer object for depth testing
	glCreateRenderbuffers(1, &mRboID);
	glNamedRenderbufferStorage(mRboID, GL_DEPTH_COMPONENT, width, height);
	// Attaching renderbuffer object to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRboID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Framebuffer creation failed");
	}
}

void GFX::FBO::PrepForDraw()
{
	// bind framebuffer as buffer to render to
	glBindFramebuffer(GL_FRAMEBUFFER, mID);

	if (mEditorMode)	// Need to render to both color attachments and Entity ID attachment
	{
		// Game attachment, Editor Attachment, Entity ID Attachment
		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
	}
	// Renders to Game Attachment by default

	glClearColor(.4f, .5f, .6f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int GFX::FBO::ReadEntityID(int posX, int posY)
{
	int mapped_x = posX + (mWidth / 2);
	int mapped_y = posY + (mHeight / 2);
	if (mapped_x >= mWidth || mapped_x < 0 || mapped_y >= mHeight || mapped_y < 0)
		return 0xFFFFFFFF;
	GLsizei bufsize = mWidth * mHeight;
	unsigned int* p_eid = new unsigned int[bufsize];	// allocate buffer

	glBindTexture(GL_TEXTURE_2D, mEntityIDAttachment);	// bind texture
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, p_eid);
	
	unsigned int eid = p_eid[mapped_y * mWidth + mapped_x];		// Retrieve entity ID

	delete[] p_eid;		// free buffer
	glBindTexture(GL_TEXTURE_2D, 0);	// unbind texture

	return eid;
}

void GFX::FBO::Resize(int width, int height)
{
	mWidth = width;
	mHeight = height;

	// Bind Game Attachment to be resized
	glBindTexture(GL_TEXTURE_2D, mGameAttachment);
	// Specifying new attachment size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	// Unbind 
	glBindTexture(GL_TEXTURE_2D, 0);

	// Bind Editor Attachment to be resized
	glBindTexture(GL_TEXTURE_2D, mEditorAttachment);
	// Specifying new attachment size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	// Unbind 
	glBindTexture(GL_TEXTURE_2D, 0);

	// Bind Entity ID Attachment to be resized
	glBindTexture(GL_TEXTURE_2D, mEntityIDAttachment);
	// Specifying new attachment size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	// Unbind 
	glBindTexture(GL_TEXTURE_2D, 0);

	glNamedRenderbufferStorage(mRboID, GL_DEPTH_COMPONENT, width, height);
}
