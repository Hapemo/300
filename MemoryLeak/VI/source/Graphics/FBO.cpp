/*!*****************************************************************************
\file FBO.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definitions for a Frame Buffer Object (FBO) class
to operate on OpenGL's Frame Buffer Object.
*******************************************************************************/
#include "Graphics/FBO.h"
#include "Logger.h"

/*!*****************************************************************************
\brief
Default constructor for FBOSpec class.
*******************************************************************************/
FBOSpec::FBOSpec() : mSamples(1), mWidth(), mHeight() {}

/*!*****************************************************************************
\brief
Initializes the FBO Class.

\param int _windowWidth
Pixel width of the FBO

\param int _windowHeight
Pixel height of the FBO
*******************************************************************************/
void FBO::Init(int _windowWidth, int _windowHeight) {
	mSpecs.mWidth = _windowWidth == 0 ? 1 : _windowWidth;
	mSpecs.mHeight = _windowHeight == 0 ? 1 : _windowHeight;

	//creating frame buffer
	glCreateFramebuffers(1, &mfboid);
	Bind();

	//create color attachment for the frame buffer
	glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
	glBindTexture(GL_TEXTURE_2D, mColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mSpecs.mWidth, mSpecs.mHeight,
							 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//link the color attachment to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

	//create depth attachment for the frame buffer
	glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, mDepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mSpecs.mWidth, mSpecs.mHeight);

	//link the depth attachment to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment, 0);

	//check if FBO is set up correctly
	if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)) {
		LOG_CRASH("Frame buffer has not been set up correctly.");
		std::exit(EXIT_FAILURE);
	}

	Unbind();
}

/*!*****************************************************************************
\brief
Destructor for FBOSpec class.
*******************************************************************************/
FBO::~FBO()
{
	glDeleteFramebuffers(1, &mfboid);
}
/*!*****************************************************************************
\brief
Deletes the FrameBuffer object. Used when resizing the viewport.
*******************************************************************************/
void FBO::DeleteFBO()
{
	glDeleteFramebuffers(1, &mfboid);
}
/*!*****************************************************************************
\brief
Binds the FBO to the OpenGL context.
*******************************************************************************/
void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mfboid);
}

/*!*****************************************************************************
\brief
Unbinds the FBO from the OpenGL context.
*******************************************************************************/
void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}