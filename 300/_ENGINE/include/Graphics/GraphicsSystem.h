/*!*************************************************************************
****
\file			GraphicsSystem.h
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The declarations for the graphics system that works within the ECS

****************************************************************************
***/

#pragma once
#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>

#include <DebugRenderer.hpp>
#include <Camera.hpp>
#include <Fbo.hpp>
#include <Animator.hpp>
#include <Ssbo.hpp>

/***************************************************************************/
/*!
\brief
	the types of camera present/
	this is used to choose which camera we should update
*/
/**************************************************************************/
enum class CAMERA_TYPE
{
	CAMERA_TYPE_GAME,
	CAMERA_TYPE_EDITOR,
	CAMERA_TYPE_ALL
};

struct PointLightSSBO
{
	alignas(16) vec4 mPosition;
	alignas(16) vec4 mColor{ 1.f, 1.f, 1.f, 0.f};
	float mLinear;
	float mQuadratic;
	float mIntensity;
	float mPad{};
};

struct MaterialSSBO
{
	alignas(8) GLuint64 mDiffuseMap		{};
	alignas(8) GLuint64 mNormalMap		{};
	alignas(8) GLuint64 mSpecularMap	{};
	alignas(8) GLuint64 mShininessMap	{};
	alignas(8) GLuint64 mEmissionMap	{};
};


/***************************************************************************/
/*!
\brief
	The graphics system class declarations
*/
/**************************************************************************/
class GraphicsSystem
{
public:

	/***************************************************************************/
	/*!
	\brief
		the default constructor for the graphics system
	*/
	/**************************************************************************/
	GraphicsSystem() = default;

	/***************************************************************************/
	/*!
	\brief
		initializes the graphics system.
		initializes the fbos, cameras, window dimensions
	*/
	/**************************************************************************/
	void Init();

	/***************************************************************************/
	/*!
	\brief
		Updates the graphics system, the camera, and the renderer.
		pushes the calculated data into the renderer for instanced rendering
	*/
	/**************************************************************************/
	void Update(float dt);

	/***************************************************************************/
	/*!
	\brief
		the draw function isolated to the game's framebuffer
	*/
	/**************************************************************************/
	void GameDraw(float dt);

	/***************************************************************************/
	/*!
	\brief
		the draw function isolated to the editor's framebuffer
	*/
	/**************************************************************************/
	void EditorDraw(float dt);

	/***************************************************************************/
	/*!
	\brief
		the additive draw function isolated to the editor's framebuffer
	*/
	/**************************************************************************/
	void AdditiveBlendFramebuffers( GFX::FBO& targetFramebuffer, unsigned int Attachment0, unsigned int Attachment1);


	/***************************************************************************/
	/*!
	\brief
		the additive blend function for the chromatic abbrebation
	*/
	/**************************************************************************/
	void ChromaticAbbrebationBlendFramebuffers(GFX::FBO& targetFramebuffer, unsigned int Attachment1);




	/***************************************************************************/
	/*!
	\brief
		Draws the game scene to default framebuffer when not running in editor
		mode
	*/
	/**************************************************************************/
	void DrawGameScene();

	/***************************************************************************/
	/*!
	\brief
		exit the graphics system. unallocates resources
	*/
	/**************************************************************************/
	void Exit();

	// -- Mesh --
	/***************************************************************************/
	/*!
	\brief
		Adds an instance of the mesh to be drawn. Populates the LTW matrix, and
		the entity ID for object identification.
		This function is essential for instanced rendering
	*/
	/**************************************************************************/
	void AddInstance(GFX::Mesh& mesh, Transform transform, const vec4& color, int meshID, unsigned entityID = 0xFFFFFFFF);		// Adds an instance of a mesh to be drawn
	void AddInstance(GFX::Mesh& mesh, mat4 transform, const vec4& color, int meshID, unsigned entityID = 0xFFFFFFFF, int animInstanceID = -1);	// Adds an instance of a mesh to be drawn

	// -- FBO --
	/***************************************************************************/
	/*!
	\brief
		Getters
	*/
	/**************************************************************************/
	vec3 GetCameraPosition(CAMERA_TYPE type);
	vec3 GetCameraTarget(CAMERA_TYPE type);
	vec3 GetCameraDirection(CAMERA_TYPE type);			// Direction vector of the camera (Target - position)
	mat4 GetCameraViewMatrix(CAMERA_TYPE type);
	ivec2 GetCameraSize(CAMERA_TYPE type);



	GFX::DebugRenderer& getDebugRenderer() { 
		return m_Renderer; 
	}
	unsigned int GetGameAttachment() {
		return m_GameFbo.GetColorAttachment(); 
	}
	unsigned int GetEditorAttachment() {
		return m_Fbo.GetColorAttachment(); 
	}
	unsigned int GetEntityID(float x, float y) {
		return m_Fbo.ReadEntityID(x, y); 
	}
	void EnableGlobalBloom() { m_EnableBloom = true; }
	void DisableGlobalBloom() { m_EnableBloom = false; }

	void SetGlobalBloomThreshold(glm::vec3 threshold) { mAmbientBloomThreshold = threshold; }
	void SetGlobalBloomExposure(float exp) { mAmbientBloomExposure = exp; }

	// -- Camera Functions --
	/***************************************************************************/
	/*!
	\brief
		Setters
	*/
	/**************************************************************************/
	void SetCameraPosition(CAMERA_TYPE type, vec3 position);
	void SetCameraTarget(CAMERA_TYPE type, vec3 position);
	void SetCameraProjection(CAMERA_TYPE type, float fovDegree, ivec2 size, float nearZ, float farZ);
	void SetCameraSize(CAMERA_TYPE type, ivec2 size);

	/***************************************************************************/
	/*!
	\brief
		Updates the camera according to the enum and dt
	*/
	/**************************************************************************/
	void UpdateCamera(CAMERA_TYPE type, const float&);

	/***************************************************************************/
	/*!
	\brief
		Renders all instances of a given mesh
	*/
	/**************************************************************************/
	void DrawAll(GFX::Mesh& mesh);		// Renders all instances of a given mesh

	/***************************************************************************/
	/*!
	\brief
		Helper functions
	*/
	/**************************************************************************/
	void PrintMat4(const glm::mat4& input);
	void PauseGlobalAnimation();
	void UnpauseGlobalAnimation();

	void CheckWindowSize();

	void ResizeWindow(ivec2 newSize);

	// Top-left position as 0, 0. normalized coordinates [0, 1]
	void ClampCursor();

	void HideCursor(bool hideCursor);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//		MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// -- Renderer --
	GFX::DebugRenderer	m_Renderer;			// isolated to debug draws
	GFX::FBO			m_Fbo;				// Editor Scene
	GFX::FBO			m_GameFbo;			// Game Scene
	GFX::PingPongFBO	m_PingPongFbo;		// Post Processing

	// -- Window --
	GFX::Window*		m_Window;
	int					m_Width;
	int					m_Height;

	// -- Camera --
	GFX::Camera m_EditorCamera;
	CAMERA_TYPE m_CameraControl;



	// -- Global tint --
	vec4	m_GlobalTint = {1.f, 1.f, 1.f, 1.f};

	// -- Bloom -- 
	vec3		mAmbientBloomThreshold { 0.05, 0.05, 0.005 };		// this yj
	float		mAmbientBloomExposure{ 0.4f };						// this yj
	float		mTexelOffset{ 1.f };								// this yj
	float		mSamplingWeight{ 1.f };								// this yj

	// -- Chromatic Abbreation --
	float		mChromaticStrength{ 0.006f };						// this yj

	bool		m_EnableBloom{ false };									// this yj
	bool		m_EnableChromaticAbberation{ false };					// this yj


	// -- Textures --
	std::vector<int> m_Textures;	// 0, 1, ..., 31

	// -- Shader Instance
	GFX::Shader m_UiShaderInst;
	GFX::Shader m_CrosshairShaderInst;
	GFX::Shader m_DrawSceneShaderInst;

	// -- Flags --
	int		m_DebugDrawing{ 1 };			// debug drawing 
	bool	m_EditorMode;
	bool	m_EnableGlobalAnimations{ 1 };
	bool	m_HasLight{ false };
	bool    m_EnableScroll{ false };
	bool	m_EditorSceneHovered{ false };
	bool    m_RightClickHeld	{ false };
	bool	m_SystemInitialized{ false };

	// -- Stats --
	int		m_LightCount{};

	// -- TEST --
	float m_CrosshairThickness	{ 1.f };
	float m_CrosshairInner		{ 20.f };
	float m_CrosshairOuter		{ 50.f };
	float m_Health				{ 100.f };

	void Unload();

private:
	// -- SSBO -- 
	const int						MAX_INSTANCES = 1000;
	GFX::SSBO						m_FinalBoneMatrixSsbo;
	std::vector<mat4>				finalBoneMatrices;

	const int						MAX_POINT_LIGHT = 5;
	GFX::SSBO						m_PointLightSsbo;
	std::vector<PointLightSSBO>		pointLights;

	GFX::SSBO						m_MaterialSsbo;
	std::vector<MaterialSSBO>		m_Materials;
	std::map<unsigned, GLuint64>	m_MaterialHandles;
	GLuint64 GetAndStoreBindlessTextureHandle(int texID);	// Stores adn Return the 64bit texture handle and makes it resident

	void SetupShaderStorageBuffers();		// Creates all SSBO required

	// -- 2D Image Rendering --
	GFX::Mesh						m_Image2DMesh;
	std::vector<unsigned>			m_Image2DStore;
	GFX::Quad2D						mScreenQuad;

	void DrawAll2DInstances(unsigned shaderID);
	void Add2DImageInstance(float width, float height, vec2 const& position, unsigned texHandle, unsigned entityID = 0xFFFFFFFF, float degree = 0.f, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });
	int StoreTextureIndex(unsigned texHandle);

	// -- Crosshair --
	GLint m_CrosshairThicknessLocation{};
	GLint m_CrosshairInnerLocation{};
	GLint m_CrosshairOuterLocation{};
	GLint m_CrosshairColorLocation{};
	void SetupCrosshairShaderLocations();
	void DrawCrosshair();
};

#endif