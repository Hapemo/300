#pragma once

#ifndef _BONE_H
#define _BONE_H

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

//#include <geom.h>
#include <Helper.h>

namespace _GEOM
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct BoneInfo
	{
		/*id is index in finalBoneMatrices*/
		int id;

		/*offset matrix transforms vertex from model space to bone space*/
		glm::mat4 offset;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	struct AssimpNodeData
	{
		glm::mat4						m_Transformation;
		std::string						m_Name;
		int								m_NumChildren{};
		std::vector<AssimpNodeData>		m_Children;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct KeyPosition
	{
		glm::vec3 position;
		float timeStamp;
	};

	struct KeyRotation
	{
		glm::quat orientation;
		float timeStamp;
	};

	struct KeyScale
	{
		glm::vec3 scale;
		float timeStamp;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Bone
	{
	public:
		Bone(const std::string& name, int ID, const aiNodeAnim* channel);

		glm::mat4 GetLocalTransform() { return m_LocalTransform; }
		
		std::string GetBoneName() const { return m_Name; }
	
		int GetBoneID() { return m_ID; }

		int GetPositionIndex(float animationTime)
		{
			for (int index = 0; index < m_NumPositions - 1; ++index)
			{
				if (animationTime < m_Positions[index + 1].timeStamp)
					return index;
			}
			assert(0);
		}

		int GetRotationIndex(float animationTime)
		{
			for (int index = 0; index < m_NumRotations - 1; ++index)
			{
				if (animationTime < m_Rotations[index + 1].timeStamp)
					return index;
			}
			assert(0);
		}

		int GetScaleIndex(float animationTime)
		{
			for (int index = 0; index < m_NumScalings - 1; ++index)
			{
				if (animationTime < m_Scales[index + 1].timeStamp)
					return index;
			}
			assert(0);
		}


	private:
		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scales;
		int m_NumPositions;
		int m_NumRotations;
		int m_NumScalings;

		glm::mat4 m_LocalTransform;
		std::string m_Name;
		int m_ID;
	};
}

#endif // !_BONE_H