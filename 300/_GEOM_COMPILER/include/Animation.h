/*!*************************************************************************
****
\file			Animation.h
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The function declarations for the animator class

****************************************************************************
***/

#pragma once

#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <unordered_map>
#include <map>
#include <vector>
#include <Bone.h>

namespace _GEOM
{
	/***************************************************************************/
	/*!
	\brief
		// The animation information of the mesh. The animation data is read from assimp in the GEOM Compiler
	*/
	/**************************************************************************/
	struct Animation
	{
		std::unordered_map<std::string, BoneInfo>		m_BoneInfoMap;
		std::map<std::string, Bone>						m_Bones;
		AssimpNodeData									m_RootNode;

		int												m_BoneCounter = 0;
		float											m_Duration{};
		float											m_TicksPerSecond{};

	/***************************************************************************/
	/*!
	\brief
		default constructor
	*/
	/**************************************************************************/
		Animation() = default;

	/***************************************************************************/
	/*!
	\brief
		Finds the bone according to the name
	*/
	/**************************************************************************/
		Bone* FindBone(const std::string& name);
	};
}

#endif // !_ANIMATION_H