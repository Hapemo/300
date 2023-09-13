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
		glm::mat4						m_Transformation{};
		std::string						m_Name{};
		int								m_NumChildren{};
		std::vector<AssimpNodeData>		m_Children{};
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

	//!< Member Variables
	public:
		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scales;
		int m_NumPositions;
		int m_NumRotations;
		int m_NumScalings;

		glm::mat4 m_LocalTransform;
		std::string m_Name;
		int m_ID;

	//!< Public Member Functions
	public:
		// Constructors
		Bone() = default;
		Bone(const std::string& name, int ID, const aiNodeAnim* channel);

		// Getters
		glm::mat4 GetLocalTransform() const;
		std::string GetBoneName() const;
		int GetBoneID() const;

		// Modified Getters
		int GetPositionIndex(float animationTime);
		int GetRotationIndex(float animationTime);
		int GetScaleIndex(float animationTime);

		// Primary Functions
		void Update(float animationTime);

	//!< Private Member Functions
	private:
		glm::mat4 InterpolatePosition(float animationTime);
		glm::mat4 InterpolateRotation(float animationTime);
		glm::mat4 InterpolateScaling(float animationTime);
		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	};
}

#endif // !_BONE_H