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

		unsigned int GetFragPosAttachment()			{ return mGFragPosAttachment; }
		unsigned int GetNormalAttachment()			{ return mGNormalAttachment; }
		unsigned int GetAlbedoSpecAttachment()		{ return mGAlbedoSpecAttachment; }
		unsigned int GetEmissionAttachment()		{ return mGEmissionAttachment; }

		int GetWidth()	{ return mWidth; }
		int GetHeight() { return mHeight; }

		~MsFBO()
		{
			glDeleteFramebuffers(1, &mID);
			glDeleteRenderbuffers(1, &mRboID);
			glDeleteTextures(1, &mColorAttachment);
			glDeleteTextures(1, &mEntityIDAttachment);
			glDeleteTextures(1, &mBrightColorsAttachment);
			glDeleteTextures(1, &mGFragPosAttachment);
			glDeleteTextures(1, &mGNormalAttachment);
			glDeleteTextures(1, &mGAlbedoSpecAttachment);
			glDeleteTextures(1, &mGEmissionAttachment);
		}

	private:
		unsigned int mID{};							// Framebuffer Object ID
		unsigned int mRboID{};						// Renderbuffer Object ID
		unsigned int mColorAttachment{};			// Color Attachment of scene
		unsigned int mEntityIDAttachment{};			// Attachment that contains Entity ID of each corresponding pixel
		unsigned int mBrightColorsAttachment;		// Bloom attachment

		unsigned int mGFragPosAttachment{};			// G-buffer fragment position attachment for deferred lighting
		unsigned int mGNormalAttachment{};			// G-buffer normal attachment for deferred lighting
		unsigned int mGAlbedoSpecAttachment{};		// G-buffer Albedo-specular attachment for deferred lighting
		unsigned int mGEmissionAttachment{};		// G-buffer emission attachment for deferred lighting

		int mWidth{};
		int mHeight{};
		int mSamples{ 16 };

		void CreateAttachment(unsigned& attachment, GLenum internalFormat);
	};

	class IntermediateFBO
	{
	public:
		void Create(int width, int height, bool resize = false);

		// Clears the attachments, preparing for incoming draws
		void PrepForDraw();

		// Copies attachment data from Multisample FBO to destFBO
		void BlitFramebuffer(unsigned int destFBO);

		void Bind()		{ glBindFramebuffer(GL_FRAMEBUFFER, mID); }
		void Unbind()	{ glBindFramebuffer(GL_FRAMEBUFFER, 0); }

		void Resize(int width, int height);

		void Clear();
		unsigned GetID() { return mID; }

		unsigned int GetColorAttachment()			{ return mColorAttachment; }
		unsigned int GetEntityIDAttachment()		{ return mEntityIDAttachment; }
		unsigned int GetBrightColorsAttachment()	{ return mBrightColorsAttachment; }

		unsigned int GetFragPosAttachment()			{ return mGFragPosAttachment; }
		unsigned int GetNormalAttachment()			{ return mGNormalAttachment; }
		unsigned int GetAlbedoSpecAttachment()		{ return mGAlbedoSpecAttachment; }
		unsigned int GetEmissionAttachment()		{ return mGEmissionAttachment; }

		int GetWidth()	{ return mWidth; }
		int GetHeight() { return mHeight; }

		~IntermediateFBO()
		{
			glDeleteFramebuffers(1, &mID);
			glDeleteRenderbuffers(1, &mRboID);
			glDeleteTextures(1, &mColorAttachment);
			glDeleteTextures(1, &mEntityIDAttachment);
			glDeleteTextures(1, &mBrightColorsAttachment);
			glDeleteTextures(1, &mGFragPosAttachment);
			glDeleteTextures(1, &mGNormalAttachment);
			glDeleteTextures(1, &mGAlbedoSpecAttachment);
			glDeleteTextures(1, &mGEmissionAttachment);
		}

	private:
		unsigned int mID{};							// Framebuffer Object ID
		unsigned int mRboID{};						// Renderbuffer Object ID
		unsigned int mColorAttachment{};			// Color Attachment of scene
		unsigned int mEntityIDAttachment{};			// Attachment that contains Entity ID of each corresponding pixel
		unsigned int mBrightColorsAttachment;		// Bloom attachment

		unsigned int mGFragPosAttachment{};			// G-buffer fragment position attachment for deferred lighting
		unsigned int mGNormalAttachment{};			// G-buffer normal attachment for deferred lighting
		unsigned int mGAlbedoSpecAttachment{};		// G-buffer Albedo-specular attachment for deferred lighting
		unsigned int mGEmissionAttachment{};		// G-buffer emission attachment for deferred lighting

		int mWidth{};
		int mHeight{};

		void CreateAttachment(unsigned& attachment, GLenum internalFormat, GLenum format, GLenum type);
	};
}

#endif // !MS_FBO_HPP