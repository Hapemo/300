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

#include "Graphics/GraphicsSystem.h"
#include <ECS/ECS_Components.h>


namespace GFX
{
    // temp fnct to test animations
    void PrintMat4(const glm::mat4& input)
    {
        std::cout << "=============================================================================\n";
        std::cout << "| " << input[0][0] << " | " << input[1][0] << " | " << input[2][0] << " | " << input[3][0] << "\n";
        std::cout << "| " << input[0][1] << " | " << input[1][1] << " | " << input[2][1] << " | " << input[3][1] << "\n";
        std::cout << "| " << input[0][2] << " | " << input[1][2] << " | " << input[2][2] << " | " << input[3][2] << "\n";
        std::cout << "| " << input[0][3] << " | " << input[1][3] << " | " << input[2][3] << " | " << input[3][3] << "\n";
        std::cout << "=============================================================================\n";
    }

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

        // reset the bone matrices
        for (auto& mtx : m_FinalBoneMatrices) {
            mtx = glm::mat4(1.f);
        }
	}


    // Update the animation
    void Animator::UpdateAnimation(float dt, const glm::mat4& inputmtx, const glm::mat4& LTW)
    {
        assert(m_CurrentAnimation != nullptr);

        m_DeltaTime = dt;

        // shouldnt increment the time when the animation is paused
        if (!mIsPaused) {
            m_CurrentTime += m_CurrentAnimation->m_TicksPerSecond * m_DeltaTime;
            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->m_Duration);        // this loops the animation
        }

        CalculateBoneTransform(&m_CurrentAnimation->m_RootNode, inputmtx, LTW);          // this calculates the bone matrices, WRT to the entity's world position
    }


    // Calculate the bone matrices, WRT to the entity's world position
    void Animator::CalculateBoneTransform(const _GEOM::AssimpNodeData* node, glm::mat4 parentTransform, const glm::mat4& LTW)
    {
        static const mat4 identity(1.f);
        static const vec4 identityvec(1.f);

        const std::string& nodename = node->m_Name;
        glm::mat4 nodeTransform = node->m_Transformation;

        _GEOM::Bone* bone = m_CurrentAnimation->FindBone(nodename);
        if (bone)
        {
            // there exist a bone with the same node name
            bone->Update(m_CurrentTime);
            nodeTransform = bone->GetLocalTransform();
        }

        glm::mat4 globalTransform = parentTransform; // * nodeTransform;

        auto& boneInfoMap = m_CurrentAnimation->m_BoneInfoMap;
        if (boneInfoMap.find(nodename) != boneInfoMap.end())
        {
            // there exist a bone info map with the same node name
            globalTransform = parentTransform * nodeTransform;
            int index = boneInfoMap[nodename].id;                           // the index for the boneinfomap is the id of the bone
            glm::mat4 offset = boneInfoMap[nodename].offset;                // the offset is the offset WRT to the bone's origin transformation

            m_FinalBoneMatrices[index] = globalTransform * offset;      // populating this vector of matrices with the bone matrices, WRT to the bone's parent transformation
                                                                        // and the entity's world position

            //!< ==== Debug Drawing for the bones ==== //
            if (systemManager->mGraphicsSystem->m_DebugDrawing && (parentTransform != identity))
            {
                vec4 final  = LTW * globalTransform * identityvec;
                vec4 parent = LTW * parentTransform * identityvec;

                systemManager->mGraphicsSystem->m_Renderer.AddCube(parent, { 0.1f, 0.1f, 0.1f }, { 1.f, 0.f, 0.f, 1.f });
                systemManager->mGraphicsSystem->m_Renderer.AddLine( parent, final, {0.f, 1.f, 1.f, 1.f});
            }
        }

        // recursively call this function for all the children of this node
        for (int i{}; i < node->m_NumChildren; ++i) {
            CalculateBoneTransform(&node->m_Children[i], globalTransform, LTW);
        }
    }
}


//!< ENGINE NAMESPACE >
bool Animator::IsEndOfAnimation()
{
    // the next frame will loop the animation back to the beginning
    //std::cout << "deltatime: " << mAnimator.m_CurrentTime << " / " << mAnimator.m_CurrentAnimation->m_Duration << "\n";
    if ((mAnimator.m_CurrentTime + (mAnimator.m_CurrentAnimation->m_TicksPerSecond * mAnimator.m_DeltaTime)) >= mAnimator.m_CurrentAnimation->m_Duration)
    {
        return true;
    }
    return false;
}

// thisentity.IsEndOfAnimation()
// 