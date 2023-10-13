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

#include <Graphics/GraphicsSystem.h>
#include <ResourceManager.h>
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
	// -- WIP -- SHADER STORAGE BUFFER OBJECT
	SetupShaderStorageBuffers();
	// -- WIP -- SHADER STORAGE BUFFER OBJECT
	m_Image2DMesh.Setup2DImageMesh();

	glEnable(GL_MULTISAMPLE);

	// Get Window Handle
	m_Window = systemManager->GetWindow();
	m_Width = m_Window->size().x;
	m_Height = m_Window->size().y;

	m_EditorMode = systemManager->IsEditor();

	// Create FBO, with the width and height of the window
	m_Fbo.Create(m_Width, m_Height, m_EditorMode);
	m_GameFbo.Create(m_Width, m_Height, m_EditorMode);

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

#if 0
#pragma region create entity 1
	// Create a new entity here, for testing purposes
	Entity newentity = systemManager->ecs->NewEntity(); // creating a new entity
														//	systemManager->mGameStateSystem->mCurrentGameState.AddScene("NewScene");
	systemManager->mGameStateSystem->mCurrentGameState.mScenes[0].mEntities.insert(newentity);

	newentity.AddComponent<MeshRenderer>();
	newentity.AddComponent<BoxCollider>();
	newentity.AddComponent<Animator>();

	newentity.GetComponent<MeshRenderer>().mMeshPath = "../assets/compiled_geom/dancing_vampire.geom";
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[DIFFUSE] = "../assets/Compressed/Vampire_diffuse.ctexture";
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[NORMAL] = "../assets/Compressed/Vampire_normal.ctexture";
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[2] = "../assets/Compressed/Vampire_emission.ctexture";
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[3] = "../assets/Compressed/Vampire_specular.ctexture";
	newentity.GetComponent<MeshRenderer>().mShaderPath = {"../_GRAPHICS/shader_files/pointLight_vert.glsl", "../_GRAPHICS/shader_files/pointLight_frag.glsl"}; // for point light

	uid temp(newentity.GetComponent<MeshRenderer>().mMeshPath);
	newentity.GetComponent<MeshRenderer>().mMeshRef = reinterpret_cast<void *>(systemManager->mResourceTySystem->get_mesh(temp.id));

	uid mat1(newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[DIFFUSE]);
	newentity.GetComponent<MeshRenderer>().mTextureRef[DIFFUSE] = reinterpret_cast<void *>(systemManager->mResourceTySystem->getMaterialInstance(mat1.id));
	newentity.GetComponent<MeshRenderer>().mTextureCont[DIFFUSE] = true;

	uid mat2(newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[NORMAL]);
	newentity.GetComponent<MeshRenderer>().mTextureRef[NORMAL] = reinterpret_cast<void *>(systemManager->mResourceTySystem->getMaterialInstance(mat2.id));
	newentity.GetComponent<MeshRenderer>().mTextureCont[NORMAL] = true;
	newentity.GetComponent<BoxCollider>().mTranslateOffset = {0.f, 80.f, 0.f};
	newentity.GetComponent<BoxCollider>().mScaleOffset = {1.f, 0.8f, 1.5f};

	auto &meshinst = systemManager->mResourceSystem->get_Mesh("../assets/compiled_geom/dancing_vampire.geom");
	newentity.GetComponent<Transform>().mScale = meshinst.mBBOX.m_Max - meshinst.mBBOX.m_Min;
	if (newentity.HasComponent<Animator>() && _ENABLE_ANIMATIONS)
	{
		newentity.GetComponent<MeshRenderer>().mShaderPath = {"../_GRAPHICS/shader_files/animations_vert.glsl", "../_GRAPHICS/shader_files/pointLight_frag.glsl"}; // for point light
		newentity.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst.mAnimation[0]);
	}
#pragma endregion
#endif

#if 0
#pragma region create entity 2
	//Create a new entity here, for testing purposes
	Entity newentity1 = systemManager->ecs->NewEntity();			// creating a new entity
	systemManager->mGameStateSystem->mCurrentGameState.mScenes[0].mEntities.insert(newentity1);
	newentity1.AddComponent<MeshRenderer>();
	newentity1.AddComponent<BoxCollider>();
	newentity1.AddComponent<Animator>();

	newentity1.GetComponent<MeshRenderer>().mMeshPath = "../assets/compiled_geom/GirlAnimationWalking.geom";
	newentity1.GetComponent<MeshRenderer>().mMaterialInstancePath[0] = "../assets/Compressed/Girl_Diffuse.ctexture";
	newentity1.GetComponent<MeshRenderer>().mMaterialInstancePath[1] = "../assets/Compressed/Girl_Normal.ctexture";
	newentity1.GetComponent<MeshRenderer>().mMaterialInstancePath[2] = "../assets/Compressed/Girl_Glossiness.ctexture";
	newentity1.GetComponent<MeshRenderer>().mMaterialInstancePath[3] = "../assets/Compressed/Girl_Specular.ctexture";

	newentity1.GetComponent<MeshRenderer>().mShaderPath = { "../_GRAPHICS/shader_files/pointLight_vert.glsl", "../_GRAPHICS/shader_files/pointLight_frag.glsl" };	// for point light

	uid mat3(newentity1.GetComponent<MeshRenderer>().mMaterialInstancePath[DIFFUSE]);
	newentity1.GetComponent<MeshRenderer>().mTextureRef[DIFFUSE] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(mat3.id));
	newentity1.GetComponent<MeshRenderer>().mTextureCont[DIFFUSE] = true;


	uid mat4(newentity1.GetComponent<MeshRenderer>().mMaterialInstancePath[NORMAL]);
	newentity1.GetComponent<MeshRenderer>().mTextureRef[NORMAL] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(mat4.id));
	newentity1.GetComponent<MeshRenderer>().mTextureCont[NORMAL] = true;


	uid temp1(newentity1.GetComponent<MeshRenderer>().mMeshPath);
	newentity1.GetComponent<MeshRenderer>().mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(temp1.id));

	//newentity1.GetComponent<Transform>().mTranslate = { 300.f, 0.f, 0.f };
	newentity1.GetComponent<BoxCollider>().mTranslateOffset = { -2.f, 98.f, 0.f };
	newentity1.GetComponent<BoxCollider>().mScaleOffset = { 0.5f, 4.2f, 0.4f };

	auto& meshinst1 = systemManager->mResourceSystem->get_Mesh("../assets/compiled_geom/GirlAnimationWalking.geom");
	newentity1.GetComponent<Transform>().mScale = meshinst1.mBBOX.m_Max - meshinst1.mBBOX.m_Min;
	if (newentity1.HasComponent<Animator>() && _ENABLE_ANIMATIONS)
	{
		newentity1.GetComponent<MeshRenderer>().mShaderPath = { "../_GRAPHICS/shader_files/animations_vert.glsl", "../_GRAPHICS/shader_files/pointLight_frag.glsl" };// for point light
		newentity1.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst1.mAnimation[0]);
	}
#pragma endregion
#endif
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
	// -- WIP --  SHADER STORAGE BUFFER OBJECT
	finalBoneMatrices.push_back(mat4(1.0f));
	finalBoneMatrices.push_back(mat4(2.0f));
	//ShaderStorageBufferSubData(finalBoneMatrices.size() * sizeof(mat4), finalBoneMatrices.data());
	finalBoneMatrices.clear();
	// -- WIP --  SHADER STORAGE BUFFER OBJECT

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
	for (Entity inst : meshRendererInstances)
	{
	
		auto& meshrefptr = inst.GetComponent<MeshRenderer>();
		if (meshrefptr.mMeshRef == nullptr)
			continue;

		std::string meshstr = inst.GetComponent<MeshRenderer>().mMeshPath;
		//Animator anim = inst.GetComponent<Animator>();

		void *tt = inst.GetComponent<MeshRenderer>().mMeshRef;
		GFX::Mesh &meshinst = *reinterpret_cast<GFX::Mesh *>(tt);

		uid temp(meshstr);
		// GFX::Mesh& meshinst = *(systemManager->mResourceTySystem->get_mesh(temp.id));

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
			// skip the mesh that does not have an animation set
			if (inst.GetComponent<Animator>().mAnimator.m_CurrentAnimation != nullptr)
			{
				inst.GetComponent<Animator>().mAnimator.UpdateAnimation(dt, mat4(1.f), final); // update the current animation

				// push back matrices into the SSBO
				for (const auto& x : inst.GetComponent<Animator>().mAnimator.m_FinalBoneMatrices)
				{
					finalBoneMatrices.push_back(x);
				}
			}
		}

		// meshinst.mLTW.push_back(final);
		AddInstance(meshinst, final, static_cast<unsigned>(inst.id));
	}
	m_FinalBoneMatrixSsbo.SubData(finalBoneMatrices.size() * sizeof(mat4), finalBoneMatrices.data());
	finalBoneMatrices.clear();

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

#pragma region render all the mesh instances
	// Render all instances of a given mesh
	for (Entity inst : meshRendererInstances)
	{
		auto& meshrefptr = inst.GetComponent<MeshRenderer>();
		if (meshrefptr.mMeshRef == nullptr)
			continue;

		//if (inst.HasParent())
		//{
		//	Transform xform = inst.GetComponent<Transform>();
		//	xform.mTranslate += Entity(inst.GetParent()).GetComponent<Transform>().mTranslate;
		//}

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

		// gets the shader filepathc
		std::pair<std::string, std::string> shaderstr = inst.GetComponent<MeshRenderer>().mShaderPath;
		GFX::Shader &shaderinst = systemManager->mResourceSystem->get_Shader(shaderstr.first + shaderstr.second);
		unsigned shaderID = shaderinst.GetHandle();

		// bind all texture
		GFX::Texture *textureInst[4]{};
		for (int i{0}; i < 4; i++)
		{
			if (inst.GetComponent<MeshRenderer>().mTextureCont[i] == true)
			{
				textureInst[i] = reinterpret_cast<GFX::Texture *>(inst.GetComponent<MeshRenderer>().mTextureRef[i]);
			}
		}
	
		shaderinst.Activate();
		glUniformMatrix4fv(shaderinst.GetUniformVP(), 1, GL_FALSE, &m_EditorCamera.viewProj()[0][0]);
		// Retrieve Point Light object
		auto lightEntity = systemManager->ecs->GetEntitiesWith<PointLight>();
		m_HasLight = !lightEntity.empty();

		GLuint mHasLightFlagLocation = shaderinst.GetUniformLocation("uHasLight");
		glUniform1i(mHasLightFlagLocation, m_HasLight);

		if (m_HasLight)
		{
			GLuint mViewPosShaderLocation = shaderinst.GetUniformLocation("uViewPos");
			vec3 viewPos = GetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR);
			glUniform3fv(mViewPosShaderLocation, 1, &viewPos[0]);

			for (int i = 0; i < lightEntity.size(); ++i)
			{
				PointLight &lightData = lightEntity.get<PointLight>(lightEntity[i]);
				Transform &lightTransform = Entity(lightEntity[i]).GetComponent<Transform>();

				PointLightSSBO light;
				light.mPosition		= vec4(lightTransform.mTranslate, 0.f);
				light.mColor		= vec4(lightData.mLightColor, 0.f);
				light.mIntensity	= lightData.mIntensity;
				light.mLinear		= lightData.mLinearFalloff;
				light.mQuadratic	= lightData.mQuadraticFalloff;

				pointLights.push_back(light);

				m_Renderer.AddCube(lightTransform.mTranslate, { 20, 20, 20 }, vec4(lightData.mLightColor, 1.f));
			}
			GLuint mLightCountShaderLocation = shaderinst.GetUniformLocation("uLightCount");
			glUniform1i(mLightCountShaderLocation, (int)pointLights.size());

			m_PointLightSsbo.SubData(pointLights.size() * sizeof(PointLightSSBO), pointLights.data());
		}

		// bind texture unit
		for (int i{0}; i < 4; i++)
		{
			if (inst.GetComponent<MeshRenderer>().mTextureCont[i] == true)
			{
				glBindTextureUnit(i, textureInst[i]->ID());
			}
		}

		m_Textures.push_back(0);
		m_Textures.push_back(1);
		m_Textures.push_back(2);
		m_Textures.push_back(3);

		GLuint uniform_tex = glGetUniformLocation(shaderID, "uTex");
		glUniform1iv(uniform_tex, (GLsizei)m_Textures.size(), m_Textures.data()); // passing Texture ID to the fragment shader

		GLuint debug_draw = glGetUniformLocation(shaderID, "uDebugDraw");
		glUniform1i(debug_draw, m_DebugDrawing);

		//// send animation data over to the shader if there is animations
		//if (inst.HasComponent<Animator>() && _ENABLE_ANIMATIONS)
		//{
		//	const auto &transform = inst.GetComponent<Animator>().mAnimator.m_FinalBoneMatrices;
		//	size_t totaltransform = transform.size();

		//	for (size_t b{}; b < totaltransform; ++b)
		//	{
		//		// send the bone matrices to the shader via uniforms
		//		std::string name = "finalBoneMatrices[" + std::to_string(b) + "]";
		//		glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &transform[b][0][0]);
		//	}
		//}

		// Bind mesh's VAO, copy render data into VBO, Draw
		DrawAll(meshinst);

		shaderinst.Deactivate();
		meshinst.UnbindVao();
		m_Textures.clear();

		// unbind the textures
		for (int i{0}; i < 4; i++)
		{
			if (inst.GetComponent<MeshRenderer>().mTextureCont[i] == true)
			{
				glBindTextureUnit(i, 0);
			}
		}
	}

	// Debug draw stuffs should be drawn last
	m_Renderer.RenderAll(m_EditorCamera.viewProj());
#pragma endregion

	// TODO: Clears all instances that have been rendered from local buffer
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

	// m_Renderer.RenderAll(camera.GetComponent<Camera>().mCamera.viewProj());

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
		std::pair<std::string, std::string> shaderstr = inst.GetComponent<MeshRenderer>().mShaderPath;
		GFX::Shader &shaderinst = systemManager->mResourceSystem->get_Shader(shaderstr.first + shaderstr.second);

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

		m_Textures.push_back(0);
		m_Textures.push_back(1);
		m_Textures.push_back(2);
		m_Textures.push_back(3);

		// Bind mesh's VAO, copy render data into VBO, Draw
		DrawAll(meshinst);
		// glDrawElementsInstanced(GL_TRIANGLES, meshinst.GetIndexCount(), GL_UNSIGNED_INT, nullptr, meshinst.mLTW.size());

		meshinst.UnbindVao();
		shaderinst.Deactivate();
		m_Textures.clear();

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
	pointLights.clear();

	// TODO: Clears all instances that have been rendered from local buffer
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
void GraphicsSystem::AddInstance(GFX::Mesh &mesh, Transform transform, unsigned entityID)
{
	// Local to world transformation
	mat4 scale = glm::scale(transform.mScale);
	mat4 translate = glm::translate(transform.mTranslate);

	glm::quat quatRotate = glm::quat(transform.mRotate); // Retrieve quaternions via euler angles
	glm::mat4 rotation = glm::toMat4(quatRotate);		 // Retrieve rotation matrix via quaternions

	mat4 world = translate * rotation * scale;
	mesh.mLTW.push_back(world);
	mesh.mTexEntID.push_back(vec4(0, (float)entityID + 0.5f, 0, 0));
}

void GraphicsSystem::AddInstance(GFX::Mesh &mesh, mat4 transform, unsigned entityID)
{
	mesh.mLTW.push_back(transform);
	mesh.mTexEntID.push_back(vec4(0, (float)entityID + 0.5f, 0, 0));
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
	PERROR("camera spoil - graphicssystem.cpp line 809");
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

void GraphicsSystem::SetupShaderStorageBuffers()
{
	// All Point Lights in the Scene -- Location 2
	m_PointLightSsbo.Create(sizeof(PointLightSSBO) * MAX_POINT_LIGHT, 2);

	// Final Bone Matrix for Animation -- Location 3
	m_FinalBoneMatrixSsbo.Create(sizeof(mat4) * MAX_NUM_BONES * MAX_INSTANCES, 3);
}

void GraphicsSystem::Add2DImageInstance(float width, float height, vec2 const& position, unsigned texHandle, vec4 const& color, unsigned entityID)
{
	mat4 world =
	{
		vec4(width, 0.f, 0.f, 0.f),
		vec4(0.f, height, 0.f, 0.f),
		vec4(0.f, 0.f, 1.f, 0.f),
		vec4(position, 0.f, 1.f)
	};

	int texIndex = StoreTextureIndex(texHandle);

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
