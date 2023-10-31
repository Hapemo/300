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
#define  _ENABLE_ANIMATIONS 1
#define  _TEST_PIE_SHADER 0

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
		m_Image2DMesh.Setup2DImageMesh();
		for (int i{}; i < 32; ++i)
		{
			m_Textures.emplace_back(i);
		}

		// Initialize the UI Shader
		std::string uiShader = "UIShader";
		uid uiShaderstr(uiShader);
		m_UiShaderInst = *systemManager->mResourceTySystem->get_Shader(uiShaderstr.id);

		// Uniforms of UI Shader
		m_UiShaderInst.Activate();
		GLuint uniform_tex = glGetUniformLocation(m_UiShaderInst.GetHandle(), "uTex2d");
		glUniform1iv(uniform_tex, (GLsizei)m_Textures.size(), m_Textures.data()); // Passing texture Binding units to frag shader [0 - 31]
		m_UiShaderInst.Deactivate();

		// Get Window Handle
		m_Window = systemManager->GetWindow();
		m_Width = m_Window->size().x;
		m_Height = m_Window->size().y;

		// Update the editor mode flag
		m_EditorMode = systemManager->IsEditor();

		// Create FBO, with the width and height of the window
		m_Fbo.Create(m_Width, m_Height, m_EditorMode);
		m_GameFbo.Create(m_Width, m_Height, m_EditorMode);
		m_PingPongFbo.Create(m_Width, m_Height);
	}
	

	// Set Cameras' starting position
	SetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR, {0, 0, 20});									// Position of camera
	SetCameraTarget(CAMERA_TYPE::CAMERA_TYPE_EDITOR, {0, 0, 0});									// Target of camera
	SetCameraProjection(CAMERA_TYPE::CAMERA_TYPE_ALL, 60.f, m_Window->size(), 0.1f, 900.f);			// Projection of camera

	// init game camera
	/*SetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_GAME, { 16.218f, 474.854f, 748.714f });
	SetCameraTarget(CAMERA_TYPE::CAMERA_TYPE_GAME, { 16.21f, 473.694f, 739.714f });*/

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

	for (Entity inst : meshRendererInstances)
	{
		MeshRenderer& meshRenderer = inst.GetComponent<MeshRenderer>();

		// if the mesh instance is not active, skip it
		if (meshRenderer.mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance) == nullptr)
			continue;
		
		
		// gives me the mesh
		void *tt = meshRenderer.mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance);
		GFX::Mesh &meshinst = *reinterpret_cast<GFX::Mesh *>(tt);


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

			// draw the mesh's origin
			m_Renderer.AddSphere(m_EditorCamera.position(), transforminst.mTranslate, 0.5f, {1.f, 1.f, 0.f, 1.f});
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
		bool hasanimation = inst.HasComponent<Animator>();
		if (hasanimation && _ENABLE_ANIMATIONS && systemManager->mGraphicsSystem->m_EnableGlobalAnimations)
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
		if (hasanimation) {
			AddInstance(meshinst, final, meshRenderer.mInstanceColor, static_cast<int>(m_Materials.size()), static_cast<unsigned>(inst.id), animationID++);
		}
		else {
			AddInstance(meshinst, final, meshRenderer.mInstanceColor, static_cast<int>(m_Materials.size()), static_cast<unsigned>(inst.id));
		}

		MaterialSSBO material{};
		// Save the materials if it exists
		//material.mDiffuseMap	= GetAndStoreBindlessTextureHandle(meshRenderer.GetTexture(DIFFUSE));		
		//material.mNormalMap		= GetAndStoreBindlessTextureHandle(meshRenderer.GetTexture(NORMAL));		
		//material.mSpecularMap	= GetAndStoreBindlessTextureHandle(meshRenderer.GetTexture(SPECULAR));	
		//material.mShininessMap	= GetAndStoreBindlessTextureHandle(meshRenderer.GetTexture(SHININESS));	
		//material.mEmissionMap	= GetAndStoreBindlessTextureHandle(meshRenderer.GetTexture(EMISSION));	

		auto getID = [&](MaterialType type, MeshRenderer& meshrenderer) ->int {

			if (meshrenderer.mTextureRef[static_cast<int>(type)].getdata(systemManager->mResourceTySystem->m_ResourceInstance) == nullptr)
				return -1;

			return static_cast<GFX::Texture*>(meshrenderer.mTextureRef[static_cast<int>(type)].data)->ID();
		};


		material.mDiffuseMap = GetAndStoreBindlessTextureHandle(getID(DIFFUSE, meshRenderer));
		material.mNormalMap = GetAndStoreBindlessTextureHandle(getID(NORMAL, meshRenderer));
		material.mSpecularMap = GetAndStoreBindlessTextureHandle(getID(SPECULAR, meshRenderer));
		material.mShininessMap = GetAndStoreBindlessTextureHandle(getID(SHININESS, meshRenderer));
		material.mEmissionMap = GetAndStoreBindlessTextureHandle(getID(EMISSION, meshRenderer));

		m_Materials.emplace_back(material);	// push back
	}
	m_FinalBoneMatrixSsbo.SubData(finalBoneMatrices.size() * sizeof(mat4), finalBoneMatrices.data());
	finalBoneMatrices.clear();
	m_MaterialSsbo.SubData(m_Materials.size() * sizeof(MaterialSSBO), m_Materials.data());
	m_Materials.clear();

	// Sending Light source data to GPU
	auto lightEntity = systemManager->ecs->GetEntitiesWith<PointLight>();
	m_HasLight = !lightEntity.empty();
	m_LightCount = lightEntity.size();

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

			m_Renderer.AddCube(lightTransform.mTranslate, { 20, 20, 20 }, vec4(lightData.mLightColor, 1.f));
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

		Add2DImageInstance(uiWidth, uiHeight, uiPosition, texID, static_cast<int>(inst.id));
	}
	// Send UI data to GPU
	m_Image2DMesh.PrepForDraw();
#pragma endregion
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
	m_Fbo.PrepForDraw();

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
		
		std::string shader{};

		if (meshinst.mHasAnimation)
			shader = "AnimationShader";
		else
			shader = "PointLightShader";

		uid shaderstr(shader);
		GFX::Shader& shaderinst = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);
		unsigned shaderID = shaderinst.GetHandle();

		//// bind all texture
		//GFX::Texture* textureInst[4]{};
		//for (int i{ 0 }; i < 4; i++)
		//{
		//	if (inst.GetComponent<MeshRenderer>().mTextureRef[i].getdata(systemManager->mResourceTySystem->m_ResourceInstance) != nullptr)
		//	{
		//		textureInst[i] = reinterpret_cast<GFX::Texture *>(inst.GetComponent<MeshRenderer>().mTextureRef[i].data);
		//	}
		//}

		shaderinst.Activate();
		glUniformMatrix4fv(shaderinst.GetUniformVP(), 1, GL_FALSE, &m_EditorCamera.viewProj()[0][0]);

		// Retrieve Point Light object
		auto lightEntity = systemManager->ecs->GetEntitiesWith<PointLight>();
		m_HasLight = !lightEntity.empty();

		GLuint mHasLightFlagLocation = shaderinst.GetUniformLocation("uHasLight");
		glUniform1i(mHasLightFlagLocation, m_HasLight);

		if (systemManager->mGraphicsSystem->m_EnableBloom)
		{
			GLuint threshold = shaderinst.GetUniformLocation("bloomThreshold");

			if (inst.HasComponent<VFX>()) {
				if (inst.GetComponent<VFX>().isObjectBloom) {
					glUniform3fv(threshold, 1, glm::value_ptr(inst.GetComponent<VFX>().mBloomThreshold));	
				}
			}
			else {
				glUniform3fv(threshold, 1, glm::value_ptr(mAmbientBloomThreshold));
			}
		}

		if (m_HasLight)
		{
			GLuint mViewPosShaderLocation = shaderinst.GetUniformLocation("uViewPos");
			vec3 viewPos = GetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR);
			glUniform3fv(mViewPosShaderLocation, 1, &viewPos[0]);

			// Light count uniform
			GLuint mLightCountShaderLocation = shaderinst.GetUniformLocation("uLightCount");
			glUniform1i(mLightCountShaderLocation, m_LightCount);
		}

		//// bind texture unit
		//for (int i{0}; i < 4; i++)
		//{
		//	if (inst.GetComponent<MeshRenderer>().mTextureRef[i].getdata(systemManager->mResourceTySystem->m_ResourceInstance) != nullptr)
		//	{
		//		glBindTextureUnit(i, textureInst[i]->ID());
		//	}
		//}


		GLuint debug_draw = glGetUniformLocation(shaderID, "uDebugDraw");
		glUniform1i(debug_draw, m_DebugDrawing);

		// Bind mesh's VAO, copy render data into VBO, Draw
		DrawAll(meshinst);

		shaderinst.Deactivate();
		meshinst.UnbindVao();

		//// unbind the textures
		//for (int i{0}; i < 4; i++)
		//{
		//	if (inst.GetComponent<MeshRenderer>().mTextureRef[i].data != nullptr)
		//	{
		//		glBindTextureUnit(i, 0);
		//	}
		//}
	}

	m_Renderer.RenderAll(m_EditorCamera.viewProj());

	if (systemManager->mGraphicsSystem->m_EnableBloom)
	{
		m_PingPongFbo.PrepForDraw();
	
		// Render the bloom for the Editor Framebuffer
		uid gaussianshaderstr("GaussianBlurShader");
		GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);

		m_PingPongFbo.GaussianBlur(gaussianShaderInst, m_Fbo, systemManager->mGraphicsSystem->mTexelOffset, systemManager->mGraphicsSystem->mSamplingWeight);

		AdditiveBlendFramebuffers(m_Fbo, m_Fbo.GetColorAttachment(), m_PingPongFbo.pingpongColorbuffers[0]);
	}


	if (systemManager->mGraphicsSystem->m_EnableChromaticAbberation)
	{
		ChromaticAbbrebationBlendFramebuffers(m_Fbo, m_Fbo.GetColorAttachment(), m_PingPongFbo.pingpongColorbuffers[0]);
	}

	// Render UI objects
	m_UiShaderInst.Activate();		// Activate shader
	DrawAll2DInstances(m_UiShaderInst.GetHandle());
	m_UiShaderInst.Deactivate();	// Deactivate shader

#if _TEST_PIE_SHADER
	std::string CircularShaderStr{"PieShaderTest"};
	uid circularShaderUID(CircularShaderStr);
	GFX::Shader& circularShaderInst = *systemManager->mResourceTySystem->get_Shader(circularShaderUID.id);

	circularShaderInst.Activate();		// activate shader
	GLuint degrees_uniform = glGetUniformLocation(circularShaderInst.GetHandle(), "uDegrees");
	glUniform1f(degrees_uniform, glm::radians(m_DegreeTest));

	// Bind 2D quad VAO
	m_Image2DMesh.BindVao();
	// Draw call
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 1);
	// Unbind 2D quad VAO
	m_Image2DMesh.UnbindVao();

	circularShaderInst.Deactivate();	// Deactivate shader
#endif

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

	// Prepare and bind the Framebuffer to be rendered on
	m_GameFbo.PrepForDraw();

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

		//GFX::Texture *textureInst[4]{};
		//for (int i{0}; i < 4; i++)
		//{
		//	if (inst.GetComponent<MeshRenderer>().mTextureRef[i].getdata(systemManager->mResourceTySystem->m_ResourceInstance) != nullptr)
		//	{
		//		textureInst[i] = reinterpret_cast<GFX::Texture *>(inst.GetComponent<MeshRenderer>().mTextureRef[i].data);
		//	}
		//}

		shaderinst.Activate();

		if (systemManager->mGraphicsSystem->m_EnableBloom)
		{
			GLuint threshold = shaderinst.GetUniformLocation("bloomThreshold");

			if (inst.HasComponent<VFX>()) {
				if (inst.GetComponent<VFX>().isObjectBloom) {
					glUniform3fv(threshold, 1, glm::value_ptr(inst.GetComponent<VFX>().mBloomThreshold));
				}
			}
			else {
				glUniform3fv(threshold, 1, glm::value_ptr(mAmbientBloomThreshold));
			}
		}

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
		}

		//for (int i{0}; i < 4; i++)
		//{
		//	if (inst.GetComponent<MeshRenderer>().mTextureRef[i].getdata(systemManager->mResourceTySystem->m_ResourceInstance) != nullptr)
		//	{
		//		glBindTextureUnit(i, textureInst[i]->ID());
		//	}
		//}

		// Bind mesh's VAO, copy render data into VBO, Draw
		DrawAll(meshinst);

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

	if (systemManager->mGraphicsSystem->m_EnableBloom)
	{
		m_PingPongFbo.PrepForDraw();

		// Render the bloom for the Game Framebuffer
		uid gaussianshaderstr("GaussianBlurShader");
		GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
		m_PingPongFbo.GaussianBlur(gaussianShaderInst, m_GameFbo, systemManager->mGraphicsSystem->mTexelOffset, systemManager->mGraphicsSystem->mSamplingWeight);

		AdditiveBlendFramebuffers(m_GameFbo, m_GameFbo.GetColorAttachment(), m_PingPongFbo.pingpongColorbuffers[0]);
	}

	if (systemManager->mGraphicsSystem->m_EnableChromaticAbberation)
	{
		ChromaticAbbrebationBlendFramebuffers(m_GameFbo, m_GameFbo.GetColorAttachment(), m_PingPongFbo.pingpongColorbuffers[0]);
	}

	// Render UI objects
	m_UiShaderInst.Activate();		// Activate shader
	DrawAll2DInstances(m_UiShaderInst.GetHandle());
	m_UiShaderInst.Deactivate();	// Deactivate Shader

	m_GameFbo.Unbind();
}


void GraphicsSystem::ChromaticAbbrebationBlendFramebuffers(GFX::FBO& targetFramebuffer, unsigned int Attachment0, unsigned int Attachment1)
{
	glBlendFunc(GL_ONE, GL_ONE);

	uid shaderstr("ChromaticAbberation");
	GFX::Shader& BlendShader = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);

	BlendShader.Activate();
	targetFramebuffer.Bind();

	// Draw to color attachment only. Otherwise might affect other attachments
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glUniform1f(BlendShader.GetUniformLocation("ChromaticAbberationStrength"), systemManager->mGraphicsSystem->mChromaticStrength);
	glBindTexture(GL_TEXTURE_2D, Attachment0);									// bind the first attachment
	glBindTexture(GL_TEXTURE_2D, Attachment1);									// bind the second attachment

	{
		mScreenQuad.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		mScreenQuad.Unbind();
	}

	targetFramebuffer.Unbind();
	BlendShader.Deactivate();
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
void GraphicsSystem::AddInstance(GFX::Mesh &mesh, Transform transform, const vec4& color, int meshID, unsigned entityID)
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
}

void GraphicsSystem::AddInstance(GFX::Mesh &mesh, mat4 transform, const vec4& color, int meshID, unsigned entityID, int animInstanceID)
{
	mesh.mLTW.push_back(transform);
	mesh.mColors.push_back(color);
	if (animInstanceID < 0)
		animInstanceID = -2;
	mesh.mTexEntID.push_back(vec4((float)meshID + 0.5f, (float)entityID + 0.5f, (float)animInstanceID + 0.5f, 0));
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

	float near = camera.mNear;
	float far = camera.mFar;
	float xn = halfWidth * near;
	float xf = halfWidth * far;
	float yn = halfHeight * near;
	float yf = halfHeight * far;

	glm::vec4 f[8u] =
	{
		// near face
		{xn, yn,	-near, 1.f},
		{-xn, yn,	-near, 1.f},
		{xn, -yn,	-near, 1.f},
		{-xn, -yn,	-near , 1.f},

		// far face
		{xf, yf,	-far, 1.f},
		{-xf, yf,	-far , 1.f},
		{xf, -yf,	-far , 1.f},
		{-xf, -yf,	-far, 1.f},
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

			//Camera_Input::getInstance().updateCameraInput(camera.GetComponent<Camera>().mCamera, dt);
			
			//camera.GetComponent<Transform>().mTranslate = camera.GetComponent<Camera>().mCamera.mPosition;
			GameCamera.GetComponent<Camera>().mCamera.mTarget += (GameCamera.GetComponent<Transform>().mTranslate - GameCamera.GetComponent<Camera>().mCamera.mPosition);
			GameCamera.GetComponent<Camera>().mCamera.mPosition = GameCamera.GetComponent<Transform>().mTranslate;
			GameCamera.GetComponent<Camera>().mCamera.mPitch = GameCamera.GetComponent<Transform>().mRotate.y;
			GameCamera.GetComponent<Camera>().mCamera.mYaw = GameCamera.GetComponent<Transform>().mRotate.x;
			GameCamera.GetComponent<Camera>().mCamera.Update();

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

			//camera.GetComponent<Transform>().mTranslate = camera.GetComponent<Camera>().mCamera.mPosition;
			GameCamera.GetComponent<Camera>().mCamera.mTarget += (GameCamera.GetComponent<Transform>().mTranslate - GameCamera.GetComponent<Camera>().mCamera.mPosition);
			GameCamera.GetComponent<Camera>().mCamera.mPosition = GameCamera.GetComponent<Transform>().mTranslate;
			GameCamera.GetComponent<Camera>().mCamera.mPitch = GameCamera.GetComponent<Transform>().mRotate.y;
			GameCamera.GetComponent<Camera>().mCamera.mYaw = GameCamera.GetComponent<Transform>().mRotate.x;
			GameCamera.GetComponent<Camera>().mCamera.Update();

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
	Perform additive blending on 2 color attachments
*/
/**************************************************************************/
void GraphicsSystem::AdditiveBlendFramebuffers(	GFX::FBO& targetFramebuffer, unsigned int Attachment0, unsigned int Attachment1)
{
	glBlendFunc(GL_ONE, GL_ONE);

	uid shaderstr("AdditiveBlendShader");
	GFX::Shader& BlendShader = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);

	BlendShader.Activate();
	targetFramebuffer.Bind();

	// Draw to color attachment only. Otherwise might affect other attachments
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glUniform1f(BlendShader.GetUniformLocation("Exposure"), systemManager->mGraphicsSystem->mAmbientBloomExposure);
	glBindTexture(GL_TEXTURE_2D, Attachment0);									// bind the first attachment
	glBindTexture(GL_TEXTURE_2D, Attachment1);									// bind the second attachment

	{
		mScreenQuad.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		mScreenQuad.Unbind();
	}

	targetFramebuffer.Unbind();
	BlendShader.Deactivate();
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
	m_PingPongFbo.Resize(newSize.x, newSize.y);

	// Update Window
	m_Window->SetWindowSize(newSize);

	m_Width = newSize.x;
	m_Height = newSize.y;
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
		GFX::Mesh& meshinst = *reinterpret_cast<GFX::Mesh*>(inst.GetComponent<MeshRenderer>().mMeshRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance));
		meshinst.ClearInstances();
	}
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
	// Bind Textures to OpenGL context
	for (size_t i{}; i < m_Image2DStore.size(); ++i)
	{
		glBindTextureUnit(i, m_Image2DStore[i]);
		//m_Textures.push_back(i);
	}
	//GLuint uniform_tex = glGetUniformLocation(shaderID, "uTex2d");
	//glUniform1iv(uniform_tex, (GLsizei)m_Textures.size(), m_Textures.data()); // passing Texture ID to the fragment shader
	//m_Textures.clear();

	// Bind 2D quad VAO
	m_Image2DMesh.BindVao();

	// Draw call
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, m_Image2DMesh.mLTW.size());

	// Unbind 2D quad VAO
	m_Image2DMesh.UnbindVao();

	// Unbind Textures from openGL context
	for (size_t i{}; i < m_Image2DStore.size(); ++i)
	{
		glBindTextureUnit(i, 0);
	}
}

void GraphicsSystem::Add2DImageInstance(float width, float height, vec2 const& position, unsigned texHandle, unsigned entityID, vec4 const& color)
{
	float half_w = m_Width * 0.5f;
	float half_h = m_Height * 0.5f;

	mat4 world =
	{
		vec4(width / m_Width, 0.f, 0.f, 0.f),
		vec4(0.f, height / m_Height, 0.f, 0.f),
		vec4(0.f, 0.f, 1.f, 0.f),
		vec4(position.x / half_w, position.y / half_h, 0.f, 1.f)
	};

	int texIndex{};
	if (texHandle > 0)
		texIndex = StoreTextureIndex(texHandle);
	else
		texIndex = -2;

	m_Image2DMesh.mLTW.push_back(world);
	m_Image2DMesh.mColors.push_back(color);
	m_Image2DMesh.mTexEntID.push_back(vec4((float)texIndex + 0.5f, (float)entityID + 0.5f, 0, 0));
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
	if (inst.HasComponent<Animator>() && meshinst->mHasAnimation)
	{
		// change the animation to the new mesh's
		inst.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst->mAnimation[0]);
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
