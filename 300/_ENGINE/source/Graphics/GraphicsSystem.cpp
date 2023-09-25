#include <Graphics/GraphicsSystem.h>
#include <ResourceManager.h>
#include "ResourceManagerTy.h"
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
	SetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_ALL, { 50, 0, 200 });							// Position of camera
	SetCameraTarget(CAMERA_TYPE::CAMERA_TYPE_ALL, { 0, 0, 0 });									// Target of camera
	SetCameraProjection(CAMERA_TYPE::CAMERA_TYPE_ALL, 60.f, m_Window->size(), 0.1f, 900.f);		// Projection of camera

	UpdateCamera(CAMERA_TYPE::CAMERA_TYPE_ALL, 0.f);

	 //Create a new entity here, for testing purposes
	Entity newentity = systemManager->ecs->NewEntity();			// creating a new entity
	newentity.AddComponent<MeshRenderer>();
	//newentity.GetComponent<MeshRenderer>().mMeshPath = "../compiled_geom/dancing_vampire.geom";
	//newentity.GetComponent<MeshRenderer>().mMaterialInstancePath = "../assets/Compressed/Vampire_diffuse.ctexture";
	//newentity.GetComponent<MeshRenderer>().mShaderPath = std::pair<std::string, std::string>( "../_GRAPHICS/shader_files/draw_vert.glsl", "../_GRAPHICS/shader_files/draw_frag.glsl" );
	newentity.AddComponent<BoxCollider>();

	//newentity.GetComponent<MeshRenderer>().mMaterialInstancePath = "../assets/Compressed/Skull.ctexture";
	//newentity.GetComponent<MeshRenderer>().mMeshPath = "../compiled_geom/Skull_textured.geom";
	//newentity.GetComponent<MeshRenderer>().mShaderPath = { "../_GRAPHICS/shader_files/draw_vert.glsl", "../_GRAPHICS/shader_files/draw_frag.glsl" };
	//newentity.GetComponent<MeshRenderer>().mMeshPath = "../assets/compiled_geom/FreeModelNathan_WalkAnim.geom";
	newentity.GetComponent<MeshRenderer>().mMeshPath = "../assets/compiled_geom/dancing_vampire.geom";
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[0]="../assets/Compressed/Vampire_diffuse.ctexture";
	newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[1]="../assets/Compressed/Vampire_normal.ctexture";
	newentity.GetComponent<MeshRenderer>().mShaderPath = { "../_GRAPHICS/shader_files/pointLight_vert.glsl", "../_GRAPHICS/shader_files/pointLight_frag.glsl" };	// for point light
	
	
	uid temp(newentity.GetComponent<MeshRenderer>().mMeshPath);
	newentity.GetComponent<MeshRenderer>().mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(temp.id));

	uid mat1(newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[DIFFUSE]);
	newentity.GetComponent<MeshRenderer>().mTextureRef[DIFFUSE] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(mat1.id));

	uid mat2(newentity.GetComponent<MeshRenderer>().mMaterialInstancePath[NORMAL]);
	newentity.GetComponent<MeshRenderer>().mTextureRef[NORMAL] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(mat2.id));


	newentity.GetComponent<BoxCollider>().mTranslateOffset = { 0.f, 1.05f, 0.f };
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

	// To be removed once entity to be drawn created
	//m_Renderer.AddSphere(m_EditorCamera.position(), { 0, 0, -300 }, 100.f, { 0, 1, 0, 1 });
	//m_Renderer.AddAabb({ -50, -60, -200 }, { 10, 30, -0.1f }, { 1, 0, 0, 1 });

	// Retrieve and update the mesh instances to be drawn
	auto meshRendererInstances = systemManager->ecs->GetEntitiesWith<MeshRenderer>();
	for (Entity inst : meshRendererInstances)
	{

		std::string meshstr = inst.GetComponent<MeshRenderer>().mMeshPath;


		// get the mesh filepath
		
		//if( )
	//	GFX::Mesh& meshinst = systemManager->mResourceSystem->get_Mesh(meshstr);						// loads the mesh

		void* tt = inst.GetComponent<MeshRenderer>().mMeshRef;
		GFX::Mesh& meshinst = *reinterpret_cast<GFX::Mesh*>(tt);


		uid temp(meshstr);
		//GFX::Mesh& meshinst = *(systemManager->mResourceTySystem->get_mesh(temp.id));

		// pushback LTW matrices
		glm::mat4	trns = glm::translate(inst.GetComponent<Transform>().mTranslate);
		glm::mat4	rot = glm::rotate(trns, glm::radians(inst.GetComponent<Transform>().mRotate.x), glm::vec3(1.f, 0.f, 0.f));
		rot = glm::rotate(rot, glm::radians(inst.GetComponent<Transform>().mRotate.y), glm::vec3(0.f, 1.f, 0.f));
		rot = glm::rotate(rot, glm::radians(inst.GetComponent<Transform>().mRotate.z), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 final = glm::scale(rot, inst.GetComponent<Transform>().mScale);

		if (m_DebugDrawing && inst.HasComponent<BoxCollider>())
		{
			// draw the AABB of the mesh
			std::cout << inst.GetComponent<Transform>().mTranslate.x << ", " << inst.GetComponent<Transform>().mTranslate.y << ", " << inst.GetComponent<Transform>().mTranslate.z  << " || " << inst.GetComponent<BoxCollider>().mTranslateOffset.x << ", " << inst.GetComponent<BoxCollider>().mTranslateOffset.y << ", " << inst.GetComponent<BoxCollider>().mTranslateOffset.z << std::endl;
			glm::vec3 bbox_dimens = meshinst.mBBOX.m_Max - meshinst.mBBOX.m_Min;
			m_Renderer.AddAabb(inst.GetComponent<Transform>().mTranslate + inst.GetComponent<BoxCollider>().mTranslateOffset, bbox_dimens, {1.f, 0.f, 0.f, 1.f});

			// draw the center of the mesh
			m_Renderer.AddSphere(m_EditorCamera.position(), inst.GetComponent<Transform>().mTranslate, 0.5f, { 1.f, 1.f, 0.f, 1.f });
		}

		meshinst.mLTW.push_back(final);
	}

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
		//GFX::Mesh& meshinst = systemManager->mResourceSystem->get_Mesh(meshstr);						// loads the mesh
		void* tt = inst.GetComponent<MeshRenderer>().mMeshRef;
		GFX::Mesh& meshinst = *reinterpret_cast<GFX::Mesh*>(tt);
		//uid temp(meshstr);
		//GFX::Mesh& meshinst = *(systemManager->mResourceTySystem->get_mesh(temp.id));
		// gets the shader filepath
		std::pair<std::string, std::string> shaderstr = inst.GetComponent<MeshRenderer>().mShaderPath;
		std::string concatname = shaderstr.first + shaderstr.second;
		GFX::Shader& shaderinst = systemManager->mResourceSystem->get_Shader(concatname);				// loads the shader

		// get the texture filepath
		//std::vector<std::string> texturestr = inst.GetComponent<MeshRenderer>().mMaterialInstancePath;
	//	GFX::Texture& textureColorinst = systemManager->mResourceSystem->get_MaterialInstance(texturestr[0]);	// loads the texture
		//GFX::Texture& textureNormalinst = systemManager->mResourceSystem->get_MaterialInstance(texturestr[1]);	// loads the texture
		GFX::Texture& textureColorinst = *reinterpret_cast<GFX::Texture*>(inst.GetComponent<MeshRenderer>().mTextureRef[DIFFUSE]);
		GFX::Texture& textureNormalinst = *reinterpret_cast<GFX::Texture*>(inst.GetComponent<MeshRenderer>().mTextureRef[NORMAL]);

		shaderinst.Activate();
		meshinst.BindVao();
		meshinst.PrepForDraw();

		glUniformMatrix4fv(shaderinst.GetUniformVP(), 1, GL_FALSE, &m_EditorCamera.viewProj()[0][0]);

		GLuint mLightPosShaderLocation = glGetUniformLocation(shaderinst.GetHandle(), "lightPos");
		vec3 lightPos = GetCameraPosition(CAMERA_TYPE::CAMERA_TYPE_EDITOR);
		glUniform3fv(mLightPosShaderLocation, 1, &lightPos[0]);

		// bind texture unit
		glBindTextureUnit(0, textureColorinst.ID());
		glBindTextureUnit(1, textureNormalinst.ID());

		m_Textures.push_back(0);
		m_Textures.push_back(1);

		GLuint uniform_tex = glGetUniformLocation(shaderinst.GetHandle(), "uTex");
		glUniform1iv(uniform_tex, (GLsizei)m_Textures.size(), m_Textures.data());	// passing Texture ID to the fragment shader

		glDrawElementsInstanced(GL_TRIANGLES, meshinst.GetIndexCount(), GL_UNSIGNED_INT, nullptr, meshinst.mLTW.size());

		shaderinst.Deactivate();
		meshinst.UnbindVao();
		m_Textures.clear();
		glBindTextureUnit(0, 0);
		glBindTextureUnit(1, 0);
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
