#include <Graphics/PostProcessing.h>
#include <Graphics/GraphicsSystem.h>

// http://www.andersriggelsen.dk/glblendfunc.php

void PostProcessing::AdditiveBlendFramebuffers(GFX::FBO& targetFramebuffer, unsigned int Attachment0, unsigned int Attachment1)
{
	// additive blending blend function
	glBlendFunc(GL_ONE, GL_ONE);

	uid shaderstr("AdditiveBlendShader");
	GFX::Shader& BlendShader = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);

	BlendShader.Activate();
	targetFramebuffer.Bind();

	// Draw to color attachment only. Otherwise might affect other attachments
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glUniform1f(BlendShader.GetUniformLocation("Exposure"), systemManager->mGraphicsSystem->mAmbientBloomExposure);
	glBindTexture(GL_TEXTURE_2D, Attachment0);									// bind the first attachment
	glBindTexture(GL_TEXTURE_2D, Attachment1);									// bind the second attachment

	{
		systemManager->mGraphicsSystem->mScreenQuad.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		systemManager->mGraphicsSystem->mScreenQuad.Unbind();
	}

	targetFramebuffer.Unbind();
	BlendShader.Deactivate();
}


void PostProcessing::ChromaticAbbrebationBlendFramebuffers(GFX::FBO& targetFramebuffer, unsigned int Attachment1)
{
	//glBlendFunc(GL_ONE, GL_ZERO);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

	uid shaderstr("ChromaticAbberation");
	GFX::Shader& BlendShader = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);

	BlendShader.Activate();
	targetFramebuffer.Bind();

	// Draw to color attachment only. Otherwise might affect other attachments
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glUniform1f(BlendShader.GetUniformLocation("ChromaticAbberationOffset"), systemManager->mGraphicsSystem->mChromaticOffset);
	glUniform1f(BlendShader.GetUniformLocation("ChromaticAbberationStrength"), systemManager->mGraphicsSystem->mChromaticStrength);
	glBindTexture(GL_TEXTURE_2D, Attachment1);									// bind the second attachment

	{
		systemManager->mGraphicsSystem->mScreenQuad.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		systemManager->mGraphicsSystem->mScreenQuad.Unbind();
	}

	targetFramebuffer.Unbind();
	BlendShader.Deactivate();
}


void PostProcessing::CRTBlendFramebuffers(GFX::FBO& targetFramebuffer, GFX::PingPongFBO& bufferfbo, float dt)
{
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

	{
		// blit the framebuffer to the temp framebuffer, as a reference framebuffer before drawing onto the target framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, targetFramebuffer.mID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bufferfbo.pingpongFBO);

		// im using the second pingpongcolor attachment as the buffer to blit to
		glDrawBuffer(GL_COLOR_ATTACHMENT1);
		glBlitFramebuffer(0, 0, targetFramebuffer.mWidth, targetFramebuffer.mHeight, 0, 0, targetFramebuffer.mWidth, targetFramebuffer.mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Restore the default framebuffer
	}

	uid shaderstr("CRT_Shader");
	GFX::Shader& BlendShader = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);

	BlendShader.Activate();
	targetFramebuffer.Bind();

	// Draw to color attachment only. Otherwise might affect other attachments
	// output texture
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glUniform1f(BlendShader.GetUniformLocation("accumulationTime"), (PostProcessing::getInstance().accumulationTime += dt));

	// input texture
	glBindTexture(GL_TEXTURE_2D, bufferfbo.pingpongColorbuffers[1]);									

	{
		systemManager->mGraphicsSystem->mScreenQuad.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		systemManager->mGraphicsSystem->mScreenQuad.Unbind();
	}

	targetFramebuffer.Unbind();
	BlendShader.Deactivate();
}


void PostProcessing::PhysBasedBloom()
{
	
}


bool PhysBasedBloomFBO::Init(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipChainLength)
{
	if (mIsInit)
		return true;

	// safety check
	assert(windowWidth < (unsigned int)INT_MAX && windowHeight < (unsigned int)INT_MAX);

	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glm::vec2 mipSize((float)windowWidth, (float)windowHeight);
	glm::ivec2 intMipSize((int)windowWidth, (int)windowHeight);

	for (unsigned int i{}; i < mipChainLength; ++i)
	{
		// creating the mips
		BloomMip mip;

		mipSize *= 0.5f;
		intMipSize /= 2;
		mip.mSize = mipSize;
		mip.mIntSize = intMipSize;

		// generate textures
		glGenTextures(1, &mip.mTexture);
		glBindTexture(GL_TEXTURE_2D, mip.mTexture);

		// we are downscaling the texture. since we dont need alpha, the GL_R11F_G11F_B10F flag gives us more hdr precision
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, intMipSize.x, intMipSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, intMipSize.x, intMipSize.y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, intMipSize.x, intMipSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//glNamedFramebufferTexture(mFBO, GL_COLOR_ATTACHMENT0 + i, mip.mTexture, 0);

		mMipChain.emplace_back(mip);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMipChain[0].mTexture, 0);
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mIsInit = true;
	return true;
}


void PhysBasedBloomFBO::Destroy()
{
	for (int i{}; i < mMipChain.size(); ++i)
	{
		glDeleteTextures(1, &mMipChain[i].mTexture);
		mMipChain[i].mTexture = 0;
	}
	glDeleteFramebuffers(1, &mFBO);
	mFBO = 0;
	mIsInit = false;
}


void PhysBasedBloomFBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}


bool PhysBasedBloomRenderer::Init(unsigned int windowWidth, unsigned int windowHeight)
{
	if (mInit)
		return true;

	mIntSrcViewportSize = glm::ivec2(windowWidth, windowHeight);
	mSrcViewportSize	= glm::vec2((float)windowWidth, (float)windowHeight);

	// create the framebuffer
	const unsigned int num_bloom_mips = 5;		// this value is interchangable
	bool status = mBloomFBO.Init(windowWidth, windowHeight, num_bloom_mips);
	assert(status);

	// Shaders
	mDownSamplingShaderStr = "DownSampleShader";
	mUpSamplingShaderStr = "UpSampleShader";

	mInit = true;
	return true;
}


void PhysBasedBloomRenderer::Destroy()
{
	mBloomFBO.Destroy();
	mInit = false;
}


void PhysBasedBloomRenderer::PrepForDraw()
{
	mBloomFBO.Bind();

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
}


void PhysBasedBloomRenderer::RenderBloom(unsigned int sourceTexture, float filterRadius)
{
	mBloomFBO.Bind();

	RenderDownsamples(sourceTexture);
	RenderUpsamples(filterRadius);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//PostProcessing::BlitFrameBuffers(0, targetFBO.mID, )

	// restore viewport
	glViewport(0, 0, mIntSrcViewportSize.x, mIntSrcViewportSize.y);
}


void PhysBasedBloomRenderer::RenderDownsamples(unsigned int sourceTexture)
{
	glDisable(GL_BLEND);
	const std::vector<BloomMip>& mipchain = mBloomFBO.mMipChain;

	uid shaderstr(mDownSamplingShaderStr);
	GFX::Shader& shaderinst = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);
	shaderinst.Activate();

	GLuint resolution = shaderinst.GetUniformLocation("srcResolution");
	glUniform2fv(resolution, 1, glm::value_ptr(mSrcViewportSize));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sourceTexture);	// bind the source texture

	// progressively downsample through the mip chain
	for (int i{}; i < mipchain.size(); ++i)
	{
		const BloomMip& mip = mipchain[i];
		glViewport(0, 0, (GLsizei)mip.mSize.x, (GLsizei)mip.mSize.y);
		
		//glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mip.mTexture, 0);

		// render the screen quad
		systemManager->mGraphicsSystem->mScreenQuad.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		systemManager->mGraphicsSystem->mScreenQuad.Unbind();

		// set the current mip resolution as srcResolution for the next iteration
		glUniform2fv(resolution, 1, glm::value_ptr(mip.mSize));

		// set the current mip as texture input for the next iteration
		glBindTexture(GL_TEXTURE_2D, mip.mTexture);
	}

	shaderinst.Deactivate();
}


void PhysBasedBloomRenderer::RenderUpsamples(float filterRadius)
{
	const std::vector<BloomMip>& mipchain = mBloomFBO.mMipChain;

	uid shaderstr(mUpSamplingShaderStr);
	GFX::Shader& shaderinst = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);
	shaderinst.Activate();

	GLuint radiusloc = shaderinst.GetUniformLocation("filterRadius");
	glUniform1f(radiusloc, filterRadius);

	// enable additive blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	for (int i = mipchain.size() - 1; i > 0; --i)
	{
		const BloomMip& mip = mipchain[i];
		const BloomMip& nextMip = mipchain[i - 1];

		// bind viewport and texture for the source
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mip.mTexture);

		// set the framebuffer render target as the destination
		glViewport(0, 0, nextMip.mIntSize.x, nextMip.mIntSize.y);

		//glDrawBuffer(GL_COLOR_ATTACHMENT0 + i-1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nextMip.mTexture, 0);	// draws to this locaion

		// render the screen quad
		systemManager->mGraphicsSystem->mScreenQuad.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		systemManager->mGraphicsSystem->mScreenQuad.Unbind();
	}
	
	shaderinst.Deactivate();
}


void PostProcessing::BlitFrameBuffers(unsigned int readFramebuffer, unsigned int drawFramebuffer, ivec2 readFramebufferSize, ivec2 drawFramebufferSize, int drawColorAttachmentOffset )
{
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

	{
		// blit the framebuffer to the temp framebuffer, as a reference framebuffer before drawing onto the target framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer);

		// im using the second pingpongcolor attachment as the buffer to blit to
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + drawColorAttachmentOffset);
		glBlitFramebuffer(0, 0, readFramebufferSize.x, readFramebufferSize.y, 0, 0, drawFramebufferSize.x, drawFramebufferSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Restore the default framebuffer
	}

}