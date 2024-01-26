#pragma once

#ifndef GRAPHICS_SYSTEM_FUNCTIONS_H
#define GRAPHICS_SYSTEM_FUNCTIONS_H

#include <ECS/ECS_Components.h>

// init
void InitUIMeshes();
void InitFramebuffers();


// update
void animationsToChangeMeshDelayedCheck(Entity inst, const float& dt);
void updateAnimations(Entity inst, const mat4& final, const float& dt);
void updateBloomValues(GFX::Mesh& meshinst);
void updateMaterials(MeshRenderer& meshrenderer);
void update_CalculateLTW(Entity inst, GFX::Mesh& meshinst, Transform& transforminst, mat4 & final);
void update_NonMeshrendererColliders();
void updateSSBO_Data();
void update_Light();
void update_UI();
void update_Portals();
void Reset_Data();

// draw
void render_Debugdraw(const mat4& camVP);

#endif // !GRAPHICS_SYSTEM_FUNCTIONS_H
