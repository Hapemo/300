#include "Reflection/Reflection.h"
#include "ECS/ECS_Components.h"

void RegisterAllComponents()
{
	RegisterVec3();
	RegisterCapsuleCollider();
	RegisterVFX();
}

void RegisterVec3()
{
	meta::class_<glm::vec3>()
		.member_("x", &glm::vec3::x)
		.member_("y", &glm::vec3::y)
		.member_("z", &glm::vec3::z);
}

void RegisterCapsuleCollider()
{
	meta::class_<CapsuleCollider>()
		.member_("Translate Offset CC", &CapsuleCollider::mTranslateOffset)
		.member_("Radius CC", &CapsuleCollider::mRadius)
		.member_("Half Height CC", &CapsuleCollider::mHalfHeight)
		.member_("Is Trigger CC", &CapsuleCollider::mIsTrigger);
}

void RegisterVFX()
{
	meta::class_<VFX>()
		.member_("Bloom Threshold", &VFX::mBloomThreshold)
		.member_("Is Object Bloom", &VFX::isObjectBloom);
}

void SetCursor()
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
}
