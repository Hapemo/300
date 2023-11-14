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


void PostProcessing::CRTBlendFramebuffers(GFX::FBO& targetFramebuffer, unsigned int tempFramebuffer, unsigned int tempColorAttachment, float dt)
{
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

	{
		// blit the framebuffer to the temp framebuffer, as a reference framebuffer before drawing onto the target framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, targetFramebuffer.mID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, tempFramebuffer);

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
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glUniform1f(BlendShader.GetUniformLocation("accumulationTime"), (PostProcessing::getInstance().accumulationTime += dt));
	glBindTexture(GL_TEXTURE_2D, tempColorAttachment);

	{
		systemManager->mGraphicsSystem->mScreenQuad.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		systemManager->mGraphicsSystem->mScreenQuad.Unbind();
	}

	targetFramebuffer.Unbind();
	BlendShader.Deactivate();
}
