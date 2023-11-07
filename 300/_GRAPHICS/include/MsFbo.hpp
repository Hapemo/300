#ifndef MS_FBO_HPP
#define MS_FBO_HPP

#include "Common.hpp"

namespace GFX
{
	class MsFBO
	{
	public:
		void Create(int width, int height, bool resize = false);

		// Clears the attachments, preparing for incoming draws
		void PrepForDraw();

		// Copies attachment data from Multisample FBO to destFBO
		void BlitFramebuffer(unsigned int destFBO);

		void Bind();
		void Unbind();

		void Resize(int width, int height);

		unsigned int GetColorAttachment()			{ return mColorAttachment; }
		unsigned int GetEntityIDAttachment()		{ return mEntityIDAttachment; }
		unsigned int GetBrightColorsAttachment()	{ return mBrightColorsAttachment; }

		int GetWidth()	{ return mWidth; }
		int GetHeight() { return mHeight; }

		~MsFBO()
		{
			glDeleteFramebuffers(1, &mID);
			glDeleteRenderbuffers(1, &mRboID);
			glDeleteTextures(1, &mColorAttachment);
			glDeleteTextures(1, &mEntityIDAttachment);
			glDeleteTextures(1, &mBrightColorsAttachment);
		}

	private:
		unsigned int mID{};							// Framebuffer Object ID
		unsigned int mRboID{};						// Renderbuffer Object ID
		unsigned int mColorAttachment{};			// Color Attachment of scene
		unsigned int mEntityIDAttachment{};			// Attachment that contains Entity ID of each corresponding pixel
		unsigned int mBrightColorsAttachment;		// Bloom attachment

		int mWidth{};
		int mHeight{};
		int mSamples{ 16 };
	};
}

#endif // !MS_FBO_HPP