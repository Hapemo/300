#include "Reflection/Reflection.h"
#include "ECS/ECS_Components.h"

void RegisterAllComponents()
{
	RegisterVec3();
	RegisterTransform();
	RegisterBoxCollider();
	RegisterSphereCollider();
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

//void RegisterGeneral()
//{
//	meta::class_<General>()
//		.member_("name", &General::name)
//		.member_("tagid", &General::tagid)
//		.member_("subtag", &General::subtag)
//		.member_("isActive", &General::isActive)
//		.member_("isPaused", &General::isPaused);
//}

void RegisterTransform()
{
	meta::class_<Transform>()
		.member_("Scale", &Transform::mScale)
		.member_("Rotate", &Transform::mRotate)
		.member_("Translate", &Transform::mTranslate);
}

//void RegisterMeshRenderer()
//{
//
//}

void RegisterBoxCollider()
{
	meta::class_<BoxCollider>()
		.member_("Scale Offset BC", &BoxCollider::mScaleOffset)
		.member_("Translate Offset BC", &BoxCollider::mTranslateOffset)
		.member_("Is Trigger BC", &BoxCollider::mIsTrigger);
}

void RegisterSphereCollider()
{
	meta::class_<SphereCollider>()
		.member_("Scale Offset SC", &SphereCollider::mScaleOffset)
		.member_("Translate Offset SC", &SphereCollider::mTranslateOffset)
		.member_("Is Trigger SC", &SphereCollider::mIsTrigger);
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

