/*!*************************************************************************
****
\file			Animation.cpp
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The function definition for the animation class

****************************************************************************
***/

#include <Animation.h>

namespace _GEOM
{

/***************************************************************************/
/*!
\brief
	Find the relevant bone given the name
*/
/**************************************************************************/
	Bone* Animation::FindBone(const std::string& name)
	{
		auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
			[&](const Bone& bone)
			{
				return bone.GetBoneName() == name;		// compare the similarity of the bone name
			});

		if (iter == m_Bones.end())
			return nullptr;

		// if a matching bone is found, return the pointer
		return &(*iter);
	}

}