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

#define MAX_NUM_BONES 200

#include <Bone.h>
#include <glm/glm.hpp>

#include <geom.h>

namespace GFX 
{
    //////////////////////////////////////////////////////////////////////////////////////////////////

    // The animator class should be tied to the entity. Since the animator class is responsible for 
    // Updating the specific animation of the entity's model.
    // The animator class should be assigned to the entity in the entity's creation, and released when
    // the entity is destroyed.
    class Animator
    {
    public:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        _GEOM::Animation* m_CurrentAnimation;
        float m_CurrentTime;
        float m_DeltaTime;


    public:
        Animator();
        Animator(_GEOM::Animation*);

        void SetAnimation(_GEOM::Animation*);
        void UpdateAnimation(float dt, const glm::mat4&, const glm::mat4& LTW);

        bool mIsPaused{ 0 };

    private:
        void CalculateBoneTransform(const _GEOM::AssimpNodeData* node, glm::mat4 parentTransform, const glm::mat4& LTW);


    };

}

#endif // ANIMATOR_HPP
