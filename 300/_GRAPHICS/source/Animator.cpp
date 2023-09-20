/**-------------------------------------------------------------------------------------/
 * @file
 *  Animator.cpp
 * @author
    Richmond Choo // 2100989 // r.choo@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  The animator class takes in the animation data of the mesh, and controls the updating
 *  of the animation
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/


#include <iostream>
#include <Animator.hpp>

namespace GFX
{
    // default
    Animator::Animator()
    {
		m_FinalBoneMatrices.resize(MAX_NUM_BONES, glm::mat4(1.f));
        m_CurrentAnimation = nullptr;
        m_CurrentTime = -1.f;
        m_DeltaTime = -1.f;
    }


    // non default, with animation
    Animator::Animator(_GEOM::Animation* animation)
    {
		m_FinalBoneMatrices.resize(MAX_NUM_BONES, glm::mat4(1.f));
		m_CurrentAnimation = animation;
		m_CurrentTime = 0.f;
		m_DeltaTime = 0.f;
	}


    // The enttiy can set a new animation for the animator to play
    void Animator::SetAnimation(_GEOM::Animation* animation)
    {
		m_CurrentAnimation = animation;
        m_CurrentTime = 0.f;
        m_DeltaTime = 0.f;
	}


    // Update the animation
    void Animator::UpdateAnimation(float dt)
    {
        assert(m_CurrentAnimation != nullptr);

        m_DeltaTime = dt;
        // ECS::GetComponent<Transform>(entityID).getLocalTransform();

        m_CurrentTime += m_CurrentAnimation->m_TicksPerSecond * m_DeltaTime;
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->m_Duration);        // this loops the animation
        CalculateBoneTransform(&m_CurrentAnimation->m_RootNode, glm::mat4(1.f));    // this calculates the bone matrices, WRT to the entity's world position
    }


    // Calculate the bone matrices, WRT to the entity's world position
    void Animator::CalculateBoneTransform(const _GEOM::AssimpNodeData* node, glm::mat4 parentTransform)
    {
        const std::string& nodename = node->m_Name;
        glm::mat4 nodeTransform = node->m_Transformation;

        _GEOM::Bone* bone = m_CurrentAnimation->FindBone(nodename);
        if (bone)
        {
            // there exist a bone with the same node name
            bone->Update(m_CurrentTime);
            nodeTransform = bone->GetLocalTransform();
        }

        glm::mat4 globalTransform = parentTransform * nodeTransform;

        auto& boneInfoMap = m_CurrentAnimation->m_BoneInfoMap;
        if (boneInfoMap.find(nodename) != boneInfoMap.end())
        {
            // there exist a bone info map with the same node name
            //globalTransform = parentTransform * nodeTransform;
            int index = boneInfoMap[nodename].id;                           // the index for the boneinfomap is the id of the bone
            glm::mat4 offset = boneInfoMap[nodename].offset;                // the offset is the offset WRT to the bone's origin transformation
            m_FinalBoneMatrices[index] = globalTransform * offset;          // populating this vector of matrices with the bone matrices, WRT to the bone's parent transformation
                                                                            // and the entity's world position
        }

        // recursively call this function for all the children of this node
        for (int i{}; i < node->m_NumChildren; ++i) {
            CalculateBoneTransform(&node->m_Children[i], globalTransform);
        }
    }
}