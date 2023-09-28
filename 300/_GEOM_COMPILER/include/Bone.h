/*!*************************************************************************
****
\file			Bone.h
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The declarations for the bone class and the structs that assist with
	its implementation

****************************************************************************
***/

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

/***************************************************************************/
/*!
\brief
	This class contains the offset for each individual bone relative to the
	model space
*/
/**************************************************************************/
	struct BoneInfo
	{
		/*id is index in finalBoneMatrices*/
		int id;

		/*offset matrix transforms vertex from model space to bone space*/
		glm::mat4 offset;
	};

	
/***************************************************************************/
/*!
\brief
	This class contains the recursive parent/child nature of the bones.
	The m_children stores the children of this particular bone, which
	then contains its own children
*/
/**************************************************************************/
	struct AssimpNodeData
	{
		glm::mat4						m_Transformation{};
		std::string						m_Name{};
		int								m_NumChildren{};
		std::vector<AssimpNodeData>		m_Children{};
	};

/***************************************************************************/
/*!
\brief
	The position keyframe within the animation
*/
/**************************************************************************/
	struct KeyPosition
	{
		glm::vec3 position;
		float timeStamp;
	};

/***************************************************************************/
/*!
\brief
	The rotation keyframe within the animation
*/
/**************************************************************************/
	struct KeyRotation
	{
		glm::quat orientation;
		float timeStamp;
	};

/***************************************************************************/
/*!
\brief
	The scale keyframe within the animation
*/
/**************************************************************************/
	struct KeyScale
	{
		glm::vec3 scale;
		float timeStamp;
	};

/***************************************************************************/
/*!
\brief
	The bone class that contains the data of a single bone
*/
/**************************************************************************/
	class Bone
	{

	//!< Member Variables
	public:
		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scales;
		std::string m_Name{};
		glm::mat4 m_LocalTransform;

		int m_NumPositions{};
		int m_NumRotations{};
		int m_NumScalings{};
		int m_ID{};

	//!< Public Member Functions
	public:
		
	/***************************************************************************/
	/*!
	\brief
		The default bone constructor
	*/
	/**************************************************************************/
		Bone() = default;

	/***************************************************************************/
	/*!
	\brief
		Non default constructor for the bone
	*/
	/**************************************************************************/
		Bone(const std::string& name, int ID, const aiNodeAnim* channel);

	/***************************************************************************/
	/*!
	\brief
		Getters for the bone class
	*/
	/**************************************************************************/
		glm::mat4 GetLocalTransform() const;
		std::string GetBoneName() const;
		int GetBoneID() const;

	/***************************************************************************/
	/*!
	\brief
		Specialized getters for the bone class
	*/
	/**************************************************************************/
		int GetPositionIndex(float animationTime);
		int GetRotationIndex(float animationTime);
		int GetScaleIndex(float animationTime);

	/***************************************************************************/
	/*!
	\brief
		The update function given the animation time
	*/
	/**************************************************************************/
		void Update(float animationTime);

	//!< Private Member Functions
	private:

		/***************************************************************************/
		/*!
		\brief
			Interpolates the Position, Rotation, and Scale according to the animation
			time
		*/
		/**************************************************************************/
		glm::mat4 InterpolatePosition(float animationTime);
		glm::mat4 InterpolateRotation(float animationTime);
		glm::mat4 InterpolateScaling(float animationTime);

		/***************************************************************************/
		/*!
		\brief
			The scale result of the animation time as a result of its outcome 
			between its last timestamp and its next timestamp
		*/	
		/**************************************************************************/
		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	};
}

#endif // !_BONE_H