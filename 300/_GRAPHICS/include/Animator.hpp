/**-------------------------------------------------------------------------------------/
 * @file
 *  Animator.hpp
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

#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#define MAX_NUM_ANIMATE_OBJECTS 100

#include <glm/glm.hpp>

#include <geom.h>

namespace GFX 
{
    //////////////////////////////////////////////////////////////////////////////////////////////////

    class Animator
    {

    private:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        _GEOM::Animation* m_CurrentAnimation;
        float m_CurrentTime;
        float m_DeltaTime;
        bool m_IsPlaying;


    public:
        Animator();
        Animator(_GEOM::Animation);

        void SetAnimation(_GEOM::Animation*);
        void UpdateAnimation(float dt);


    private:
        void CalculateBoneTransform(const _GEOM::AssimpNodeData* node, glm::mat4 parentTransform);

    };


    //////////////////////////////////////////////////////////////////////////////////////////////////

    class AnimatorManager
    {

    public:
        AnimatorManager();
        Animator& GetAvailableAnimator();

    private:
        Animator m_SceneAnimators[MAX_NUM_ANIMATE_OBJECTS];

    };
}

#endif // ANIMATOR_HPP
