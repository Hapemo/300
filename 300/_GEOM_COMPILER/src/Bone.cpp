/*!*************************************************************************
****
\file			Bone.cpp
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The definitions for the bone class

****************************************************************************
***/

#include <Bone.h>

#include <iostream>
#include <GLM/gtx/quaternion.hpp>

namespace _GEOM
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Constructors
	
/***************************************************************************/
/*!
\brief
	Non default bone constructor. Initializes the bones and its relevant
	keyframe transformations
*/
/**************************************************************************/
	Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
	{
		m_LocalTransform = glm::mat4(1.f);
		m_Name = name;
		m_ID = ID;
		m_NumPositions = channel->mNumPositionKeys;

		// Getting the bone's position data
		for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
		{
			aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
			float timeStamp = static_cast<float>(channel->mPositionKeys[positionIndex].mTime);
			KeyPosition data;
			data.position = AssimpHelper::GetGLMVec(aiPosition);
			data.timeStamp = timeStamp;
			m_Positions.push_back(data);
		}

		// Getting the bone's rotation data
		m_NumRotations = channel->mNumRotationKeys;
		for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
		{
			aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;	// getting the aiQuaternion from assimp's channel
			float timeStamp = static_cast<float>(channel->mRotationKeys[rotationIndex].mTime);				// getting the rotation's timestamp
			KeyRotation data;
			data.orientation = AssimpHelper::GetGLMQuat(aiOrientation);					// convert from Assimp to GLM format. assignment
			data.timeStamp = timeStamp;													// assignment		
			m_Rotations.push_back(data);												// populate bone's internal rotation data
		}

		// Getting the bone's scale data
		m_NumScalings = channel->mNumScalingKeys;
		for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
		{
			aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
			float timeStamp = static_cast<float>(channel->mScalingKeys[keyIndex].mTime);
			KeyScale data;
			data.scale = AssimpHelper::GetGLMVec(scale);
			data.timeStamp = timeStamp;
			m_Scales.push_back(data);
		}
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Modified Getters
	
/***************************************************************************/
/*!
\brief
	Get the bone's position index, given at which point in time the animation is currently in
*/
/**************************************************************************/
	int Bone::GetPositionIndex(float animationTime)
	{
		for (int index{}; index < m_NumPositions - 1; ++index)
		{
			// if the animation time is less than the next keyframe's timestamp, return the current index
			if(animationTime < m_Positions[index + 1].timeStamp)
				return index;
		}
		assert(0);

		return 0;
	}


/***************************************************************************/
/*!
\brief
	// Get the bone's rotation index, given at which point in time the animation is currently in
*/
/**************************************************************************/
	int Bone::GetRotationIndex(float animationTime)
	{
		for (int index{}; index < m_NumRotations - 1; ++index)
		{
			// if the animation time is less than the next keyframe's timestamp, return the current index
			if (animationTime < m_Rotations[index + 1].timeStamp)
				return index;
		}
		assert(0);

		return 0;
	}


/***************************************************************************/
/*!
\brief
	// Get the bone's scale index, given at which point in time the animation is currently in
*/
/**************************************************************************/
	int Bone::GetScaleIndex(float animationTime)
	{
		for (int index{}; index < m_NumScalings - 1; ++index)
		{
			// if the animation time is less than the next keyframe's timestamp, return the current index
			if (animationTime < m_Scales[index + 1].timeStamp)
				return index;
		}
		assert(0);

		return 0;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters

/***************************************************************************/
/*!
\brief
	bone_id getter
*/
/**************************************************************************/
	int Bone::GetBoneID() const
	{
		return m_ID;
	}

/***************************************************************************/
/*!
\brief
	bone name getter
*/
/**************************************************************************/
	std::string Bone::GetBoneName() const
	{
		return m_Name;
	}


/***************************************************************************/
/*!
\brief
	local transform getter
*/
/**************************************************************************/
	glm::mat4 Bone::GetLocalTransform() const
	{
		return m_LocalTransform;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Primary Functions

/***************************************************************************/
/*!
\brief
	// Updates the position, rotation and scale of the bone WRT to the current animation time
*/
/**************************************************************************/
	void Bone::Update(float animationTime)
	{
		glm::mat4 translation = InterpolatePosition(animationTime);
		glm::mat4 rotation = InterpolateRotation(animationTime);
		glm::mat4 scale = InterpolateScaling(animationTime);
;
		m_LocalTransform = translation * rotation * scale;
		//m_LocalTransform = trans;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Private Functions

/***************************************************************************/
/*!
\brief
	get the scale factor depending on the animation time with respect
	to the next timeframe
*/
/**************************************************************************/
	float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		float scaleFactor = 0.f;	
		float midWayLength = animationTime - lastTimeStamp;		// how far into the animation are we?
		float framesDiff = nextTimeStamp - lastTimeStamp;		// how many frames are there between the two keyframes?
		scaleFactor = midWayLength / framesDiff;				// how far into the animation are we, in terms of frames? given in a fraction of one another
		return scaleFactor;
	}

/***************************************************************************/
/*!
\brief
	internal function used to interpolate the position given the animation
	time
*/
/**************************************************************************/
	glm::mat4 Bone::InterpolatePosition(float animationTime)
	{
		// if there is only one position keyframe, return the position
		if (1 == m_NumPositions) {
			return glm::translate(glm::mat4(1.f), m_Positions[0].position);
		}

		int p0Index = GetPositionIndex(animationTime);
		int p1Index = p0Index + 1;

		// given the current animation time, tell me how far i am between the next animation frame
		float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp, m_Positions[p1Index].timeStamp, animationTime);	

		// get the final position by interpolating between the two keyframes, given the scale factor
		glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position, m_Positions[p1Index].position, scaleFactor);
		return glm::translate(glm::mat4(1.f), finalPosition);
	}

/***************************************************************************/
/*!
\brief
	internal functoin used to interpolate the rotation given the 
	animation time
*/
/**************************************************************************/
	glm::mat4 Bone::InterpolateRotation(float animationTime)
	{
		// if there is only one rotation keyframe, return the rotation
		if (1 == m_NumRotations) {
			return glm::toMat4(glm::normalize(m_Rotations[0].orientation));
		}

		int p0Index = GetRotationIndex(animationTime);
		int p1Index = p0Index + 1;

		// given the current animation time, tell me how far i am between the next animation frame
		float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp, m_Rotations[p1Index].timeStamp, animationTime);

		// get the final rotation by interpolating between the two keyframes, given the scale factor
		glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation, scaleFactor);
		finalRotation = glm::normalize(finalRotation);
		return glm::toMat4(finalRotation);
	}

/***************************************************************************/
/*!
\brief
	internal function used to interpolate the scaling given the animation
	time
*/
/**************************************************************************/
	glm::mat4 Bone::InterpolateScaling(float aniamtionTime)
	{
		// if there is only one scale keyframe, return the scale
		if (1 == m_NumScalings) {
			return glm::scale(glm::mat4(1.f), m_Scales[0].scale);
		}

		int p0Index = GetScaleIndex(aniamtionTime);
		int p1Index = p0Index + 1;

		// given the current animation time, tell me how far i am between the next animation frame
		float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp, m_Scales[p1Index].timeStamp, aniamtionTime);
		glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale, scaleFactor);
		return glm::scale(glm::mat4(1.f), finalScale);
	}
}