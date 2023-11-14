#pragma once
#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>

#include <Fbo.hpp>

struct PostProcessing
{
	PostProcessing() : accumulationTime(0.0)
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
	static void CRTBlendFramebuffers(GFX::FBO& targetFramebuffer, unsigned int tempFramebuffer, float dt);

	double accumulationTime;
};

#endif // !POSTPROCESSING_H
