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

#include <ECS/ECS_Components.h>
#include <Graphics/GraphicsSystem.h>
#include "ResourceManagerTy.h"
#include <Graphics/Camera_Input.h>
#include "Debug/EnginePerformance.h"
#include "GameState/GameStateManager.h"

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

	m_EditorMode = systemManager->IsEditor();

	// Create FBO, with the width and height of the window
	m_Fbo.Create(m_Width, m_Height, m_EditorMode);
	m_GameFbo.Create(m_Width, m_Height, m_EditorMode);
	m_PingPongFbo.Create(m_Width, m_Height);

	// Set Cameras' starting position
	SetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR, {0, 0, 20});									// Position of camera
	SetCameraTarget(CAMERA_TYPE::CAMERA_TYPE_EDITOR, {0, 0, 0});									// Target of camera
	SetCameraProjection(CAMERA_TYPE::CAMERA_TYPE_ALL, 60.f, m_Window->size(), 0.1f, 900.f);			// Projection of camera

	// init game camera
	SetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_GAME, { 16.218f, 474.854f, 748.714f });
	SetCameraTarget(CAMERA_TYPE::CAMERA_TYPE_GAME, { 16.21f, 473.694f, 739.714f });

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
		if (meshRenderer.mMeshRef == nullptr)
			continue;

		// gives me the mesh
		void *tt = meshRenderer.mMeshRef;
		GFX::Mesh &meshinst = *reinterpret_cast<GFX::Mesh *>(tt);

		// pushback LTW matrices
		vec3 trans = inst.GetComponent<Transform>().mTranslate;
		if (inst.HasParent())
		{
			trans += Entity(inst.GetParent()).GetComponent<Transform>().mTranslate;
		}
		glm::mat4 trns = glm::translate(trans);
		glm::mat4 scale = glm::scale(trns, inst.GetComponent<Transform>().mScale / (meshinst.mBBOX.m_Max - meshinst.mBBOX.m_Min));
		glm::mat4 final = glm::rotate(scale, glm::radians(inst.GetComponent<Transform>().mRotate.x), glm::vec3(1.f, 0.f, 0.f));
		final = glm::rotate(final, glm::radians(inst.GetComponent<Transform>().mRotate.y), glm::vec3(0.f, 1.f, 0.f));
		final = glm::rotate(final, glm::radians(inst.GetComponent<Transform>().mRotate.z), glm::vec3(0.f, 0.f, 1.f));

		// if the debug drawing is turned on
		if (m_DebugDrawing && inst.HasComponent<BoxCollider>())
		{
			// draw the AABB of the mesh
			glm::vec3 bbox_dimens = inst.GetComponent<Transform>().mScale * inst.GetComponent<BoxCollider>().mScaleOffset;
			glm::vec3 bbox_xlate = inst.GetComponent<Transform>().mTranslate + inst.GetComponent<BoxCollider>().mTranslateOffset;
			if (inst.HasParent())
			{
				bbox_xlate += Entity(inst.GetParent()).GetComponent<Transform>().mTranslate;
			}
			m_Renderer.AddAabb(bbox_xlate, bbox_dimens, {1.f, 0.f, 0.f, 1.f});

			// draw the mesh's origin
			m_Renderer.AddSphere(m_EditorCamera.position(), inst.GetComponent<Transform>().mTranslate, 0.5f, {1.f, 1.f, 0.f, 1.f});
		}

		// Update the animation
		if (inst.HasComponent<Animator>() && _ENABLE_ANIMATIONS && systemManager->mGraphicsSystem->m_EnableGlobalAnimations)
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
		if (inst.HasComponent<Animator>()) {
			AddInstance(meshinst, final, meshRenderer.mInstanceColor, static_cast<unsigned>(inst.id), animationID++);
		}
		else {
			AddInstance(meshinst, final, meshRenderer.mInstanceColor, static_cast<unsigned>(inst.id));
		}
	}
	m_FinalBoneMatrixSsbo.SubData(finalBoneMatrices.size() * sizeof(mat4), finalBoneMatrices.data());
	finalBoneMatrices.clear();

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

		Add2DImageInstance(uiWidth, uiHeight, uiPosition, uiRenderer.ID(), static_cast<int>(inst.id));
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
		if (meshrefptr.mMeshRef == nullptr)
			continue;

		std::string meshstr = inst.GetComponent<MeshRenderer>().mMeshPath;
		if (renderedMesh.find(meshstr) != renderedMesh.end())
		{
			// the mesh has been rendered before, skip it
			continue;
		}

		// update the map of rendered meshes
		renderedMesh[meshstr] = 1;

		// render the mesh and its instances here
		GFX::Mesh& meshinst = *reinterpret_cast<GFX::Mesh*>(inst.GetComponent<MeshRenderer>().mMeshRef);
		std::string shader{};

		if (meshinst.mHasAnimation)
			shader = "AnimationShader";
		else
			shader = "PointLightShader";

		uid shaderstr(shader);
		GFX::Shader& shaderinst = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);
		unsigned shaderID = shaderinst.GetHandle();

		// bind all texture
		GFX::Texture* textureInst[4]{};
		for (int i{ 0 }; i < 4; i++)
		{
			if (inst.GetComponent<MeshRenderer>().mTextureCont[i] == true)
			{
				textureInst[i] = reinterpret_cast<GFX::Texture*>(inst.GetComponent<MeshRenderer>().mTextureRef[i]);
			}
		}

		shaderinst.Activate();
		glUniformMatrix4fv(shaderinst.GetUniformVP(), 1, GL_FALSE, &m_EditorCamera.viewProj()[0][0]);

		// Retrieve Point Light object
		auto lightEntity = systemManager->ecs->GetEntitiesWith<PointLight>();
		m_HasLight = !lightEntity.empty();

		GLuint mHasLightFlagLocation = shaderinst.GetUniformLocation("uHasLight");
		glUniform1i(mHasLightFlagLocation, m_HasLight);

		GLuint threshold = shaderinst.GetUniformLocation("bloomThreshold");
		glUniform3fv(threshold, 1, glm::value_ptr(mBloomThreshold));

		if (m_HasLight)
		{
			GLuint mViewPosShaderLocation = shaderinst.GetUniformLocation("uViewPos");
			vec3 viewPos = GetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR);
			glUniform3fv(mViewPosShaderLocation, 1, &viewPos[0]);

			// Light count uniform
			GLuint mLightCountShaderLocation = shaderinst.GetUniformLocation("uLightCount");
			glUniform1i(mLightCountShaderLocation, m_LightCount);
		}

		// bind texture unit
		for (int i{ 0 }; i < 4; i++)
		{
			if (inst.GetComponent<MeshRenderer>().mTextureCont[i] == true)
			{
				glBindTextureUnit(i, textureInst[i]->ID());
			}
		}

		GLuint uniform_tex = glGetUniformLocation(shaderID, "uTex");
		glUniform1iv(uniform_tex, (GLsizei)m_Textures.size(), m_Textures.data()); // passing Texture ID to the fragment shader

		GLuint debug_draw = glGetUniformLocation(shaderID, "uDebugDraw");
		glUniform1i(debug_draw, m_DebugDrawing);

		// Bind mesh's VAO, copy render data into VBO, Draw
		DrawAll(meshinst);

		shaderinst.Deactivate();
		meshinst.UnbindVao();

		// unbind the textures
		for (int i{0}; i < 4; i++)
		{
			if (inst.GetComponent<MeshRenderer>().mTextureCont[i] == true)
			{
				glBindTextureUnit(i, 0);
			}
		}
	}

	m_Renderer.RenderAll(m_EditorCamera.viewProj());

#pragma region render bloom
	m_PingPongFbo.PrepForDraw();
	
	// Render the bloom for the Editor Framebuffer
	uid gaussianshaderstr("GaussianBlurShader");
	GFX::Shader& gaussianShaderInst = *systemManager->mResourceTySystem->get_Shader(gaussianshaderstr.id);
	m_PingPongFbo.GaussianBlur(gaussianShaderInst, m_Fbo);

	//BlendFramebuffers(m_Fbo, m_Fbo.GetColorAttachment(), "Scene", m_PingPongFbo.pingpongColorbuffers[0], "BloomBlur");
	//BlendFramebuffers(m_Fbo, m_Fbo.GetColorAttachment(), "Scene", m_Fbo.GetBrightColorsAttachment(), "BloomBlur");
	//BlendFramebuffers(m_Fbo, m_Fbo.GetColorAttachment(), "Scene", m_Fbo.GetColorAttachment(), "BloomBlur");
#pragma endregion


	// Render UI objects
	m_UiShaderInst.Activate();		// Activate shader
	DrawAll2DInstances(m_UiShaderInst.GetHandle());
	m_UiShaderInst.Deactivate();	// Deactivate shader

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
		if (meshrefptr.mMeshRef == nullptr)
			continue;

		std::string meshstr = inst.GetComponent<MeshRenderer>().mMeshPath;
		if (renderedMesh.find(meshstr) != renderedMesh.end())
		{
			// the mesh has been rendered before, skip it
			continue;
		}

		// update the map of rendered meshes
		renderedMesh[meshstr] = 1;

		// render the mesh and its instances here
		GFX::Mesh &meshinst = *reinterpret_cast<GFX::Mesh *>(inst.GetComponent<MeshRenderer>().mMeshRef);

		// gets the shader filepath
		uid shaderstr("PointLightShader");
		GFX::Shader &shaderinst = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);

		GFX::Texture *textureInst[4]{};
		for (int i{0}; i < 4; i++)
		{
			if (inst.GetComponent<MeshRenderer>().mTextureCont[i] == true)
			{
				textureInst[i] = reinterpret_cast<GFX::Texture *>(inst.GetComponent<MeshRenderer>().mTextureRef[i]);
			}
		}

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
		}

		for (int i{0}; i < 4; i++)
		{
			if (inst.GetComponent<MeshRenderer>().mTextureCont[i] == true)
			{
				glBindTextureUnit(i, textureInst[i]->ID());
			}
		}

		// Bind mesh's VAO, copy render data into VBO, Draw
		DrawAll(meshinst);

		meshinst.UnbindVao();
		shaderinst.Deactivate();

		// unbind the textures
		for (int i{0}; i < 4; i++)
		{
			if (inst.GetComponent<MeshRenderer>().mTextureCont[i] == true)
			{
				glBindTextureUnit(i, 0);
			}
		}

		meshinst.ClearInstances();
		m_Renderer.ClearInstances();
	}

	// Render UI objects
	m_UiShaderInst.Activate();		// Activate shader
	DrawAll2DInstances(m_UiShaderInst.GetHandle());
	m_UiShaderInst.Deactivate();	// Deactivate Shader

	m_GameFbo.Unbind();
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
void GraphicsSystem::AddInstance(GFX::Mesh &mesh, Transform transform, const vec4& color, unsigned entityID)
{
	// Local to world transformation
	mat4 scale = glm::scale(transform.mScale);
	mat4 translate = glm::translate(transform.mTranslate);

	glm::quat quatRotate = glm::quat(transform.mRotate); // Retrieve quaternions via euler angles
	glm::mat4 rotation = glm::toMat4(quatRotate);		 // Retrieve rotation matrix via quaternions

	mat4 world = translate * rotation * scale;
	mesh.mLTW.push_back(world);
	mesh.mTexEntID.push_back(vec4(0, (float)entityID + 0.5f, 0, 0));
	mesh.mColors.push_back(color);
}

void GraphicsSystem::AddInstance(GFX::Mesh &mesh, mat4 transform, const vec4& color, unsigned entityID, int animInstanceID)
{
	mesh.mLTW.push_back(transform);
	mesh.mColors.push_back(color);
	if (animInstanceID < 0)
		animInstanceID = -2;
	mesh.mTexEntID.push_back(vec4(0, (float)entityID + 0.5f, (float)animInstanceID + 0.5f, 0));
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

/***************************************************************************/
/*!
\brief
	Updates the camera and its input according to the enum provided
*/
/**************************************************************************/
void GraphicsSystem::UpdateCamera(CAMERA_TYPE type, const float &dt)
{
	auto localcamera = systemManager->ecs->GetEntitiesWith<Camera>();
	if (localcamera.empty())
		return; // Cannot find camera Richmond
	Entity camera = localcamera.front();

	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		Camera_Input::getInstance().updateCameraInput(camera.GetComponent<Camera>().mCamera, dt);
		camera.GetComponent<Camera>().mCamera.Update();
		camera.GetComponent<Transform>().mTranslate = camera.GetComponent<Camera>().mCamera.mPosition;
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		Camera_Input::getInstance().updateCameraInput(m_EditorCamera, dt);
		m_EditorCamera.Update();
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		m_EditorCamera.Update();
		camera.GetComponent<Camera>().mCamera.Update();

		if (m_CameraControl == CAMERA_TYPE::CAMERA_TYPE_EDITOR) {
			Camera_Input::getInstance().updateCameraInput(m_EditorCamera, dt);
		}

		else if (m_CameraControl == CAMERA_TYPE::CAMERA_TYPE_GAME) {
			Camera_Input::getInstance().updateCameraInput(camera.GetComponent<Camera>().mCamera, dt);
			camera.GetComponent<Transform>().mTranslate = camera.GetComponent<Camera>().mCamera.mPosition;
		}

		break;
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
void GraphicsSystem::BlendFramebuffers(	GFX::FBO& targetFramebuffer,
										unsigned int Attachment0, std::string Attachment0Uniform,
										unsigned int Attachment1, std::string Attachment1Uniform)
{

	uid shaderstr("AdditiveBlendShader");
	GFX::Shader& BlendShader = *systemManager->mResourceTySystem->get_Shader(shaderstr.id);

	BlendShader.Activate();
	
	//targetFramebuffer.Bind();
	glBindTexture(GL_TEXTURE_2D, targetFramebuffer.GetColorAttachment());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Attachment0);									// bind the first attachment
	//glUniform1i(BlendShader.GetUniformLocation(Attachment0Uniform.c_str()), 0); // set the first texture to the shader

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Attachment1);									// bind the second attachment
	//glUniform1i(BlendShader.GetUniformLocation(Attachment1Uniform.c_str()), 1); // set the second texture to the shader	

	{
		mScreenQuad.Bind();

		//glClearColor(0.f, 0.f, 0.f, 1.f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mScreenQuad.Unbind();
	}

	BlendShader.Deactivate();
	//targetFramebuffer.Unbind();
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

void GraphicsSystem::SetupShaderStorageBuffers()
{
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


void MeshRenderer::SetMesh(const std::string& meshName)
{
	std::string descFilepath = systemManager->mResourceTySystem->fbx_path + meshName + ".fbx.desc";
	unsigned guid = _GEOM::GetGUID(descFilepath);

	mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(guid));
	mMeshPath = systemManager->mResourceTySystem->compiled_geom_path + meshName + ".geom";
}


void MeshRenderer::SetTexture(MaterialType MaterialType, const std::string& Texturename)
{
	std::string MaterialInstancePath = systemManager->mResourceTySystem->compressed_texture_path + Texturename + ".ctexture";
	mMaterialInstancePath[MaterialType] = MaterialInstancePath;

	mTextureCont[MaterialType] = true;

	uid guid(MaterialInstancePath);
	mTextureRef[MaterialType] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(guid.id));
}


void PointLight::SetColor(const vec3& color)
{
	mLightColor = color;
}
