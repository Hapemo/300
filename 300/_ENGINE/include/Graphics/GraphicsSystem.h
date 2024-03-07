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
#include <MsFbo.hpp>
#include <Animator.hpp>
#include <Ssbo.hpp>
#include <ComputeShader.hpp>

#include <Graphics/PostProcessing.h>
#include <Graphics/ParticleSystem.h>

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

struct SpotLightSSBO
{
	alignas(16) vec4 mPosition;
	alignas(16) vec4 mDirection { 0.f, -1.f, 0.f, 0.f };
	alignas(16) vec4 mColor { 1.f, 1.f, 1.f, 0.f };
	float mCutoff;
	float mOuterCutoff;
	float mIntensity;
	float mPad{};
};

struct MaterialSSBO
{
	alignas(8) GLuint64 mDiffuseMap		{};
	alignas(8) GLuint64 mAOMap			{};
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

	void Draw(float dt, bool forEditor = false);

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
	// Adds an instance of a mesh to be drawn
	void AddInstance(GFX::Mesh& mesh, Transform transform, const vec4& color, int meshID, const vec4& bloomthreshold, unsigned entityID = 0xFFFFFFFF);
	void AddInstance(GFX::Mesh& mesh, mat4 transform, const vec4& color, int meshID, const vec4& bloomthreshold, unsigned entityID = 0xFFFFFFFF, int animInstanceID = -1);


	// -- FBO --
	/***************************************************************************/
	/*!
	\brief
		Getters
	*/
	/**************************************************************************/
	GFX::Camera GetCamera(CAMERA_TYPE type);
	vec3 GetCameraPosition(CAMERA_TYPE type);
	vec3 GetCameraTarget(CAMERA_TYPE type);
	vec3 GetCameraDirection(CAMERA_TYPE type);			// Direction vector of the camera (Target - position)
	mat4 GetCameraViewMatrix(CAMERA_TYPE type);
	mat4 GetCameraViewProjMatrix(CAMERA_TYPE type);
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
	void SetAllEntityBloomThreshold(glm::vec4 threshold, std::string meshName);
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

	void PrintDeviceInfo();

	// Top-left position as 0, 0. normalized coordinates [0, 1]
	void ClampCursor();

	void HideCursor(bool hideCursor);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//		MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES MEMBER VARIABLES
	// 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// -- Renderer --
	GFX::DebugRenderer			m_Renderer;				// isolated to debug draws
	GFX::FBO					m_Fbo;					// Editor Scene
	GFX::FBO					m_GameFbo;				// Game Scene
	GFX::MsFBO					m_MultisampleFBO;		// Multisample FBO
	GFX::IntermediateFBO		m_IntermediateFBO;		// Intermediate FBO
	GFX::PingPongFBO			m_PingPongFbo;			// Post Processing
	PhysBasedBloomRenderer		m_PhysBloomRenderer;	// Bloom


	// -- Window --
	GFX::Window* m_Window;
	int					m_Width;
	int					m_Height;
	bool				m_IsCursorEnabledForEditor{ true };

	// -- Camera --
	GFX::Camera m_EditorCamera;
	CAMERA_TYPE m_CameraControl;

	// -- Global tint --
	vec4	m_GlobalTint = { 1.f, 1.f, 1.f, 1.f };

	// -- Bloom -- 
	vec3		mAmbientBloomThreshold{ 0.05, 0.05, 0.005 };	// Global bloom threshold	
	float		mAmbientBloomExposure{ 0.4f };
	float		mTexelOffset{ 1.f };							// Gaussian blur Ver1						
	float		mSamplingWeight{ 1.f };							// Gaussian blur Ver1/2
	float		mFilterRadius{ 0.001f };						// Phys Based Bloom

	BloomType	mBloomType{ BloomType::PHYS_BASED_BLOOM };		// defaults to Phys based bloom

	// -- Chromatic Abbreation --
	float		mChromaticOffset{ 0.006f };
	float		mChromaticStrength{ 1.f };

	bool		m_EnableBloom{ true };
	bool		m_EnableChromaticAbberation{ true };
	bool		m_EnableCRT{ false };						// this yj

	// -- Textures --
	std::vector<int> m_Textures;	// 0, 1, ..., 31

	// -- Shader Instance
	GFX::Shader m_UiShaderInst;
	GFX::Shader m_CrosshairShaderInst;
	GFX::Shader m_DrawSceneShaderInst;
	GFX::Shader m_HealthbarShaderInst;
	GFX::Shader m_AnimationShaderInst;
	GFX::Shader m_GBufferShaderInst;
	GFX::Shader m_DeferredLightShaderInst;
	GFX::Shader m_Quad3DShaderInst;

	// -- Flags --
	int		m_DebugDrawing{ 0 };			// debug drawing 
	bool	m_EditorMode;
	bool	m_EnableGlobalAnimations{ 1 };
	bool	m_HasLight{ false };
	bool    m_EnableScroll{ false };
	bool	m_EditorSceneHovered{ false };
	bool    m_RightClickHeld{ false };
	bool	m_SystemInitialized{ false };

	// -- Stats --
	int		m_LightCount{};
	int		m_SpotlightCount{};

	// -- 2D Image Rendering --
	GFX::Mesh						m_Image2DMesh;
	GFX::Mesh						m_HealthbarMesh;
	GFX::Mesh						m_WorldUIMesh;
	GFX::Mesh						m_ParticleMesh;
	std::vector<unsigned>			m_Image2DStore;
	GFX::Quad2D						mScreenQuad;

	void Unload();

	// -- SSBO -- 
	const int						MAX_INSTANCES = 1000;
	GFX::SSBO						m_FinalBoneMatrixSsbo;
	std::vector<mat4>				finalBoneMatrices;

	const int						MAX_POINT_LIGHT = 200;
	GFX::SSBO						m_PointLightSsbo;
	std::vector<PointLightSSBO>		pointLights;

	GFX::SSBO						m_MaterialSsbo;
	std::vector<MaterialSSBO>		m_Materials;
	std::map<unsigned, GLuint64>	m_MaterialHandles;

	const int						MAX_SPOTLIGHT = 50;
	GFX::SSBO						m_SpotlightSsbo;
	std::vector<SpotLightSSBO>		spotlights;

	const int						MAX_UI = 1000;
	GFX::SSBO						m_UITextureSsbo;
	std::map<unsigned, GLuint64>	m_UIHandles;
	std::vector<GLuint64>			UITextures;

	const int							MAX_PARTICLE_EMITTER = 50;
	const int							MAX_PARTICLES = 100'000;
	GFX::SSBO							m_ParticleEmitterSsbo;
	GFX::SSBO							m_ParticleSsbo;
	GFX::SSBO							m_ParticleFreelistSsbo;
	std::vector<ParticleEmitterSSBO>	m_Emitters;

	GLuint64 GetAndStoreBindlessTextureHandle(int texID);	// Stores adn Return the 64bit texture handle and makes it resident
	int StoreUITexture(int texID);			// Make texture resident, store handle into map, returns the index of the handle
	void SetupShaderStorageBuffers();		// Creates all SSBO required

	void DrawAll2DInstances(unsigned shaderID);
	void Add2DImageInstance(float width, float height, vec3 const& position, unsigned texHandle, unsigned entityID = 0xFFFFFFFF, float degree = 0.f, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f }, float slider = 1.f);
	void Add2DImageWorldInstance(Transform transform, unsigned texHandle, unsigned entityID = 0xFFFFFFFF, float degree = 0.f, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f }, float slider = 1.f);
	int StoreTextureIndex(unsigned texHandle);

	// -- Health Bar --
	void AddHealthbarInstance(Entity e, const vec3& camPos, unsigned texHandle = 0, bool forFrame = false);
	void DrawAllHealthbarInstance(const mat4& viewProj);
	GLint m_HealthbarViewProjLocation{};

	// -- Crosshair --
	GLint m_CrosshairThicknessLocation{};
	GLint m_CrosshairInnerLocation{};
	GLint m_CrosshairOuterLocation{};
	GLint m_CrosshairColorLocation{};
	void SetupCrosshairShaderLocations();
	void DrawCrosshair();

	// -- Deferred Lighting WIP --
	void DrawDeferredLight(const vec3& camPos, GFX::FBO& destFbo);
	void BlitMultiSampleToDestinationFBO(GFX::FBO& destFbo);
	GLint m_DeferredCamPosLocation{};
	GLint m_DeferredLightCountLocation{};

	// -- Compute Shader WIP --
	GFX::ComputeShader computeDeferred;
	void ComputeDeferredLight(bool editorDraw = false);
	GLint m_ComputeDeferredGlobalTintLocation{};
	GLint m_ComputeDeferredCamPosLocation{};
	GLint m_ComputeDeferredLightCountLocation{};
	GLint m_ComputeDeferredSpotlightCountLocation{};
	GLint m_ComputeDeferredGlobalBloomLocation{};

	GFX::ComputeShader m_ComputeCRTShader;
	GLint m_ComputeCRTTimeLocation{};
	GLint m_ComputeCRTHeightOffsetLocation{};
	GLint m_ComputeCRTDistortionLocation{};
	GLint m_ComputeCRTChromaticAbberationLocation{};

	GFX::ComputeShader m_ComputeAddBlendShader;
	GLint m_ComputeAddBlendExposureLocation{};

	// -- Shader Setup --
	void SetupAllShaders();

	// -- Portal WIP --
	mat4 GetPortalViewMatrix(GFX::Camera const& camera, Transform const& sourcePortal, Transform const& destPortal);
	mat4 ObliqueNearPlaneClipping(mat4 proj, mat4 view, Transform const& srcPortal, Transform const& destPortal);
	void AddPortalInstance(Entity portal);
	void DrawAllWorldUI(bool editorDraw);

	// -- Particles WIP --
	//void AddParticleInstance(Particle const& p, vec3 const& camPos);
	void DrawAllParticles(bool forEditor);
	GFX::ComputeShader m_ComputeEmitterShader;
	GFX::ComputeShader m_ComputeParticleShader;
	GFX::Shader m_ParticleShaderInst{};
	std::vector<ParticleSSBO> particleVector{};
	GLuint m_ComputeEmitterCountLocation{};
	GLuint m_ComputeEmitterCamPosLocation{};
	GLuint m_ComputeEmitterSeedLocation{};
	GLuint m_ComputeParticleDeltaTimeLocation{};
	GLuint m_ComputeParticleCountLocation{};
	GLuint m_ComputeParticleCamPosLocation{};
	void ProcessEmitterAndParticle(vec3 const& camPos, float dt);
	void UpdateEmitters(vec3 const& camPos);
	void UpdateParticles(vec3 const& camPos, float dt);
	void EmitParticles(ParticleEmitter& e, vec3 const& position);

	// -- Shadows WIP --
	GFX::Shader shadowMapShaderInst;
	GLuint shadowMapLightSpaceMatrixLocation{};
	GLuint computePBRLightSpaceMatrixLocation{};
	GLuint m_ComputeDirLightPosLocation{};
	GFX::ShadowFBO m_ShadowFbo;
	mat4 lightSpaceMatrix{};
	vec3 dirLightPos{ -0.4f, 12.f, 74.f };
	//vec3 dirLightDir{ 0.3f, -1.f, -1.f };
	//vec2 dirLightSize{ 108.f, 128.f };
	//vec2 dirLightNearFar{ 0.1f, 90.7f };
	void RenderShadowMap();
};

#endif
