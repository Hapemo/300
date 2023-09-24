#include <Graphics/GraphicsSystem.h>
#include <ResourceManager.h>
#include <Graphics/Camera_Input.h>

/***************************************************************************/
/*!
\brief
	Initialize the Graphics system
*/
/**************************************************************************/
void GraphicsSystem::Init()
{
	// Get Window Handle
	m_Window = systemManager->GetWindow();
	m_Width = m_Window->size().x;
	m_Height = m_Window->size().y;

	m_EditorMode = systemManager->IsEditor();

	// Create FBO, with the width and height of the window
	m_Fbo.Create(m_Width, m_Height, m_EditorMode);

	// Set Cameras' starting position
	SetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_ALL, { 0, 0, 20 });								// Position of camera
	SetCameraTarget(CAMERA_TYPE::CAMERA_TYPE_ALL, { 0, 0, 0 });									// Target of camera
	SetCameraProjection(CAMERA_TYPE::CAMERA_TYPE_ALL, 60.f, m_Window->size(), 0.1f, 900.f);		// Projection of camera

	UpdateCamera(CAMERA_TYPE::CAMERA_TYPE_ALL, 0.f);

	// Create a new entity here, for testing purposes
	Entity newentity = systemManager->ecs->NewEntity();			// creating a new entity
	newentity.AddComponent<MeshRenderer>();
	newentity.AddComponent<BoxCollider>();

	//newentity.GetComponent<MeshRenderer>().mMaterialInstancePath = "../assets/Compressed/Skull.ctexture";
	//newentity.GetComponent<MeshRenderer>().mMeshPath = "../compiled_geom/Skull_textured.geom";
	//newentity.GetComponent<MeshRenderer>().mShaderPath = { "../_GRAPHICS/shader_files/draw_vert.glsl", "../_GRAPHICS/shader_files/draw_frag.glsl" };
	//newentity.GetComponent<MeshRenderer>().mMeshPath = "../assets/compiled_geom/FreeModelNathan_WalkAnim.geom";
	//newentity.GetComponent<MeshRenderer>().mMeshPath = "../assets/compiled_geom/dancing_vampire.geom";
	newentity.GetComponent<MeshRenderer>().mMeshPath = "../assets/compiled_geom/dancing_vampire.geom";
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath.emplace_back("../assets/Compressed/Vampire_diffuse.ctexture");
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath.emplace_back("../assets/Compressed/Vampire_normal.ctexture");
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath.emplace_back("../assets/Compressed/Vampire_emission.ctexture");
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath.emplace_back("../assets/Compressed/Vampire_specular.ctexture");
	newentity.GetComponent<MeshRenderer>().mShaderPath = { "../_GRAPHICS/shader_files/pointLight_vert.glsl", "../_GRAPHICS/shader_files/pointLight_frag.glsl" };	// for point light

	newentity.GetComponent<BoxCollider>().mTranslateOffset = { 0.f, 1.05f, 0.f };

	auto& meshinst = systemManager->mResourceSystem->get_Mesh("../assets/compiled_geom/dancing_vampire.geom");
	if (meshinst.mHasAnimation && _ENABLE_ANIMATIONS)
	{
		newentity.GetComponent<MeshRenderer>().mShaderPath = { "../_GRAPHICS/shader_files/animations_vert.glsl", "../_GRAPHICS/shader_files/pointLight_frag.glsl" };// for point light
		m_Animator.SetAnimation(&meshinst.mAnimation[0]);
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
	// local variable to keep track of rendered mesh instances
	std::map<std::string, short> renderedMesh;

	// update the camera's transformations, and its input
	UpdateCamera(CAMERA_TYPE::CAMERA_TYPE_EDITOR, dt);

	// Retrieve and update the mesh instances to be drawn
	auto meshRendererInstances = systemManager->ecs->GetEntitiesWith<MeshRenderer>();
	for (Entity inst : meshRendererInstances)
	{
		// get the mesh filepath
		std::string meshstr = inst.GetComponent<MeshRenderer>().mMeshPath;
		GFX::Mesh& meshinst = systemManager->mResourceSystem->get_Mesh(meshstr);						// loads the mesh

		// pushback LTW matrices
		glm::mat4 scale = glm::scale(mat4(1.0f), inst.GetComponent<Transform>().mScale);
		glm::mat4	rot = glm::rotate(mat4(1.0f), glm::radians(inst.GetComponent<Transform>().mRotate.x), glm::vec3(1.f, 0.f, 0.f));
					rot = glm::rotate(rot, glm::radians(inst.GetComponent<Transform>().mRotate.y), glm::vec3(0.f, 1.f, 0.f));
					rot = glm::rotate(rot, glm::radians(inst.GetComponent<Transform>().mRotate.z), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4	trns = glm::translate(inst.GetComponent<Transform>().mTranslate);
		//glm::mat4 final = trns * rot * scale;
		glm::mat4 final = glm::mat4(1.f);

		// if the debug drawing is turned on 
		if (m_DebugDrawing && inst.HasComponent<BoxCollider>())
		{
			// draw the AABB of the mesh
			glm::vec3 bbox_dimens = meshinst.mBBOX.m_Max - meshinst.mBBOX.m_Min;
			bbox_dimens = bbox_dimens * inst.GetComponent<Transform>().mScale * inst.GetComponent<BoxCollider>().mScaleOffset;
			m_Renderer.AddAabb(inst.GetComponent<Transform>().mTranslate + inst.GetComponent<BoxCollider>().mTranslateOffset, bbox_dimens, {1.f, 0.f, 0.f, 1.f});

			// draw the mesh's origin
			m_Renderer.AddSphere(m_EditorCamera.position(), inst.GetComponent<Transform>().mTranslate, 0.5f, { 1.f, 1.f, 0.f, 1.f });

			// draw the animation data for the bones
			if (meshinst.mHasAnimation && _ENABLE_ANIMATIONS)
			{
				// draw the mesh's bone positions as boxes
				for (const auto& bones : meshinst.mAnimation[0].m_Bones)
				{
					static const vec3 bonescale(0.1f, 0.1f, 0.1f);
					vec4 bonestrns = m_Animator.m_FinalBoneMatrices[bones.GetBoneID()] * vec4(inst.GetComponent<Transform>().mTranslate, 1.f);

					m_Renderer.AddAabb({ bonestrns.x, bonestrns.y, bonestrns.z }, bonescale, vec4(1.f, 1.f, 0.f, 1.f));
				}
			}
		}

		meshinst.mLTW.push_back(final);

		if (m_Animator.m_CurrentAnimation != nullptr && _ENABLE_ANIMATIONS)
		{
			// update the current animation
			m_Animator.UpdateAnimation(dt, final);
		}
	}
	m_Renderer.AddCube({ -10, 0, 0 }, { 2, 15, 2 }, { 1.f, 0., 0.f, 1.f });

	// Prepare and bind the Framebuffer to be rendered on
	m_Fbo.PrepForDraw();
	m_Renderer.RenderAll(m_EditorCamera.viewProj());
	m_Renderer.ClearInstances();

	// Render all instances of a given mesh
	for (Entity inst : meshRendererInstances)
	{
		std::string meshstr = inst.GetComponent<MeshRenderer>().mMeshPath;
		if (renderedMesh.find(meshstr) != renderedMesh.end()) {
			// the mesh has been rendered before, skip it
			continue;
		}

		// update the map
		renderedMesh[meshstr] = 1;

		// render the mesh and its instances here
		GFX::Mesh& meshinst = systemManager->mResourceSystem->get_Mesh(meshstr);								// loads the mesh

		// gets the shader filepath
		std::pair<std::string, std::string> shaderstr = inst.GetComponent<MeshRenderer>().mShaderPath;
		std::string concatname = shaderstr.first + shaderstr.second;
		GFX::Shader& shaderinst = systemManager->mResourceSystem->get_Shader(concatname);						// loads the shader
		unsigned shaderID = shaderinst.GetHandle();

		// get the texture filepath
		std::vector<std::string> texturestr = inst.GetComponent<MeshRenderer>().mMaterialInstancePath;
		GFX::Texture& textureColorinst = systemManager->mResourceSystem->get_MaterialInstance(texturestr[0]);		// loads the diffuse texture
		GFX::Texture& textureNormalinst = systemManager->mResourceSystem->get_MaterialInstance(texturestr[1]);		// loads the normal texture
		GFX::Texture& textureEmissioninst = systemManager->mResourceSystem->get_MaterialInstance(texturestr[2]);	// loads the emission texture
		GFX::Texture& textureSpecularinst = systemManager->mResourceSystem->get_MaterialInstance(texturestr[3]);	// loads the specular texture

		shaderinst.Activate();
		meshinst.BindVao();
		meshinst.PrepForDraw();

		glUniformMatrix4fv(shaderinst.GetUniformVP(), 1, GL_FALSE, &m_EditorCamera.viewProj()[0][0]);

		GLuint mLightPosShaderLocation = glGetUniformLocation(shaderID, "uLightPos");
		GLuint mViewPosShaderLocation = glGetUniformLocation(shaderID, "uViewPos");
		vec3 lightPos = GetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR);
		glUniform3fv(mLightPosShaderLocation, 1, &lightPos[0]);
		glUniform3fv(mViewPosShaderLocation, 1, &lightPos[0]);

		// bind texture unit
		glBindTextureUnit(0, textureColorinst.ID());
		glBindTextureUnit(1, textureNormalinst.ID());
		glBindTextureUnit(2, textureEmissioninst.ID());
		glBindTextureUnit(3, textureSpecularinst.ID());

		m_Textures.push_back(0);
		m_Textures.push_back(1);
		m_Textures.push_back(2);
		m_Textures.push_back(3);

		GLuint uniform_tex = glGetUniformLocation(shaderID, "uTex");
		glUniform1iv(uniform_tex, (GLsizei)m_Textures.size(), m_Textures.data());	// passing Texture ID to the fragment shader

		// send animation data over to the shader if there is animations
		if (meshinst.mHasAnimation && _ENABLE_ANIMATIONS)
		{
			const auto& transform = m_Animator.m_FinalBoneMatrices;
			size_t totaltransform = transform.size();

			for (size_t b{}; b < totaltransform; ++b)
			{
				// send the bone matrices to the shader via uniforms
				std::string name = "finalBoneMatrices[" + std::to_string(b) + "]";
				glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &transform[b][0][0]);
			}
		}

		glDrawElementsInstanced(GL_TRIANGLES, meshinst.GetIndexCount(), GL_UNSIGNED_INT, nullptr, meshinst.mLTW.size());

		shaderinst.Deactivate();
		meshinst.UnbindVao();
		m_Textures.clear();
		glBindTextureUnit(0, 0);
		glBindTextureUnit(1, 0);
		glBindTextureUnit(2, 0);
		glBindTextureUnit(3, 0);

		meshinst.ClearInstances();
	}

	// TODO: Clears all instances that have been rendered from local buffer
	m_Fbo.Unbind();
}

/***************************************************************************/
/*!
\brief
	Exit the Graphics system. Unalloc anything unnecessary
*/
/**************************************************************************/
void GraphicsSystem::Exit()
{

}

/***************************************************************************/
/*!
\brief
	Adds an instance of a mesh to be drawn, For instancing
*/
/**************************************************************************/
void GraphicsSystem::AddInstance(GFX::Mesh& mesh, Transform transform)
{
	// Local to world transformation
	mat4 scale = glm::scale(transform.mScale);
	mat4 translate = glm::translate(transform.mTranslate);

	glm::quat quatRotate = glm::quat(transform.mRotate);	// Retrieve quaternions via euler angles
	glm::mat4 rotation = glm::toMat4(quatRotate);			// Retrieve rotation matrix via quaternions

	mat4 world = translate * rotation * scale;
	mesh.mLTW.push_back(world);
}

/***************************************************************************/
/*!
\brief
	Updates the camera's position. 
*/
/**************************************************************************/
void GraphicsSystem::SetCameraPosition(CAMERA_TYPE type, vec3 position)
{
	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		m_GameCamera.SetPosition(position);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		m_EditorCamera.SetPosition(position);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		m_GameCamera.SetPosition(position);
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
	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		m_GameCamera.SetTarget(position);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		m_EditorCamera.SetTarget(position);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		m_GameCamera.SetTarget(position);
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
	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		m_GameCamera.SetProjection(fovDegree, size, nearZ, farZ);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		m_EditorCamera.SetProjection(fovDegree, size, nearZ, farZ);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		m_GameCamera.SetProjection(fovDegree, size, nearZ, farZ);
		m_EditorCamera.SetProjection(fovDegree, size, nearZ, farZ);
		break;
	}
}

void GraphicsSystem::SetCameraSize(CAMERA_TYPE type, ivec2 size)
{
	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		m_GameCamera.SetSize(size);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		m_EditorCamera.SetSize(size);
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		m_GameCamera.SetSize(size);
		m_EditorCamera.SetSize(size);
		break;
	}
}

void GraphicsSystem::UpdateCamera(CAMERA_TYPE type, const float& dt)
{
	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		Camera_Input::getInstance().updateCameraInput(m_GameCamera, dt);
		m_GameCamera.Update();
		break;

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		Camera_Input::getInstance().updateCameraInput(m_EditorCamera, dt);
		m_EditorCamera.Update();
		break;

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		Camera_Input::getInstance().updateCameraInput(m_GameCamera, dt);
		Camera_Input::getInstance().updateCameraInput(m_EditorCamera, dt);
		m_GameCamera.Update();
		m_EditorCamera.Update();
		break;
	}
}

vec3 GraphicsSystem::GetCameraPosition(CAMERA_TYPE type)
{
	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		return m_GameCamera.position();

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		return m_EditorCamera.position();

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		break;
	}
}

vec3 GraphicsSystem::GetCameraTarget(CAMERA_TYPE type)
{
	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		return m_GameCamera.target();

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		return m_EditorCamera.target();

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		break;
	}
}

vec3 GraphicsSystem::GetCameraDirection(CAMERA_TYPE type)
{
	switch (type)
	{
	case CAMERA_TYPE::CAMERA_TYPE_GAME:
		return m_GameCamera.direction();

	case CAMERA_TYPE::CAMERA_TYPE_EDITOR:
		return m_EditorCamera.direction();

	case CAMERA_TYPE::CAMERA_TYPE_ALL:
		break;
	}
}

void GraphicsSystem::DrawAll(GFX::Mesh& mesh)
{
	mesh.DrawAllInstances();
}

void GraphicsSystem::PrintMat4(const glm::mat4& input)
{
	std::cout << "=============================================================================\n";
	std::cout << "| " << input[0][0] << " | " << input[1][0] << " | " << input[2][0] << " | " << input[3][0] << "\n";
	std::cout << "| " << input[0][1] << " | " << input[1][1] << " | " << input[2][1] << " | " << input[3][1] << "\n";
	std::cout << "| " << input[0][2] << " | " << input[1][2] << " | " << input[2][2] << " | " << input[3][2] << "\n";
	std::cout << "| " << input[0][3] << " | " << input[1][3] << " | " << input[2][3] << " | " << input[3][3] << "\n";
	std::cout << "=============================================================================\n";
}