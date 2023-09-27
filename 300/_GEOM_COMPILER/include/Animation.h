#pragma once

#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <unordered_map>
#include <vector>
#include <Bone.h>

namespace _GEOM
{
	// The animation information of the mesh. The animation data is read from assimp in the GEOM Compiler
	struct Animation
	{
		std::unordered_map<std::string, BoneInfo>		m_BoneInfoMap;
		std::vector<Bone>								m_Bones;
		AssimpNodeData									m_RootNode;

		int												m_BoneCounter = 0;
		float											m_Duration{};
		float											m_TicksPerSecond{};

		Animation() = default;
		Bone* FindBone(const std::string& name);

		// NOTE:: there is another member variable called AssimpNodeData, that contains the
		// transformation, childrencount, and name. But this is already done with pretransformations
		// in the mesh loading.. keep it in mind for now.
	};
}

#endif // !_ANIMATION_H