#pragma once
#include "ECS_systems.h"

//class ColliderTransformManager : public System {
//public:
//	void			SetScale(const Entity& e, const glm::vec2& scale);
//	void			SetRotation(const Entity& e, float rotation);
//	void			SetTranslate(const Entity& e, const glm::vec2& translation);
//
//	glm::vec2		GetScale(const Entity& e) { return e.GetComponent<ColliderTransform>().scale; }
//	float			GetRotation(const Entity& e) { return e.GetComponent<ColliderTransform>().rotation; }
//	glm::vec2		GetTranslate(const Entity& e) { return e.GetComponent<ColliderTransform>().translation; }
//
//	void			SetRenderFlag(const Entity& e, bool isRender);
//	bool			GetRenderFlag(const Entity& e) { return e.GetComponent<ColliderTransform>().renderFlag; }
//
//	void			SetMatchFlag(const Entity& e, bool isMatch);
//	bool			GetMatchFlag(const Entity& e) { return e.GetComponent<ColliderTransform>().matchFlag; }
//};
