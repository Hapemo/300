/*!*************************************************************************
****
\file			GraphicsSystem.cpp
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The function definitions for the functionalities of the graphics system

****************************************************************************
***/
#define  _ENABLE_ANIMATIONS					true
#define  ENABLE_MULTISAMPLE					true	
#define  _TEST_HEALTHBAR_SHADER				false
#define  ENABLE_UI_IN_EDITOR_SCENE			true
#define  ENABLE_CROSSHAIR_IN_EDITOR_SCENE	false
#define  TEST_DEFERRED_LIGHT				true	

#include <ECS/ECS_Components.h>
#include <Graphics/GraphicsSystem.h>
#include "ResourceManagerTy.h"
#include <Graphics/Camera_Input.h>
#include "Debug/EnginePerformance.h"
#include "GameState/GameStateManager.h"
#include "Input/InputMapSystem.h"

#include "cstdlib"


/***************************************************************************/
/*!
\brief
	Initialize the Graphics system
*/
/**************************************************************************/
float first_entitytime{};
float second_entitytime{};



void GraphicsSystem::Init()
{
	if (!m_SystemInitialized)
	{
		m_SystemInitialized = true;

		// -- Setup Storage Buffer Object
		SetupShaderStorageBuffers();

		// -- Setup UI stuffs
		m_HealthbarMesh.Setup2DImageMesh();
		m_Image2DMesh.Setup2DImageMesh();
		for (int i{}; i < 32; ++i)
		{
			m_Textures.emplace_back(i);
		}

		// Get Window Handle
		m_Window = systemManager->GetWindow();
		m_Width = m_Window->size().x;
		m_Height = m_Window->size().y;

		// Update the editor mode flag
		m_EditorMode = systemManager->IsEditor();

		// Create FBO, with the width and height of the window
		m_Fbo.Create(m_Width, m_Height, m_EditorMode);
		m_GameFbo.Create(m_Width, m_Height, m_EditorMode);
		m_MultisampleFBO.Create(m_Width, m_Height);
		m_IntermediateFBO.Create(m_Width, m_Height);
		m_PingPongFbo.Create(m_Width, m_Height);
		m_PhysBloomRenderer.Init(m_Width, m_Height);

		SetupAllShaders();

		if (m_DebugDrawing) {
			m_GlobalTint.a = 0.3f;
		}
		else {
			m_GlobalTint.a = 1.f;
		}
	}

	if (!m_EditorMode)	// If not running as editor
	{
		// Initialize the Draw Scene Shader
		std::string drawSceneShader = "DrawSceneShader";
		uid drawSceneShaderstr(drawSceneShader);
		m_DrawSceneShaderInst = *systemManager->mResourceTySystem->get_Shader(drawSceneShaderstr.id);
	}
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Set Cameras' starting position
	SetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR, {38, 20, 30});									// Position of camera
	SetCameraTarget(CAMERA_TYPE::CAMERA_TYPE_EDITOR, {0, 0, 0});									// Target of camera
	SetCameraProjection(CAMERA_TYPE::CAMERA_TYPE_ALL, 60.f, m_Window->size(), 0.1f, 900.f);			// Projection of camera

	if (m_EditorMode)
	{
		// update both the editor and game camera
		UpdateCamera(CAMERA_TYPE::CAMERA_TYPE_ALL, 0.f);
	}
	else
	{
		// only update the game camera if editor mode is not enabled
		UpdateCamera(CAMERA_TYPE::CAMERA_TYPE_GAME, 0.f);
	}

	computeDeferred.CreateShaderFromFile("../assets/shader_files/computePBR.glsl");
	computeDeferred.Activate();
	m_ComputeDeferredLightCountLocation		= computeDeferred.GetUniformLocation("uLightCount");
	m_ComputeDeferredCamPosLocation			= computeDeferred.GetUniformLocation("uCamPos");
	m_ComputeDeferredGlobalTintLocation		= computeDeferred.GetUniformLocation("uGlobalTint");
	m_ComputeDeferredGlobalBloomLocation	= computeDeferred.GetUniformLocation("uGlobalBloomThreshold");
	GFX::Shader::Deactivate();

	// Input
	glBindImageTexture(2, m_IntermediateFBO.GetBrightColorsAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(3, m_IntermediateFBO.GetFragPosAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(4, m_IntermediateFBO.GetNormalAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(5, m_IntermediateFBO.GetAlbedoSpecAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(6, m_IntermediateFBO.GetEmissionAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	PINFO("Window size: %d, %d", m_Window->size().x, m_Window->size().y);
}

/***************************************************************************/
/*!
\brief
	Update the Graphics system. Called every frame
	Updates the camera, and the renderer
*/
/**************************************************************************/
void GraphicsSystem::Update(float dt)
{
	m_RightClickHeld = systemManager->mInputActionSystem->GetKey(M_BUTTON_R);

	// Check window size for any updates
	CheckWindowSize();

	// update the camera's transformations, and its input
	if (m_EditorMode)
	{
		// update both the editor and game camera
		UpdateCamera(CAMERA_TYPE::CAMERA_TYPE_ALL, dt);
	}
	else
	{
		// only update the game camera if editor mode is not enabled
		UpdateCamera(CAMERA_TYPE::CAMERA_TYPE_GAME, dt);
	}

#pragma region update all the mesh instances

	// Retrieve and update the mesh instances to be drawn
	auto meshRendererInstances = systemManager->ecs->GetEntitiesWith<MeshRenderer>();
	int animationID{ 0 };
	bool hasanimator{false};

	for (Entity inst : meshRendererInstances)
	{
		MeshRenderer& meshRenderer = inst.GetComponent<MeshRenderer>();

		// if the mesh instance is not active, skip it
		if (meshRenderer.mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance) == nullptr)
			continue;
		
		// gives me the mesh
		void *tt = meshRenderer.mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance);
		GFX::Mesh &meshinst = *reinterpret_cast<GFX::Mesh *>(tt);

		hasanimator				= inst.HasComponent<Animator>();
		if (hasanimator)
		{
			Animator& animatorInst	= inst.GetComponent<Animator>();

			// handles the change of mesh when called in the scripts
			float lAllowance = animatorInst.mAnimator.m_CurrentAnimation->m_TicksPerSecond * dt;
			if (animatorInst.mAnimator.mToChangeMeshDelayed.first && (animatorInst.mAnimator.m_CurrentTime <= lAllowance))
			{
				// the mesh change delayed flag is set and the animation is completed, change the mesh
				animatorInst.mAnimator.mToChangeMeshDelayed.first = false;
				meshRenderer.SetMesh(animatorInst.mAnimator.mToChangeMeshDelayed.second, inst);
			}
		}

		{
			// Setting the bloom threshold once per loop
			std::string shdr;
			if (meshinst.mHasAnimation)
				shdr = "AnimationShader";
			else
				shdr = "PointLightShader";
			uid shaderstr(shdr);
			GFX::Shader& shaderinst = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);

			vec4 lAmbientBloomThreshold{ 0.f, 0.f, 0.f, 0.f };
			if (systemManager->mGraphicsSystem->m_EnableBloom) {
				lAmbientBloomThreshold = vec4(mAmbientBloomThreshold, 1.f);
			}

			shaderinst.Activate();
			GLuint threshold = shaderinst.GetUniformLocation("globalBloomThreshold");
			glUniform4fv(threshold, 1, glm::value_ptr(lAmbientBloomThreshold));
			shaderinst.Deactivate();
		}

		// pushback LTW matrices
		auto& transforminst = inst.GetComponent<Transform>();
		vec3 trans = transforminst.mTranslate;
		vec3 parent_translate(0.f);

		mat4 S = glm::scale(transforminst.mScale / (meshinst.mBBOX.m_Max - meshinst.mBBOX.m_Min));
		mat4 R = glm::toMat4(glm::quat(glm::radians(transforminst.mRotate)));
		mat4 T = glm::translate(trans);

		if (inst.HasParent())	// Compute parent's offset
		{
			parent_translate = Entity(inst.GetParent()).GetComponent<Transform>().mTranslate;

			// Compute view to world
			if (inst.GetParent().GetComponent<TAG>() == TAG::PLAYER)	// if parent is the Player
			{
				// view space --> world space
				mat4 viewToWorld = glm::inverse(GetCameraViewMatrix(CAMERA_TYPE::CAMERA_TYPE_GAME));
				T = viewToWorld * T;
			}
			else
			{
				trans += parent_translate;
				T = glm::translate(trans);
			}
		}
		mat4 final = T * R * S;

		// if the debug drawing is turned on
		if (m_DebugDrawing && inst.HasComponent<BoxCollider>())
		{
			BoxCollider& boxcolliderinst = inst.GetComponent<BoxCollider>();

			// draw the AABB of the mesh
			glm::vec3 bbox_dimens = transforminst.mScale * boxcolliderinst.mScaleOffset;
			glm::vec3 bbox_xlate = transforminst.mTranslate + boxcolliderinst.mTranslateOffset;
			if (inst.HasParent()) {
				//bbox_xlate += Entity(inst.GetParent()).GetComponent<Transform>().mTranslate;
				bbox_xlate += parent_translate;
			}

			// calculate the transformations
			glm::mat4 bboxScale = glm::scale(bbox_dimens);
			glm::mat4 bboxTranslate = glm::translate(bbox_xlate);
			glm::mat4 bboxFinal = bboxTranslate * R * bboxScale;

			m_Renderer.AddAabb(bboxFinal, {1.f, 0.f, 0.f, 1.f});
		}

		if (m_DebugDrawing && inst.HasComponent<CapsuleCollider>())
		{
			CapsuleCollider& cap = inst.GetComponent<CapsuleCollider>();
			if (inst.HasParent())
				transforminst.mTranslate += static_cast<Entity>(inst.GetParent()).GetComponent<Transform>().mTranslate;

			glm::vec3 capPos = transforminst.mTranslate + cap.mTranslateOffset;

			glm::vec3 first = capPos;
			glm::vec3 second = capPos;
			first.y -= cap.mHalfHeight;
			second.y += cap.mHalfHeight;

			m_Renderer.AddCapsule(m_EditorCamera.position(), first, second, cap.mRadius, glm::vec4(0.f, 1.f, 0.f, 1.f));
		}

		if (m_DebugDrawing)
		{
			// Draw the axes
			static const vec3 origin{-180.f, -100.f, 250.f};
			m_Renderer.AddLine(origin, origin + vec3{ 100.f, 0.f, 0.f }, { 1.f, 0.f, 0.f, 1.f });
			m_Renderer.AddLine(origin, origin + vec3{ 0.f, 100.f, 0.f }, { 0.f, 1.f, 0.f, 1.f });
			m_Renderer.AddLine(origin, origin + vec3{ 0.f, 0.f, 100.f }, { 0.f, 0.f, 1.f, 1.f });
		}

		// Update the animation
		if (hasanimator && _ENABLE_ANIMATIONS && systemManager->mGraphicsSystem->m_EnableGlobalAnimations)
		{
			Animator& animatorInst = inst.GetComponent<Animator>();

			// skip the mesh that does not have an animation set
			if (animatorInst.mAnimator.m_CurrentAnimation != nullptr)
			{
				animatorInst.mAnimator.UpdateAnimation(dt, mat4(1.f), final); // update the current animation

				// push back matrices into the SSBO
				for (const auto& x : animatorInst.mAnimator.m_FinalBoneMatrices)
				{
					finalBoneMatrices.push_back(x);
				}
			}
		}

		// animations are present
		if (hasanimator) {
			AddInstance(meshinst, final, meshRenderer.mInstanceColor, static_cast<int>(m_Materials.size()), meshRenderer.mBloomThreshold, static_cast<unsigned>(inst.id), animationID++);
		}
		else {
			AddInstance(meshinst, final, meshRenderer.mInstanceColor, static_cast<int>(m_Materials.size()), meshRenderer.mBloomThreshold, static_cast<unsigned>(inst.id));
		}

		auto getID = [&](MaterialType type, MeshRenderer& meshrenderer) ->int {

			if (meshrenderer.mTextureRef[static_cast<int>(type)].getdata(systemManager->mResourceTySystem->m_ResourceInstance) == nullptr)
				return -1;

			return static_cast<GFX::Texture*>(meshrenderer.mTextureRef[static_cast<int>(type)].data)->ID();
		};

		MaterialSSBO material{};
		material.mDiffuseMap = GetAndStoreBindlessTextureHandle(getID(DIFFUSE, meshRenderer));
		material.mNormalMap = GetAndStoreBindlessTextureHandle(getID(NORMAL, meshRenderer));
		material.mSpecularMap = GetAndStoreBindlessTextureHandle(getID(SPECULAR, meshRenderer));
		material.mShininessMap = GetAndStoreBindlessTextureHandle(getID(SHININESS, meshRenderer));
		material.mEmissionMap = GetAndStoreBindlessTextureHandle(getID(EMISSION, meshRenderer));

		m_Materials.emplace_back(material);	// push back
	}

	if (m_DebugDrawing)
	{
		auto debugDrawsWithoutMesh = systemManager->ecs->GetEntitiesWith<BoxCollider>();
		for (Entity e : debugDrawsWithoutMesh)
		{
			if (e.HasComponent<MeshRenderer>())
				continue;
			BoxCollider& boxcolliderinst = e.GetComponent<BoxCollider>();

			Transform xform = e.GetComponent<Transform>();
			// draw the AABB of the mesh
			glm::vec3 bbox_dimens = xform.mScale * boxcolliderinst.mScaleOffset;
			glm::vec3 bbox_xlate = xform.mTranslate + boxcolliderinst.mTranslateOffset;
			if (e.HasParent()) {
				//bbox_xlate += Entity(inst.GetParent()).GetComponent<Transform>().mTranslate;
				bbox_xlate += Entity(e.GetParent()).GetComponent<Transform>().mTranslate;
			}
			mat4 R = glm::toMat4(glm::quat(glm::radians(xform.mRotate)));

			// calculate the transformations
			glm::mat4 bboxScale = glm::scale(bbox_dimens);
			glm::mat4 bboxTranslate = glm::translate(bbox_xlate);
			glm::mat4 bboxFinal = bboxTranslate * R * bboxScale;

			m_Renderer.AddAabb(bboxFinal, { 1.f, 0.f, 0.f, 1.f });
		}
	}
	
	m_FinalBoneMatrixSsbo.SubData(finalBoneMatrices.size() * sizeof(mat4), finalBoneMatrices.data());
	finalBoneMatrices.clear();
	m_MaterialSsbo.SubData(m_Materials.size() * sizeof(MaterialSSBO), m_Materials.data());
	m_Materials.clear();

	// Sending Light source data to GPU
	auto lightEntity = systemManager->ecs->GetEntitiesWith<PointLight>();
	m_HasLight = !lightEntity.empty();
	m_LightCount = static_cast<int>(lightEntity.size());

	if (m_HasLight)
	{
		for (int i = 0; i < lightEntity.size(); ++i)
		{
			PointLight& lightData = lightEntity.get<PointLight>(lightEntity[i]);
			Transform& lightTransform = Entity(lightEntity[i]).GetComponent<Transform>();

			PointLightSSBO light;
			light.mPosition = vec4(lightTransform.mTranslate, 0.f);
			light.mColor = vec4(lightData.mLightColor, 0.f);
			light.mIntensity = lightData.mIntensity;
			light.mLinear = lightData.mLinearFalloff;
			light.mQuadratic = lightData.mQuadraticFalloff;

			pointLights.push_back(light);

			m_Renderer.AddCube(lightTransform.mTranslate, { 2, 2, 2 }, vec4(lightData.mLightColor, 1.f));
		}
		// Copy light source data into storage buffer
		m_PointLightSsbo.SubData(pointLights.size() * sizeof(PointLightSSBO), pointLights.data());
		pointLights.clear();
	}

	// UI Objects
	m_Image2DMesh.ClearInstances();		// Clear data from previous frame
	m_Image2DStore.clear();
	auto UiInstances = systemManager->ecs->GetEntitiesWith<UIrenderer>();
	for (Entity inst : UiInstances)
	{
		UIrenderer& uiRenderer = inst.GetComponent<UIrenderer>();
		Transform& uiTransform = inst.GetComponent<Transform>();

		float uiWidth = uiTransform.mScale.x;
		float uiHeight = uiTransform.mScale.y;
		vec2 uiPosition = vec2(uiTransform.mTranslate.x, uiTransform.mTranslate.y);

		unsigned texID{};
		if (uiRenderer.mTextureRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance) != nullptr)
			texID = reinterpret_cast<GFX::Texture*>(uiRenderer.mTextureRef.data)->ID();

		Add2DImageInstance(uiWidth, uiHeight, uiPosition, texID, static_cast<int>(inst.id), uiRenderer.mDegree);
	}
	// Send UI data to GPU
	m_Image2DMesh.PrepForDraw();

#pragma endregion
}

void GraphicsSystem::Draw(bool forEditor)
{
	std::map<std::string, short> renderedMesh;
	auto meshRendererInstances = systemManager->ecs->GetEntitiesWith<MeshRenderer>();

	// Prepare and bind the Framebuffer to be rendered on
	m_MultisampleFBO.PrepForDraw();
	glDisable(GL_BLEND);				// Blending must be disabled for G-Buffer pass

	// Get the Camera View Projection
	mat4 camVP;
	if (forEditor)
		camVP = m_EditorCamera.viewProj();
	else
		camVP = GetCameraViewProjMatrix(CAMERA_TYPE::CAMERA_TYPE_GAME);

	// Update View projection matrix uniform
	m_GBufferShaderInst.Activate();
	glUniformMatrix4fv(m_GBufferShaderInst.GetUniformVP(), 1, GL_FALSE, &camVP[0][0]);
	GFX::Shader::Deactivate();
	m_AnimationShaderInst.Activate();
	glUniformMatrix4fv(m_AnimationShaderInst.GetUniformVP(), 1, GL_FALSE, &camVP[0][0]);
	GFX::Shader::Deactivate();

#pragma region render all the mesh instances onto the editor camera framebuffer
	// Render all instances of a given mesh
	for (Entity inst : meshRendererInstances)
	{
		auto& meshrefptr = inst.GetComponent<MeshRenderer>();
		if (meshrefptr.mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance) == nullptr)
			continue;

		std::string meshstr = meshrefptr.mMeshPath;
		if (renderedMesh.find(meshstr) != renderedMesh.end())
		{
			// the mesh has been rendered before, skip it
			continue;
		}

		// update the map of rendered meshes
		renderedMesh[meshstr] = 1;

		// render the mesh and its instances here
		GFX::Mesh& meshinst = *reinterpret_cast<GFX::Mesh*>(meshrefptr.mMeshRef.data);

		// Activate animation shader
		if (meshinst.mHasAnimation)
			m_AnimationShaderInst.Activate();
		else
			m_GBufferShaderInst.Activate();

		//Bind mesh's VAO, copy render data into VBO, Draw
		DrawAll(meshinst);

		//shaderinst.Deactivate();
		meshinst.UnbindVao();

		if (!forEditor)	// Clear all mesh instances after done drawing for game scene
			meshinst.ClearInstances();
	}
	GFX::Shader::Deactivate();
	if (forEditor)
	{
		m_Renderer.RenderAll(camVP);
		m_Renderer.ClearInstances();
	}

	// Perform blitting over pixel data from Multisample FBO -> intermediate FBO -> Destination FBO
	if (forEditor)
		BlitMultiSampleToDestinationFBO(m_Fbo, true);
	else
		BlitMultiSampleToDestinationFBO(m_GameFbo);

	// Compute the light pass with completed G-Buffers
	ComputeDeferredLight(forEditor);
	glEnable(GL_BLEND);			// Enable back blending for later draws

	if (systemManager->mGraphicsSystem->m_EnableBloom)
	{
		m_PingPongFbo.PrepForDraw();

		// Render the bloom for the Editor Framebuffer
		//uid gaussianshaderstr("GaussianBlurShader");
		//GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
		//m_PingPongFbo.GaussianBlur(gaussianShaderInst, m_Fbo, systemManager->mGraphicsSystem->mTexelOffset, systemManager->mGraphicsSystem->mSamplingWeight);

		uid gaussianshaderstr("GaussianBlurShaderVer2");
		GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
		if (forEditor)
		{
			m_PingPongFbo.GaussianBlurShader(gaussianShaderInst, m_Fbo, systemManager->mGraphicsSystem->mSamplingWeight);
			AdditiveBlendFramebuffers(m_Fbo, m_Fbo.GetColorAttachment(), m_PingPongFbo.pingpongColorbuffers[0]);
		}
		else
		{
			m_PingPongFbo.GaussianBlurShader(gaussianShaderInst, m_GameFbo, systemManager->mGraphicsSystem->mSamplingWeight);
			AdditiveBlendFramebuffers(m_GameFbo, m_GameFbo.GetColorAttachment(), m_PingPongFbo.pingpongColorbuffers[0]);
		}

	}

	if (systemManager->mGraphicsSystem->m_EnableChromaticAbberation)
	{
		//ChromaticAbbrebationBlendFramebuffers(m_Fbo, m_Fbo.GetBrightColorsAttachment());
		if (forEditor)
			ChromaticAbbrebationBlendFramebuffers(m_Fbo, m_PingPongFbo.pingpongColorbuffers[0]);
		else
			ChromaticAbbrebationBlendFramebuffers(m_GameFbo, m_PingPongFbo.pingpongColorbuffers[0]);
	}

	// Set blend function back to usual for UI rendering
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind the appropriate FBO
	if (forEditor)		// Bind Editor FBO
	{
		m_Fbo.Bind();
		m_Fbo.DrawBuffers(true, true);
	}
	else				// Bind Game FBO
	{
		m_GameFbo.Bind();
		m_GameFbo.DrawBuffers(true);
	}

	// Render UI objects
	m_UiShaderInst.Activate();		// Activate shader
	DrawAll2DInstances(m_UiShaderInst.GetHandle());
	GFX::Shader::Deactivate();	// Deactivate shader

	if (ENABLE_CROSSHAIR_IN_EDITOR_SCENE || !forEditor)
		DrawCrosshair();	// Render crosshair, if any

	// Healthbar objects
	auto healthbarInstances = systemManager->ecs->GetEntitiesWith<Healthbar>();
	for (Entity inst : healthbarInstances)
	{
		Healthbar& healthbar = inst.GetComponent<Healthbar>();
		
		if (forEditor)
			AddHealthbarInstance(healthbar, GetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR), static_cast<int>(inst.id));
		else
			AddHealthbarInstance(healthbar, GetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_GAME), static_cast<int>(inst.id));
	}
	m_HealthbarMesh.PrepForDraw();
	DrawAllHealthbarInstance(camVP);
	m_HealthbarMesh.ClearInstances();	// Clear data

#pragma endregion
	// Unbind FBO at end of draw frame
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/***************************************************************************/
/*!
\brief
	The Draw function isolated onto the editor framebuffer
*/
/**************************************************************************/
void GraphicsSystem::EditorDraw(float dt)
{
	(void)dt;

	std::map<std::string, short> renderedMesh;
	auto meshRendererInstances = systemManager->ecs->GetEntitiesWith<MeshRenderer>();

	// Prepare and bind the Framebuffer to be rendered on
	m_MultisampleFBO.PrepForDraw();
#if TEST_DEFERRED_LIGHT
	glDisable(GL_BLEND);
#endif

#if TEST_DEFERRED_LIGHT
	// Update View projection matrix uniform
	m_GBufferShaderInst.Activate();
	glUniformMatrix4fv(m_GBufferShaderInst.GetUniformVP(), 1, GL_FALSE, &m_EditorCamera.viewProj()[0][0]);
	GFX::Shader::Deactivate();
	m_AnimationShaderInst.Activate();
	glUniformMatrix4fv(m_AnimationShaderInst.GetUniformVP(), 1, GL_FALSE, &m_EditorCamera.viewProj()[0][0]);
	GFX::Shader::Deactivate();
#endif

#pragma region render all the mesh instances onto the editor camera framebuffer
	// Render all instances of a given mesh
	for (Entity inst : meshRendererInstances)
	{
		auto& meshrefptr = inst.GetComponent<MeshRenderer>();
		if (meshrefptr.mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance) == nullptr)
			continue;

		std::string meshstr = meshrefptr.mMeshPath;
		if (renderedMesh.find(meshstr) != renderedMesh.end())
		{
			// the mesh has been rendered before, skip it
			continue;
		}

		// update the map of rendered meshes
		renderedMesh[meshstr] = 1;

		// render the mesh and its instances here
		GFX::Mesh &meshinst = *reinterpret_cast<GFX::Mesh *>(meshrefptr.mMeshRef.data);
#if TEST_DEFERRED_LIGHT

		// Activate animation shader
		if (meshinst.mHasAnimation)
			m_AnimationShaderInst.Activate();
		else
			m_GBufferShaderInst.Activate();

#endif

#if !TEST_DEFERRED_LIGHT
		GLuint debug_draw = glGetUniformLocation(shaderID, "globalTint");
		glUniform4fv(debug_draw, 1, glm::value_ptr(m_GlobalTint));
#endif
		 //Bind mesh's VAO, copy render data into VBO, Draw
		DrawAll(meshinst);

		//shaderinst.Deactivate();
		meshinst.UnbindVao();
	}
	m_Renderer.RenderAll(m_EditorCamera.viewProj());
#if TEST_DEFERRED_LIGHT
	GFX::Shader::Deactivate();
#endif

#if ENABLE_MULTISAMPLE
	// Perform blitting over pixel data from Multisample FBO -> intermediate FBO -> Destination FBO
	BlitMultiSampleToDestinationFBO(m_Fbo);
#if TEST_DEFERRED_LIGHT
	//DrawDeferredLight(m_EditorCamera.position(), m_Fbo);
	ComputeDeferredLight(true);
	glEnable(GL_BLEND);
#endif

#endif

	if (systemManager->mGraphicsSystem->m_EnableBloom)
	{
		if (mBloomType == BloomType::PHYS_BASED_BLOOM)
		{
			m_PhysBloomRenderer.PrepForDraw();
			m_PhysBloomRenderer.RenderBloom(m_Fbo.GetBrightColorsAttachment(), mFilterRadius);
			PostProcessing::AdditiveBlendFramebuffers(m_Fbo, m_Fbo.GetColorAttachment(), m_PhysBloomRenderer.getBloomTexture());
		}

		else
		{
			//Render the bloom for the Editor Framebuffer
			m_PingPongFbo.PrepForDraw();
	
			if (mBloomType == BloomType::GAUSSIANBLUR)
			{
				uid gaussianshaderstr("GaussianBlurShader");
				GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
				m_PingPongFbo.GaussianBlur(gaussianShaderInst, m_Fbo, systemManager->mGraphicsSystem->mTexelOffset, systemManager->mGraphicsSystem->mSamplingWeight);
			}

			else if (mBloomType == BloomType::GAUSSIANBLUR_VER2)
			{
				uid gaussianshaderstr("GaussianBlurShaderVer2");
				GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
				m_PingPongFbo.GaussianBlurShader(gaussianShaderInst, m_Fbo, systemManager->mGraphicsSystem->mSamplingWeight);
			}

			PostProcessing::AdditiveBlendFramebuffers(m_Fbo, m_Fbo.GetColorAttachment(), m_PingPongFbo.pingpongColorbuffers[0]);
		}
	}

	if (systemManager->mGraphicsSystem->m_EnableChromaticAbberation)
	{
		if (mBloomType == BloomType::PHYS_BASED_BLOOM) {
			PostProcessing::ChromaticAbbrebationBlendFramebuffers(m_Fbo, m_PhysBloomRenderer.getBloomTexture());
		}
		else {
			PostProcessing::ChromaticAbbrebationBlendFramebuffers(m_Fbo, m_PingPongFbo.pingpongColorbuffers[0]);
		}
	}


#if ENABLE_UI_IN_EDITOR_SCENE || ENABLE_CROSSHAIR_IN_EDITOR_SCENE
	m_Fbo.Bind();
	m_Fbo.DrawBuffers(true, true);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

#if ENABLE_UI_IN_EDITOR_SCENE
	// Render UI objects
	m_UiShaderInst.Activate();		// Activate shader
	DrawAll2DInstances(m_UiShaderInst.GetHandle());
	GFX::Shader::Deactivate();	// Deactivate shader
#endif

#if ENABLE_CROSSHAIR_IN_EDITOR_SCENE
	// Render crosshair, if any
	DrawCrosshair();
#endif
	 // Healthbar objects
	 auto healthbarInstances = systemManager->ecs->GetEntitiesWith<Healthbar>();
	 for (Entity inst : healthbarInstances)
	 {
		 Healthbar& healthbar = inst.GetComponent<Healthbar>();

		 AddHealthbarInstance(healthbar, GetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR), static_cast<int>(inst.id));
	 }
	 m_HealthbarMesh.PrepForDraw();
	 DrawAllHealthbarInstance(m_EditorCamera.viewProj());
	 m_HealthbarMesh.ClearInstances();	// Clear data

#pragma endregion

	m_Fbo.Unbind();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/*!
\brief
	The draw function isolated onto the game's framebuffer
*/
/**************************************************************************/
void GraphicsSystem::GameDraw(float dt)
{
	(void)dt;

	std::map<std::string, short> renderedMesh;
	auto meshRendererInstances = systemManager->ecs->GetEntitiesWith<MeshRenderer>();

	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return; // Cannot find camera Richmond
	Entity camera = localcamera.front();
	mat4 gameCamVP = camera.GetComponent<Camera>().mCamera.viewProj();

#if ENABLE_MULTISAMPLE
	m_MultisampleFBO.PrepForDraw();
#else
	// Prepare and bind the Framebuffer to be rendered on
	m_GameFbo.PrepForDraw();
#endif

	// Render all instances of a given mesh
	for (Entity inst : meshRendererInstances)
	{

		auto& meshrefptr = inst.GetComponent<MeshRenderer>();
		if (meshrefptr.mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance) == nullptr)
			continue;

		std::string meshstr = meshrefptr.mMeshPath;
		if (renderedMesh.find(meshstr) != renderedMesh.end())
		{
			// the mesh has been rendered before, skip it
			continue;
		}

		// update the map of rendered meshes
		renderedMesh[meshstr] = 1;

		// render the mesh and its instances here
		GFX::Mesh &meshinst = *reinterpret_cast<GFX::Mesh *>(meshrefptr.mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance));

		// gets the shader filepath
		std::string shader{};
		if (meshinst.mHasAnimation)
			shader = "AnimationShader";
		else
			shader = "PointLightShader";

		uid shaderstr(shader);
		GFX::Shader &shaderinst = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);

		shaderinst.Activate();

		mat4 gameCamVP = camera.GetComponent<Camera>().mCamera.viewProj();
		glUniformMatrix4fv(shaderinst.GetUniformVP(), 1, GL_FALSE, &gameCamVP[0][0]);

		// Retrieve Point Light object
		auto lightEntity = systemManager->ecs->GetEntitiesWith<PointLight>();
		m_HasLight = !lightEntity.empty();
		GLuint mHasLightFlagLocation = shaderinst.GetUniformLocation("uHasLight");
		glUniform1i(mHasLightFlagLocation, m_HasLight);

		// Factor in Lighting if any
		if (m_HasLight)
		{
			GLuint mViewPosShaderLocation = shaderinst.GetUniformLocation("uViewPos");
			vec3 viewPos = camera.GetComponent<Camera>().mCamera.position();
			glUniform3fv(mViewPosShaderLocation, 1, &viewPos[0]);

			// Light count uniform
			GLuint mLightCountShaderLocation = shaderinst.GetUniformLocation("uLightCount");
			glUniform1i(mLightCountShaderLocation, m_LightCount);
		}

		// Bind mesh's VAO, copy render data into VBO, Draw
		DrawAll(meshinst);

		GLuint globalTintLocation = glGetUniformLocation(shaderinst.GetHandle(), "globalTint");
		glUniform4fv(globalTintLocation, 1, glm::value_ptr(vec4(1.f, 1.f, 1.f, 1.f)));

		meshinst.UnbindVao();
		shaderinst.Deactivate();

		// unbind the textures
		for (int i{0}; i < 4; i++)
		{
			if (meshrefptr.mTextureRef[i].data != nullptr)
			{
				glBindTextureUnit(i, 0);
			}
		}

		meshinst.ClearInstances();
		m_Renderer.ClearInstances();
	}

#if ENABLE_MULTISAMPLE
	m_GameFbo.Clear();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Copy pixel data from multisample FBO to editor FBO
	m_MultisampleFBO.BlitFramebuffer(m_GameFbo.GetID());
#endif

	if (systemManager->mGraphicsSystem->m_EnableBloom)
	{
		m_PingPongFbo.PrepForDraw();

		// Render the bloom for the Game Framebuffer

#if 0
		uid gaussianshaderstr("GaussianBlurShader");
		GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
		m_PingPongFbo.GaussianBlur(gaussianShaderInst, m_GameFbo, systemManager->mGraphicsSystem->mTexelOffset, systemManager->mGraphicsSystem->mSamplingWeight);
#else
		uid gaussianshaderstr("GaussianBlurShaderVer2");
		GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
		m_PingPongFbo.GaussianBlurShader(gaussianShaderInst, m_GameFbo, systemManager->mGraphicsSystem->mSamplingWeight);
#endif

		AdditiveBlendFramebuffers(m_GameFbo, m_GameFbo.GetColorAttachment(), m_PingPongFbo.pingpongColorbuffers[0]);
	}

	if (systemManager->mGraphicsSystem->m_EnableChromaticAbberation)
	{
		//ChromaticAbbrebationBlendFramebuffers(m_GameFbo, m_GameFbo.GetBrightColorsAttachment());
		PostProcessing::ChromaticAbbrebationBlendFramebuffers(m_GameFbo, m_PingPongFbo.pingpongColorbuffers[0]);
	}

	m_GameFbo.Bind();
	m_GameFbo.DrawBuffers(true);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render UI objects
	m_UiShaderInst.Activate();		// Activate shader
	DrawAll2DInstances(m_UiShaderInst.GetHandle());
	m_UiShaderInst.Deactivate();	// Deactivate Shader

	// Render crosshair, if any
	DrawCrosshair();

	// Healthbar objects
	auto healthbarInstances = systemManager->ecs->GetEntitiesWith<Healthbar>();
	for (Entity inst : healthbarInstances)
	{
		Healthbar& healthbar = inst.GetComponent<Healthbar>();

		AddHealthbarInstance(healthbar, GetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_GAME), static_cast<int>(inst.id));
	}
	m_HealthbarMesh.PrepForDraw();
	DrawAllHealthbarInstance(gameCamVP);
	m_HealthbarMesh.ClearInstances();	// Clear data

	if (systemManager->mGraphicsSystem->m_EnableCRT && !systemManager->mGraphicsSystem->m_EnableChromaticAbberation)
	{
		// CRT post processing effect. Called here so it can be rendered over the UI
		PostProcessing::CRTBlendFramebuffers(m_GameFbo, m_PingPongFbo, dt);
	}

	if (systemManager->mGraphicsSystem->m_EnableBloom)
	{
		if (mBloomType == BloomType::PHYS_BASED_BLOOM)
		{
			m_PhysBloomRenderer.PrepForDraw();
			//m_PhysBloomRenderer.RenderBloom(m_GameFbo.GetColorAttachment(), mFilterRadius);
			m_PhysBloomRenderer.RenderBloom(m_GameFbo.GetBrightColorsAttachment(), mFilterRadius);
			PostProcessing::AdditiveBlendFramebuffers(m_GameFbo, m_GameFbo.GetColorAttachment(), m_PhysBloomRenderer.getBloomTexture());
		}

		else
		{
			// Render the bloom for the Game Framebuffer
			m_PingPongFbo.PrepForDraw();

			if (mBloomType == BloomType::GAUSSIANBLUR)
			{
				uid gaussianshaderstr("GaussianBlurShader");
				GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
				m_PingPongFbo.GaussianBlur(gaussianShaderInst, m_GameFbo, systemManager->mGraphicsSystem->mTexelOffset, systemManager->mGraphicsSystem->mSamplingWeight);
			}

			else if (mBloomType == BloomType::GAUSSIANBLUR_VER2)
			{
				uid gaussianshaderstr("GaussianBlurShaderVer2");
				GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
				m_PingPongFbo.GaussianBlurShader(gaussianShaderInst, m_GameFbo, systemManager->mGraphicsSystem->mSamplingWeight);
			}

			PostProcessing::AdditiveBlendFramebuffers(m_GameFbo, m_GameFbo.GetColorAttachment(), m_PingPongFbo.pingpongColorbuffers[0]);
		}
	}

	m_GameFbo.Unbind();
	m_PingPongFbo.UnloadAndClear();
}






void GraphicsSystem::DrawGameScene()
{
	// Clear Default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// back to default framebuffer
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);				// Depth testing must be disabled!!

	// Activate shader program
	m_DrawSceneShaderInst.Activate();
	m_Image2DMesh.BindVao();										// Bind VAO
	glBindTexture(GL_TEXTURE_2D, m_GameFbo.GetColorAttachment());	// Bind texture to be drawn

	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 1);				// DRAW

	m_Image2DMesh.BindVao();										// Unbind VAO
	glBindTexture(GL_TEXTURE_2D, 0);								// Unbind texture after drawing

	// Deactivate shader program
	m_DrawSceneShaderInst.Deactivate();

	glEnable(GL_DEPTH_TEST);
}

/***************************************************************************/
/*!
\brief
	Exit the Graphics system. Unalloc anything unnecessary
*/
/**************************************************************************/
void GraphicsSystem::Exit()
{
	m_Image2DMesh.Destroy();

}

/***************************************************************************/
/*!
\brief
	Adds an instance of a mesh to be drawn, For instancing
*/
/**************************************************************************/
void GraphicsSystem::AddInstance(GFX::Mesh &mesh, Transform transform, const vec4& color, int meshID, const vec4& bloomthreshold, unsigned entityID)
{
	// Local to world transformation
	mat4 scale = glm::scale(transform.mScale);
	mat4 translate = glm::translate(transform.mTranslate);

	glm::quat quatRotate = glm::quat(transform.mRotate); // Retrieve quaternions via euler angles
	glm::mat4 rotation = glm::toMat4(quatRotate);		 // Retrieve rotation matrix via quaternions

	mat4 world = translate * rotation * scale;
	mesh.mLTW.push_back(world);
	mesh.mTexEntID.push_back(vec4((float)meshID + 0.5f, (float)entityID + 0.5f, 0, 0));
	mesh.mColors.push_back(color);
	mesh.mBloomThresholds.push_back(bloomthreshold);
}

void GraphicsSystem::AddInstance(GFX::Mesh &mesh, mat4 transform, const vec4& color, int meshID, const vec4& bloomthreshold, unsigned entityID, int animInstanceID)
{
	mesh.mLTW.push_back(transform);
	mesh.mColors.push_back(color);
	if (animInstanceID < 0)
		animInstanceID = -2;
	mesh.mTexEntID.push_back(vec4((float)meshID + 0.5f, (float)entityID + 0.5f, (float)animInstanceID + 0.5f, 0));
	mesh.mBloomThresholds.push_back(bloomthreshold);
}

void GraphicsSystem::SetAllEntityBloomThreshold(glm::vec4 threshold, std::string meshName)
{
	//!< Helper
	auto getMeshname = [](std::string filepath) -> std::string
	{
		// returns AT-AT
		std::string ret_str = filepath.substr(filepath.find_last_of("/") + 1);
		ret_str = ret_str.substr(0, ret_str.find_first_of("."));
		return ret_str;
	};

	auto meshRendererInstances = systemManager->ecs->GetEntitiesWith<MeshRenderer>();
	for (Entity inst : meshRendererInstances)
	{
		auto& meshinst = inst.GetComponent<MeshRenderer>();
		if ( getMeshname(meshinst.mMeshPath) == meshName)
		{
			meshinst.mBloomThreshold = threshold;
		}
	}
}

/***************************************************************************/
/*!
\brief
	Updates the camera's position.
*/
/**************************************************************************/
void GraphicsSystem::SetCameraPosition(CAMERA_TYPE type, vec3 position)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return; // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		camera.GetComponent<Camera>().mCamera.SetPosition(position);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		m_EditorCamera.SetPosition(position);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		camera.GetComponent<Camera>().mCamera.SetPosition(position);
		m_EditorCamera.SetPosition(position);
		break;
	}
}

/***************************************************************************/
/*!
\brief
	Updates the camera's target.
*/
/**************************************************************************/
void GraphicsSystem::SetCameraTarget(CAMERA_TYPE type, vec3 position)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return; // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		camera.GetComponent<Camera>().mCamera.SetTarget(position);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		m_EditorCamera.SetTarget(position);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		camera.GetComponent<Camera>().mCamera.SetTarget(position);
		m_EditorCamera.SetTarget(position);
		break;
	}
}

/***************************************************************************/
/*!
\brief
	Updates the camera's projection.
*/
/**************************************************************************/
void GraphicsSystem::SetCameraProjection(CAMERA_TYPE type, float fovDegree, ivec2 size, float nearZ, float farZ)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return; // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{

	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		camera.GetComponent<Camera>().mCamera.SetProjection(fovDegree, size, nearZ, farZ);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		m_EditorCamera.SetProjection(fovDegree, size, nearZ, farZ);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		camera.GetComponent<Camera>().mCamera.SetProjection(fovDegree, size, nearZ, farZ);
		m_EditorCamera.SetProjection(fovDegree, size, nearZ, farZ);
		break;
	}
}

/***************************************************************************/
/*!
\brief
	Sets the size of the camera according to the enum provided
*/
/**************************************************************************/
void GraphicsSystem::SetCameraSize(CAMERA_TYPE type, ivec2 size)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return; // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		camera.GetComponent<Camera>().mCamera.SetSize(size);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		m_EditorCamera.SetSize(size);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		camera.GetComponent<Camera>().mCamera.SetSize(size);
		m_EditorCamera.SetSize(size);
		break;
	}
}


inline void drawViewFrustum(Entity gameCamera)
{
	auto& camera = gameCamera.GetComponent<Camera>().mCamera;

	mat4 inv = glm::inverse(camera.mView);

	float halfHeight = tanf(glm::radians(camera.mFovDegree / 2.f));
	float halfWidth = halfHeight * camera.mAspectRatio;

	float near1 = camera.mNear;
	float far1 = camera.mFar;
	float xn = halfWidth * near1;
	float xf = halfWidth * far1;
	float yn = halfHeight * near1;
	float yf = halfHeight * far1;

	glm::vec4 f[8u] =
	{
		// near face
		{xn, yn,	-near1, 1.f},
		{-xn, yn,	-near1, 1.f},
		{xn, -yn,	-near1, 1.f},
		{-xn, -yn,	-near1 , 1.f},

		// far face
		{xf, yf,	-far1, 1.f},
		{-xf, yf,	-far1 , 1.f},
		{xf, -yf,	-far1 , 1.f},
		{-xf, -yf,	-far1, 1.f},
	};

	glm::vec3 v[8];
	for (int i = 0; i < 8; i++)
	{
		vec4 ff = inv * f[i];
		v[i].x = ff.x / ff.w;
		v[i].y = ff.y / ff.w;
		v[i].z = ff.z / ff.w;
	}

	//glm::vec4 color = {1.f, 0.38f, 0.01f, 1.f};
	glm::vec4 color = {1.f, 1.f, 0.5f, 1.f};
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[0], v[1], color);
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[0], v[2], color);
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[3], v[1], color);
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[3], v[2], color);

	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[4], v[5], color);
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[4], v[6], color);
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[7], v[5], color);
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[7], v[6], color);

	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[0], v[4], color);
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[1], v[5], color);
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[3], v[7], color);
	systemManager->mGraphicsSystem->m_Renderer.AddLine(v[2], v[6], color);
}


/***************************************************************************/
/*!
\brief
	Updates the camera and its input according to the enum provided
*/
/**************************************************************************/
void GraphicsSystem::UpdateCamera(CAMERA_TYPE type, const float &dt)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	Entity GameCamera;

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		{
			if (localcamera.empty())
				return;
			GameCamera = localcamera.front();		// there will only be one game camera
			auto& GameCameraTransform = GameCamera.GetComponent<Transform>();
			auto& GameCameraComponent = GameCamera.GetComponent<Camera>();

			GameCameraComponent.mCamera.mTarget += (GameCameraTransform.mTranslate - GameCameraComponent.mCamera.mPosition);
			GameCameraComponent.mCamera.mPosition = GameCameraTransform.mTranslate;
			GameCameraComponent.mCamera.mPitch = GameCameraTransform.mRotate.y;
			GameCameraComponent.mCamera.mYaw = GameCameraTransform.mRotate.x;
			GameCameraComponent.mCamera.Update();

			// debug drawing
			if (systemManager->mGraphicsSystem->m_DebugDrawing) {
				drawViewFrustum(GameCamera);
			}
			break;
		}

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		{
			Camera_Input::getInstance().updateCameraInput(m_EditorCamera, dt);
			m_EditorCamera.Update();
			break;
		}

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		{
			// == update editor camera ==
			Camera_Input::getInstance().updateCameraInput(m_EditorCamera, dt);
			m_EditorCamera.Update();

			// == update game camera ==
			if (localcamera.empty())
				return;
			GameCamera = localcamera.front();		// there will only be one game camera

			auto& GameCameraTransform = GameCamera.GetComponent<Transform>();
			auto& GameCameraComponent = GameCamera.GetComponent<Camera>();

			GameCameraComponent.mCamera.mTarget += (GameCameraTransform.mTranslate - GameCameraComponent.mCamera.mPosition);
			GameCameraComponent.mCamera.mPosition = GameCameraTransform.mTranslate;
			GameCameraComponent.mCamera.mPitch = GameCameraTransform.mRotate.y;
			GameCameraComponent.mCamera.mYaw = GameCameraTransform.mRotate.x;

			GameCameraComponent.mCamera.Update();

			// debug drawing
			if (systemManager->mGraphicsSystem->m_DebugDrawing) {
				drawViewFrustum(GameCamera);
			}
			break;
		}
	}
}

/***************************************************************************/
/*!
\brief
	Gets the camera's position depending on the enum provided
*/
/**************************************************************************/
vec3 GraphicsSystem::GetCameraPosition(CAMERA_TYPE type)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return vec3(); // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		return camera.GetComponent<Camera>().mCamera.position();

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		return m_EditorCamera.position();

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		break;
	}
	PERROR("camera spoil - graphicssystem.cpp line 751");
	return {};
}

/***************************************************************************/
/*!
\brief
	Gets the camera's target depending on the enum provided
*/
/**************************************************************************/
vec3 GraphicsSystem::GetCameraTarget(CAMERA_TYPE type)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return vec3(); // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		return camera.GetComponent<Camera>().mCamera.target();

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		return m_EditorCamera.target();

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		return vec3();
		break;
	}
	PERROR("camera spoil - graphicssystem.cpp line 780");
	return {};
}

/***************************************************************************/
/*!
\brief
	Gets the direction the camera is pointint to depending on the enum
	provided
*/
/**************************************************************************/
vec3 GraphicsSystem::GetCameraDirection(CAMERA_TYPE type)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return vec3(); // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		return camera.GetComponent<Camera>().mCamera.direction();

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		return m_EditorCamera.direction();

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		return vec3();
		break;
	}
	PERROR("camera spoil - graphicssystem.cpp GetCameraDirection()");
	return {};
}

mat4 GraphicsSystem::GetCameraViewMatrix(CAMERA_TYPE type)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return mat4(1.0); // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		return camera.GetComponent<Camera>().mCamera.mView;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		return m_EditorCamera.mView;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		return mat4(1.0);
		break;
	}
	PERROR("camera spoil - graphicssystem.cpp GetCameraViewMatrix()");
	return {};
}

mat4 GraphicsSystem::GetCameraViewProjMatrix(CAMERA_TYPE type)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return mat4(1.0); // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		return camera.GetComponent<Camera>().mCamera.viewProj();

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		return m_EditorCamera.viewProj();

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		return mat4(1.0);
		break;
	}
	PERROR("camera spoil - graphicssystem.cpp GetCameraViewMatrix()");
	return {};
}

ivec2 GraphicsSystem::GetCameraSize(CAMERA_TYPE type)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return ivec2(); // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		return camera.GetComponent<Camera>().mCamera.mSize;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		return m_EditorCamera.mSize;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		return ivec2();
		break;
	}
	PERROR("camera spoil - graphicssystem.cpp GetCameraSize()");
	return {};
}

/***************************************************************************/
/*!
\brief
	Draws all instances of this mesh
*/
/**************************************************************************/
void GraphicsSystem::DrawAll(GFX::Mesh &mesh)
{
	mesh.DrawAllInstances();
}



/***************************************************************************/
/*!
\brief
	Helper function -- to print mat4 matrices
*/
/**************************************************************************/
void GraphicsSystem::PrintMat4(const glm::mat4 &input)
{
	std::cout << "=============================================================================\n";
	std::cout << "| " << input[0][0] << " | " << input[1][0] << " | " << input[2][0] << " | " << input[3][0] << "\n";
	std::cout << "| " << input[0][1] << " | " << input[1][1] << " | " << input[2][1] << " | " << input[3][1] << "\n";
	std::cout << "| " << input[0][2] << " | " << input[1][2] << " | " << input[2][2] << " | " << input[3][2] << "\n";
	std::cout << "| " << input[0][3] << " | " << input[1][3] << " | " << input[2][3] << " | " << input[3][3] << "\n";
	std::cout << "=============================================================================\n";
}

/***************************************************************************/
/*!
\brief
	Global enum to unpause all animations
*/
/**************************************************************************/
void GraphicsSystem::UnpauseGlobalAnimation()
{
	m_EnableGlobalAnimations = true;
}

void GraphicsSystem::CheckWindowSize()
{
	int width{}, height{};

	glfwGetFramebufferSize(m_Window->GetHandle(), &width, &height);

	if (width != m_Width || height != m_Height)
	{
		ResizeWindow({ width, height });
	}
}

void GraphicsSystem::ResizeWindow(ivec2 newSize)
{
	// Update viewport
	glViewport(0, 0, newSize.x, newSize.y);

	// Update FBOs
	m_Fbo.Resize(newSize.x, newSize.y);
	m_GameFbo.Resize(newSize.x, newSize.y);
	m_MultisampleFBO.Resize(newSize.x, newSize.y);
	m_IntermediateFBO.Resize(newSize.x, newSize.y);
	m_PingPongFbo.Resize(newSize.x, newSize.y);

	// Update Window
	m_Window->SetWindowSize(newSize);

	m_Width = newSize.x;
	m_Height = newSize.y;

	SetCameraSize(CAMERA_TYPE::CAMERA_TYPE_ALL, newSize);
}

void GraphicsSystem::ClampCursor()
{
	double posX, posY;

	m_Window->GetCursorPos(&posX, &posY);
	posX = glm::clamp(posX, -20.0, static_cast<double>(m_Window->GetScreenWidth()));
	posY = glm::clamp(posY, -20.0, static_cast<double>(m_Window->GetScreenHeight()));

	glfwSetCursorPos(m_Window->GetHandle(), posX, posY);
}

void GraphicsSystem::HideCursor(bool hideCursor)
{
	if (hideCursor)
		glfwSetInputMode(m_Window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	else
		glfwSetInputMode(m_Window->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GraphicsSystem::Unload()
{
	auto meshRendererInstances = systemManager->ecs->GetEntitiesWith<MeshRenderer>();

	for (Entity inst : meshRendererInstances)
	{
		GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(inst.GetComponent<MeshRenderer>().mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance));
		if (meshinst == nullptr)
			continue;

		// clears all instances of LTW, Colors for every mesh
		meshinst->ClearInstances();
	}

	// clears all debug draw instances
	m_Renderer.ClearInstances();
}

GLuint64 GraphicsSystem::GetAndStoreBindlessTextureHandle(int texID)
{
	if (texID < 0)
		return 0;

	// Store the 64-bit handle
	GLuint64 handle = glGetTextureHandleARB((unsigned)texID);
	m_MaterialHandles[(unsigned)texID] = handle;

	if (!glIsTextureHandleResidentARB(handle))
	{
		glMakeTextureHandleResidentARB(handle);
	}

	return handle;
}

void GraphicsSystem::SetupShaderStorageBuffers()
{
	// All materials of each instance -- Location 1
	m_MaterialSsbo.Create(sizeof(MaterialSSBO) * MAX_INSTANCES, 1);

	// All Point Lights in the Scene -- Location 2
	m_PointLightSsbo.Create(sizeof(PointLightSSBO) * MAX_POINT_LIGHT, 2);

	// Final Bone Matrix for Animation -- Location 3
	m_FinalBoneMatrixSsbo.Create(sizeof(mat4) * MAX_NUM_BONES * MAX_INSTANCES, 3);

}

void GraphicsSystem::DrawAll2DInstances(unsigned shaderID)
{
	(void)shaderID;

	if (m_Image2DStore.size() == 0)
		return;

	// Bind Textures to OpenGL context
	for (size_t i{}; i < m_Image2DStore.size(); ++i)
	{
		glBindTextureUnit(static_cast<GLuint>(i), m_Image2DStore[static_cast<GLuint>(i)]);
	}

	// Bind 2D quad VAO
	m_Image2DMesh.BindVao();

	// Draw call
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, static_cast<GLsizei>(m_Image2DMesh.mLTW.size()));

	// Unbind 2D quad VAO
	m_Image2DMesh.UnbindVao();

	// Unbind Textures from openGL context
	for (size_t i{}; i < m_Image2DStore.size(); ++i)
	{
		glBindTextureUnit(static_cast<GLuint>(i), 0);
	}
}

void GraphicsSystem::Add2DImageInstance(float width, float height, vec2 const& position, unsigned texHandle, unsigned entityID, float degree, vec4 const& color)
{
	float half_w = m_Width * 0.5f;
	float half_h = m_Height * 0.5f;

	mat4 world =
	{
		vec4(width / half_w, 0.f, 0.f, 0.f),
		vec4(0.f, height / half_h, 0.f, 0.f),
		vec4(0.f, 0.f, 1.f, 0.f),
		vec4(position.x / m_Width, position.y / m_Height, 0.f, 1.f)
	};

	int texIndex{};
	if (texHandle > 0)
		texIndex = StoreTextureIndex(texHandle);
	else
		texIndex = -2;

	m_Image2DMesh.mLTW.push_back(world);
	m_Image2DMesh.mColors.push_back(color);
	m_Image2DMesh.mTexEntID.push_back(vec4((float)texIndex + 0.5f, (float)entityID + 0.5f, degree, 0));
}

int GraphicsSystem::StoreTextureIndex(unsigned texHandle)
{
	if (m_Image2DStore.size() >= 32)
	{
		std::cout << "Too many different textures to be rendered in 1 frame, texture discarded\n";
		return -1;
	}

	// Locate if ID already in container
	auto it = std::find(m_Image2DStore.cbegin(), m_Image2DStore.cend(), (GLint)texHandle);
	if (it == m_Image2DStore.cend())	// ID not in container
	{
		m_Image2DStore.push_back((GLint)texHandle);
		return (int)m_Image2DStore.size() - 1;
	}

	// ID is already in container
	int pos = (int)(it - m_Image2DStore.cbegin());
	return pos;
}

void GraphicsSystem::DrawAllHealthbarInstance(const mat4& viewProj)
{
	if (m_HealthbarMesh.mLTW.size() == 0)
		return;

	// Bind shader and VAO
	m_HealthbarShaderInst.Activate();
	m_HealthbarMesh.BindVao();

	// Set view projection matrix uniform
	glUniformMatrix4fv(m_HealthbarViewProjLocation, 1, GL_FALSE, &viewProj[0][0]);

	// Draw call
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, static_cast<GLsizei>(m_HealthbarMesh.mLTW.size()));

	// Unbinding shader and VAO
	GFX::Shader::Deactivate();
	m_HealthbarMesh.UnbindVao();
}

void GraphicsSystem::AddHealthbarInstance(const Healthbar& healthbar, const vec3& camPos, unsigned entityID)
{
	// Compute the rotation vectors
	vec3 normal = camPos - healthbar.mPosition;
	vec3 up = { 0, 1, 0 };
	vec3 right = glm::cross(up, normal);
	vec3 forward = glm::cross(right, normal);

	mat4 scale = {
		vec4(healthbar.mWidth, 0.f, 0.f, 0.f),
		vec4(0.f, healthbar.mHeight, 0.f, 0.f),
		vec4(0.f, 0.f, 1.f, 0.f),
		vec4(0.f, 0.f, 0.f, 1.f)
	};

	// Rotation matrix to always face the camera
	mat4 rotate = {
		vec4(glm::normalize(right), 0.0f),
		vec4(glm::normalize(forward), 0.0f),
		vec4(glm::normalize(normal), 0.0f),
		vec4(healthbar.mPosition, 1.0f)
	};

	mat4 world = rotate * scale;

	// Update colors
	vec4 healthColor = vec4(healthbar.mHealthColor.x, healthbar.mHealthColor.y, healthbar.mHealthColor.z, healthbar.mHealth);
	m_HealthbarMesh.mColors.push_back(healthColor);
	m_HealthbarMesh.mTexEntID.push_back(healthbar.mBackColor);
	// Update LTW matrix
	m_HealthbarMesh.mLTW.push_back(world);
}

void GraphicsSystem::SetupCrosshairShaderLocations()
{
	m_CrosshairThicknessLocation = m_CrosshairShaderInst.GetUniformLocation("uThickness");
	m_CrosshairInnerLocation = m_CrosshairShaderInst.GetUniformLocation("uInner");
	m_CrosshairOuterLocation = m_CrosshairShaderInst.GetUniformLocation("uOuter");
	m_CrosshairColorLocation = m_CrosshairShaderInst.GetUniformLocation("uColor");
}

void GraphicsSystem::DrawCrosshair()
{
	// Get entity with crosshair component
	auto ent = systemManager->ecs->GetEntitiesWith<Crosshair>();

	if(ent.size() == 0)
		return;

	// Use crosshair shader program adn VAO
	m_CrosshairShaderInst.Activate();
	m_Image2DMesh.BindVao();

	for (Entity inst : ent)	// only if entity exists
	{
		auto crosshairInst = inst.GetComponent<Crosshair>();

		glUniform1f(m_CrosshairThicknessLocation, crosshairInst.mThickness);
		glUniform1f(m_CrosshairInnerLocation, crosshairInst.mInner);
		glUniform1f(m_CrosshairOuterLocation, crosshairInst.mOuter);
		glUniform4fv(m_CrosshairColorLocation, 1, &crosshairInst.mColor[0]);
	}

	// Draw call
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 1);

	// Stop using crosshair shader program and VAO
	m_CrosshairShaderInst.Deactivate();
	m_Image2DMesh.UnbindVao();
}

void GraphicsSystem::DrawDeferredLight(const vec3& camPos, GFX::FBO& destFbo)
{
	destFbo.Bind();
	// Bind VAO and activate shader
	m_Image2DMesh.BindVao();
	m_DeferredLightShaderInst.Activate();
	
	// Set Shader uniforms
	glUniform3fv(m_DeferredCamPosLocation, 1, &camPos[0]);
	glUniform1i(m_DeferredLightCountLocation, m_LightCount);

	// Draw
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 1);
	
	// Unbind VAO and Deactivate shader
	m_DeferredLightShaderInst.Deactivate();
	m_Image2DMesh.UnbindVao();
	destFbo.Unbind();
}

void GraphicsSystem::BlitMultiSampleToDestinationFBO(GFX::FBO& destFbo, bool editorFlag)
{
	// Clear out intermediate before blitting
	m_IntermediateFBO.Clear();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Copy pixel data from multisample FBO to intermediate FBO
	m_MultisampleFBO.BlitFramebuffer(m_IntermediateFBO.GetID());

	// Clear out destination FBO before blitting
	destFbo.Clear();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Copies only the entity ID data to editor FBO
	if (editorFlag)
		m_IntermediateFBO.BlitFramebuffer(destFbo.GetID());
}

void GraphicsSystem::SetupAllShaders()
{
	// Initialize the UI Shader
	uid uiShaderstr("UIShader");
	m_UiShaderInst = *systemManager->mResourceTySystem->get_Shader(uiShaderstr.id);
	// Uniforms of UI Shader
	m_UiShaderInst.Activate();
	GLuint uniform_tex = glGetUniformLocation(m_UiShaderInst.GetHandle(), "uTex2d");
	glUniform1iv(uniform_tex, (GLsizei)m_Textures.size(), m_Textures.data()); // Passing texture Binding units to frag shader [0 - 31]
	m_UiShaderInst.Deactivate();

	// Initialize the Crosshair shader
	uid crosshairShaderstr("CrosshairShader");
	m_CrosshairShaderInst = *systemManager->mResourceTySystem->get_Shader(crosshairShaderstr.id);
	m_CrosshairShaderInst.Activate();
	SetupCrosshairShaderLocations();
	m_CrosshairShaderInst.Deactivate();

	// Initialize the Healthbar shader and uniform location
	uid healthbarShaderstr("healthbarShader");
	m_HealthbarShaderInst = *systemManager->mResourceTySystem->get_Shader(healthbarShaderstr.id);
	m_HealthbarShaderInst.Activate();
	m_HealthbarViewProjLocation = m_HealthbarShaderInst.GetUniformLocation("uViewProj");
	m_HealthbarShaderInst.Deactivate();

	// Initialize the Healthbar shader and uniform location
	uid animationShaderstr("AnimationShader");
	m_AnimationShaderInst = *systemManager->mResourceTySystem->get_Shader(animationShaderstr.id);
	
	// Initialize the deferred light shader and uniform location
	uid deferredLightShaderstr("deferredLightShader");
	m_DeferredLightShaderInst = *systemManager->mResourceTySystem->get_Shader(deferredLightShaderstr.id);
	m_DeferredLightShaderInst.Activate();
	m_DeferredCamPosLocation = m_DeferredLightShaderInst.GetUniformLocation("uCamPos");
	m_DeferredLightCountLocation = m_DeferredLightShaderInst.GetUniformLocation("uLightCount");

	// Bind Textures
	GLuint64 handle = glGetTextureHandleARB(m_IntermediateFBO.GetFragPosAttachment());
	glMakeTextureHandleResidentARB(handle);
	glUniform1ui64ARB(m_DeferredLightShaderInst.GetUniformLocation("gPosition"), handle);

	handle = glGetTextureHandleARB(m_IntermediateFBO.GetNormalAttachment());
	glMakeTextureHandleResidentARB(handle);
	glUniform1ui64ARB(m_DeferredLightShaderInst.GetUniformLocation("gNormal"), handle);

	handle = glGetTextureHandleARB(m_IntermediateFBO.GetAlbedoSpecAttachment());
	glMakeTextureHandleResidentARB(handle);
	glUniform1ui64ARB(m_DeferredLightShaderInst.GetUniformLocation("gAlbedoSpec"), handle);

	handle = glGetTextureHandleARB(m_IntermediateFBO.GetEmissionAttachment());
	glMakeTextureHandleResidentARB(handle);
	glUniform1ui64ARB(m_DeferredLightShaderInst.GetUniformLocation("gEmission"), handle);
	m_DeferredLightShaderInst.Deactivate();

	// Initialize the G-buffer shader and uniform location
	uid gBufferShaderstr("gBufferShader");
	m_GBufferShaderInst = *systemManager->mResourceTySystem->get_Shader(gBufferShaderstr.id);
}

void GraphicsSystem::ComputeDeferredLight(bool editorDraw)
{
	if (editorDraw)	// Draw to Editor FBO
	{
		glBindImageTexture(0, m_Fbo.GetColorAttachment(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(1, m_Fbo.GetBrightColorsAttachment(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}
	else
	{
		glBindImageTexture(0, m_GameFbo.GetColorAttachment(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(1, m_GameFbo.GetBrightColorsAttachment(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}

	computeDeferred.Activate();

	glUniform1i(m_ComputeDeferredLightCountLocation, m_LightCount);
	glUniform3fv(m_ComputeDeferredCamPosLocation, 1, &m_EditorCamera.mPosition[0]);
	glUniform4fv(m_ComputeDeferredGlobalTintLocation, 1, &m_GlobalTint[0]);
	vec4 globalBloom = vec4(mAmbientBloomThreshold, m_EnableBloom);
	glUniform4fv(m_ComputeDeferredGlobalBloomLocation, 1, &globalBloom[0]);

	int num_group_x = glm::ceil(m_Width / 29);
	int num_group_y = glm::ceil(m_Height / 29);
	glDispatchCompute(num_group_x, num_group_y, 1);
	// make sure writing to image is done before reading
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	computeDeferred.Deactivate();
}

/***************************************************************************/
/*!
\brief
	Global enum to pause all animations
*/
/**************************************************************************/
void GraphicsSystem::PauseGlobalAnimation()
{
	m_EnableGlobalAnimations = false;
}


void MeshRenderer::SetColor(const vec4& color)
{
	mInstanceColor = color;
}


void MeshRenderer::SetMesh(const std::string& meshName, Entity inst)
{
	// gets the guid from the fbx descriptor file
	std::string descFilepath = systemManager->mResourceTySystem->fbx_path + meshName + ".fbx.desc";
	unsigned guid = _GEOM::GetGUID(descFilepath);

	mMeshRef.data = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(guid));
	mMeshPath = systemManager->mResourceTySystem->compiled_geom_path + meshName + ".geom";

	GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mMeshRef.data);
	if (inst.HasComponent<Animator>())
	{
		if (meshinst->mHasAnimation) {
			// change the animation to the new mesh's
			inst.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst->mAnimation[0]);
		}

		else {
			// the new mesh has no animation, but the current entity has an animator component
			inst.GetComponent<Animator>().mAnimator.SetAnimation(nullptr);
		}
	}
}


void MeshRenderer::SetMeshDelayed(const std::string& meshName, Entity inst)
{
	// gets the guid from the fbx descriptor file
	std::string descFilepath = systemManager->mResourceTySystem->fbx_path + meshName + ".fbx.desc";
	unsigned guid = _GEOM::GetGUID(descFilepath);

	GFX::Mesh* meshinst = systemManager->mResourceTySystem->get_mesh(guid);

	if (inst.HasComponent<Animator>() && meshinst->mHasAnimation)
	{
		// animation is available, then set the animation after current animation is done
		auto& animatorinst = inst.GetComponent<Animator>();
		animatorinst.mAnimator.mToChangeMeshDelayed.first = true;
		animatorinst.mAnimator.mToChangeMeshDelayed.second = meshName;
	}

	else
	{
		// there is no animation, then set the mesh immediately
		mMeshRef.data	= reinterpret_cast<void*>(meshinst);
		mMeshPath		= systemManager->mResourceTySystem->compiled_geom_path + meshName + ".geom";

		if (inst.HasComponent<Animator>())
			inst.GetComponent<Animator>().mAnimator.SetAnimation(nullptr);	// reset the animation as needed
	}
}


void MeshRenderer::SetTexture(MaterialType MaterialType, const std::string& Texturename)
{
	std::string MaterialInstancePath = systemManager->mResourceTySystem->compressed_texture_path + Texturename + ".ctexture";
	mMaterialInstancePath[MaterialType] = MaterialInstancePath;

	std::string descFilepath = MaterialInstancePath + ".desc";
	unsigned guid = _GEOM::GetGUID(descFilepath);

	mTextureRef[MaterialType].data = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(guid));
}


void PointLight::SetColor(const vec3& color)
{
	mLightColor = color;
}
