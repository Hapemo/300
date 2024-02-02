#pragma once
#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>

#include <Fbo.hpp>


struct BloomMip
{
	glm::vec2		mSize;
	glm::ivec2		mIntSize;
	unsigned int	mTexture;
};


class PhysBasedBloomFBO
{
public:
	PhysBasedBloomFBO() 
	{}

	~PhysBasedBloomFBO() 
	{}

	bool Init(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipChainLength);
	void Destroy();
	void Bind();

public:
	std::vector<BloomMip>	mMipChain;
	unsigned int			mFBO;
	bool					mIsInit;
};


class PhysBasedBloomRenderer
{
public:
	PhysBasedBloomRenderer() : mInit(false)
	{}

	~PhysBasedBloomRenderer() 
	{}
	
	bool Init(unsigned int windowWidth, unsigned int windowHeight);
	void Destroy();
	void PrepForDraw();
	void Resize(int width, int height);
	void RenderBloom(unsigned int sourceTexture, float filterRadius);
	GLuint getBloomTexture() { return mBloomFBO.mMipChain[0].mTexture; }
	
public:
	bool				mInit;
	PhysBasedBloomFBO	mBloomFBO;
	glm::ivec2			mIntSrcViewportSize;
	glm::vec2			mSrcViewportSize;
	std::string			mUpSamplingShaderStr;
	std::string			mDownSamplingShaderStr;

private:
	void RenderDownsamples(unsigned int sourceTexture);
	void RenderUpsamples(float filterRadius);
};


struct PostProcessing
{
	PostProcessing() : mCRT_AccumulationTime(0.0), mCRT_DistortionValue(0.0005f), mCRT_HeightOffset(100), mCRT_ChromaticAbberationStrength(0.002f)
	{}

	static PostProcessing& getInstance()
	{
		static PostProcessing instance;
		return instance;
	}
	
	/***************************************************************************/
	/*!
	\brief
		the additive blend function for post processing bloom
	*/
	/**************************************************************************/
	static void AdditiveBlendFramebuffers(GFX::FBO& targetFramebuffer, unsigned int Attachment0, unsigned int Attachment1);

	
	/***************************************************************************/
	/*!
	\brief
		post processing chromatic abbrebation effect
	*/
	/**************************************************************************/
	static void ChromaticAbbrebationBlendFramebuffers(GFX::FBO& targetFramebuffer, unsigned int Attachment1);

	
	/***************************************************************************/
	/*!
	\brief
		post processing CRT effect
	*/
	/**************************************************************************/
	static void CRTBlendFramebuffers(GFX::FBO& targetFramebuffer, GFX::PingPongFBO& bufferfbo, float dt);

	static void BlitFrameBuffers(unsigned int readFramebuffer, unsigned int drawFramebuffer, ivec2 readFramebufferSize, ivec2 drawFramebufferSize, int drawColorAttachmentOffset);

public:
	double	mCRT_AccumulationTime;
	int		mCRT_HeightOffset;
	float	mCRT_DistortionValue;
	float	mCRT_ChromaticAbberationStrength;
};


#endif // !POSTPROCESSING_H

//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
//
//{
//	// blit the framebuffer to the temp framebuffer, as a reference framebuffer before drawing onto the target framebuffer
//	glBindFramebuffer(GL_READ_FRAMEBUFFER, targetFramebuffer.mID);
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bufferfbo.pingpongFBO);
//
//	// im using the second pingpongcolor attachment as the buffer to blit to
//	glDrawBuffer(GL_COLOR_ATTACHMENT1);
//	glBlitFramebuffer(0, 0, targetFramebuffer.mWidth, targetFramebuffer.mHeight, 0, 0, targetFramebuffer.mWidth, targetFramebuffer.mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Restore the default framebuffer
//}
//
//uid shaderstr("CRT_Shader");
//GFX::Shader& BlendShader = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);
//
//BlendShader.Activate();
//targetFramebuffer.Bind();
//
//// Draw to color attachment only. Otherwise might affect other attachments
//glDrawBuffer(GL_COLOR_ATTACHMENT0);
//
//glUniform1f(BlendShader.GetUniformLocation("mCRT_AccumulationTime"), (PostProcessing::getInstance().mCRT_AccumulationTime += dt));
//glBindTexture(GL_TEXTURE_2D, bufferfbo.pingpongColorbuffers[1]);
//
//{
//	systemManager->mGraphicsSystem->mScreenQuad.Bind();
//
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	systemManager->mGraphicsSystem->mScreenQuad.Unbind();
//}
//
//targetFramebuffer.Unbind();
//BlendShader.Deactivate();