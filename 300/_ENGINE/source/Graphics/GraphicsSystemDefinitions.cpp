#include <Graphics/GraphicsSystemDefinitions.h>
#include <Graphics/GraphicsSystem.h>
#include <omp.h>


void InitUIMeshes()
{
	systemManager->mGraphicsSystem->m_HealthbarMesh.Setup2DImageMesh();
	systemManager->mGraphicsSystem->m_Image2DMesh.Setup2DImageMesh();
	systemManager->mGraphicsSystem->m_PortalMesh.Setup2DImageMesh();
	systemManager->mGraphicsSystem->m_ParticleMesh.Setup2DImageMesh();

	for (int i{}; i < 32; ++i)
	{
		systemManager->mGraphicsSystem->m_Textures.emplace_back(i);
	}
}

void InitFramebuffers()
{
	int width = systemManager->mGraphicsSystem->m_Width;
	int height = systemManager->mGraphicsSystem->m_Height;
	bool editormode = systemManager->mGraphicsSystem->m_EditorMode;

	systemManager->mGraphicsSystem->m_Fbo.Create(width, height, editormode);
	systemManager->mGraphicsSystem->m_GameFbo.Create(width, height, editormode);
	systemManager->mGraphicsSystem->m_MultisampleFBO.Create(width, height);
	systemManager->mGraphicsSystem->m_IntermediateFBO.Create(width, height);
	systemManager->mGraphicsSystem->m_PingPongFbo.Create(width, height);
	systemManager->mGraphicsSystem->m_PhysBloomRenderer.Init(width, height);
	systemManager->mGraphicsSystem->m_ShadowFbo.Create(width * 4, height * 4);

	// Input
	glBindImageTexture(2, systemManager->mGraphicsSystem->m_IntermediateFBO.GetBrightColorsAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(3, systemManager->mGraphicsSystem->m_IntermediateFBO.GetFragPosAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(4, systemManager->mGraphicsSystem->m_IntermediateFBO.GetNormalAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(5, systemManager->mGraphicsSystem->m_IntermediateFBO.GetAlbedoSpecAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(6, systemManager->mGraphicsSystem->m_IntermediateFBO.GetEmissionAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
}

void animationsToChangeMeshDelayedCheck(Entity inst, const float& dt)
{
	Animator& animatorInst = inst.GetComponent<Animator>();

	if (animatorInst.mAnimator.mToChangeMeshDelayed.first && animatorInst.mAnimator.m_CurrentAnimation != nullptr)
	{
		if(animatorInst.IsEndOfAnimation())
		{
			// the mesh change delayed flag is set and the animation is completed, change the mesh
			MeshRenderer& meshRenderer = inst.GetComponent<MeshRenderer>();
			animatorInst.mAnimator.mToChangeMeshDelayed.first = false;
			meshRenderer.SetMesh(animatorInst.mAnimator.mToChangeMeshDelayed.second, inst);
		}
	}
}

void updateBloomValues(GFX::Mesh& meshinst)
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
		lAmbientBloomThreshold = vec4(systemManager->mGraphicsSystem->mAmbientBloomThreshold, 1.f);
	}

	shaderinst.Activate();
	GLuint threshold = shaderinst.GetUniformLocation("globalBloomThreshold");
	glUniform4fv(threshold, 1, glm::value_ptr(lAmbientBloomThreshold));
	shaderinst.Deactivate();
}


void update_CalculateLTW(Entity inst, GFX::Mesh& meshinst, Transform& transforminst, mat4& final)
{
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
			mat4 viewToWorld = glm::inverse(systemManager->mGraphicsSystem->GetCameraViewMatrix(CAMERA_TYPE::CAMERA_TYPE_GAME));
			T = viewToWorld * T;
		}
		else
		{
			trans += parent_translate;
			T = glm::translate(trans);
		}
	}

	final = T * R * S;


	// if the debug drawing is turned on
	if (systemManager->mGraphicsSystem->m_DebugDrawing)
	{
		if (inst.HasComponent<BoxCollider>())
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
			glm::mat4 bboxRotate = glm::toMat4(glm::quat(glm::radians(transforminst.mRotate)));

			glm::mat4 bboxFinal = bboxTranslate * bboxRotate * bboxScale;

			systemManager->mGraphicsSystem->m_Renderer.AddAabb(bboxFinal, { 1.f, 0.f, 0.f, 1.f });
		}

		if (inst.HasComponent<CapsuleCollider>())
		{
			CapsuleCollider& cap = inst.GetComponent<CapsuleCollider>();
			if (inst.HasParent())
				transforminst.mTranslate += static_cast<Entity>(inst.GetParent()).GetComponent<Transform>().mTranslate;

			glm::vec3 capPos = transforminst.mTranslate + cap.mTranslateOffset;

			glm::vec3 first = capPos;
			glm::vec3 second = capPos;
			first.y -= cap.mHalfHeight;
			second.y += cap.mHalfHeight;

			systemManager->mGraphicsSystem->m_Renderer.AddCapsule(systemManager->mGraphicsSystem->m_EditorCamera.position(), first, second, cap.mRadius, glm::vec4(0.f, 1.f, 0.f, 1.f));
		}

		// Draw the axes
		static const vec3 origin{ -180.f, -100.f, 250.f };
		systemManager->mGraphicsSystem->m_Renderer.AddLine(origin, origin + vec3{ 100.f, 0.f, 0.f }, { 1.f, 0.f, 0.f, 1.f });
		systemManager->mGraphicsSystem->m_Renderer.AddLine(origin, origin + vec3{ 0.f, 100.f, 0.f }, { 0.f, 1.f, 0.f, 1.f });
		systemManager->mGraphicsSystem->m_Renderer.AddLine(origin, origin + vec3{ 0.f, 0.f, 100.f }, { 0.f, 0.f, 1.f, 1.f });
	}
}


void updateAnimations(Entity inst, const mat4& final, const float& dt)
{
	Animator& animatorInst = inst.GetComponent<Animator>();

	// skip the mesh that does not have an animation set
	if (animatorInst.mAnimator.m_CurrentAnimation != nullptr)
	{
		animatorInst.mAnimator.UpdateAnimation(dt, mat4(1.f), final); // update the current animation
	}

#pragma omp parallel for
	for (size_t i = 0; i < animatorInst.mAnimator.m_FinalBoneMatrices.size(); ++i) {
		// Access each element using the index i and push it back into the SSBO
		systemManager->mGraphicsSystem->finalBoneMatrices.push_back(animatorInst.mAnimator.m_FinalBoneMatrices[i]);
	}
}


void updateMaterials(MeshRenderer& meshRenderer)
{
	auto getID = [&](MaterialType type, MeshRenderer& meshrenderer) ->int {

		if (meshrenderer.mTextureRef[static_cast<int>(type)].getdata(systemManager->mResourceTySystem->m_ResourceInstance) == nullptr)
			return -1;

		return static_cast<GFX::Texture*>(meshrenderer.mTextureRef[static_cast<int>(type)].data)->ID();
	};

	MaterialSSBO material{};
	material.mDiffuseMap	= systemManager->mGraphicsSystem->GetAndStoreBindlessTextureHandle(getID(DIFFUSE,	meshRenderer));
	material.mNormalMap		= systemManager->mGraphicsSystem->GetAndStoreBindlessTextureHandle(getID(NORMAL,	meshRenderer));
	material.mSpecularMap	= systemManager->mGraphicsSystem->GetAndStoreBindlessTextureHandle(getID(SPECULAR,	meshRenderer));
	material.mShininessMap	= systemManager->mGraphicsSystem->GetAndStoreBindlessTextureHandle(getID(SHININESS, meshRenderer));
	material.mEmissionMap	= systemManager->mGraphicsSystem->GetAndStoreBindlessTextureHandle(getID(EMISSION,	meshRenderer));
	material.mAOMap			= systemManager->mGraphicsSystem->GetAndStoreBindlessTextureHandle(getID(AO,		meshRenderer));

	systemManager->mGraphicsSystem->m_Materials.emplace_back(material);	// push back
}


void update_NonMeshrendererColliders()
{
	if (systemManager->mGraphicsSystem->m_DebugDrawing)
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

			systemManager->mGraphicsSystem->m_Renderer.AddAabb(bboxFinal, { 1.f, 0.f, 0.f, 1.f });
		}
	}
}


void updateSSBO_Data()
{
	systemManager->mGraphicsSystem->m_FinalBoneMatrixSsbo.SubData(systemManager->mGraphicsSystem->finalBoneMatrices.size() * sizeof(mat4), systemManager->mGraphicsSystem->finalBoneMatrices.data());
	systemManager->mGraphicsSystem->m_MaterialSsbo.SubData(systemManager->mGraphicsSystem->m_Materials.size() * sizeof(MaterialSSBO), systemManager->mGraphicsSystem->m_Materials.data());
	systemManager->mGraphicsSystem->m_UITextureSsbo.SubData(systemManager->mGraphicsSystem->UITextures.size() * sizeof(GLuint64), systemManager->mGraphicsSystem->UITextures.data());
}


void update_Light()
{
	// Sending Light source data to GPU
	{	
		auto lightEntity = systemManager->ecs->GetEntitiesWith<General, PointLight>();
		systemManager->mGraphicsSystem->m_HasLight = !lightEntity.empty();
		systemManager->mGraphicsSystem->m_LightCount = static_cast<int>(lightEntity.size());

		if (systemManager->mGraphicsSystem->m_HasLight)
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

				systemManager->mGraphicsSystem->pointLights.push_back(light);

				systemManager->mGraphicsSystem->m_Renderer.AddCube(lightTransform.mTranslate, { 2, 2, 2 }, vec4(lightData.mLightColor, 1.f));
			}
			// Copy light source data into storage buffer
			systemManager->mGraphicsSystem->m_PointLightSsbo.SubData(systemManager->mGraphicsSystem->pointLights.size() * sizeof(PointLightSSBO), systemManager->mGraphicsSystem->pointLights.data());
		}
	}

	// Spotlight
	{
		auto spotlightEntity = systemManager->ecs->GetEntitiesWith<Spotlight>();
		systemManager->mGraphicsSystem->m_SpotlightCount = static_cast<int>(spotlightEntity.size());

		for (int i{}; i < spotlightEntity.size(); ++i)
		{
			Spotlight& lightData = spotlightEntity.get<Spotlight>(spotlightEntity[i]);
			Transform& lightTransform = Entity(spotlightEntity[i]).GetComponent<Transform>();

			SpotLightSSBO light;
			light.mPosition = vec4(lightTransform.mTranslate, 0.f);
			light.mDirection = vec4(lightData.mDirection, 0.0f);
			light.mColor = vec4(lightData.mColor, 0.f);
			light.mIntensity = lightData.mIntensity;
			light.mCutoff = glm::cos(glm::radians(lightData.mCutoff));
			light.mOuterCutoff = glm::cos(glm::radians(lightData.mOuterCutoff));

			systemManager->mGraphicsSystem->spotlights.push_back(light);

			systemManager->mGraphicsSystem->m_Renderer.AddCube(lightTransform.mTranslate, { 2, 2, 2 }, vec4(lightData.mColor, 1.f));
			systemManager->mGraphicsSystem->m_Renderer.AddLine(light.mPosition, light.mPosition + light.mDirection * 10.f, vec4(lightData.mColor, 1.f));
		}
		systemManager->mGraphicsSystem->m_SpotlightSsbo.SubData(systemManager->mGraphicsSystem->spotlights.size() * sizeof(SpotLightSSBO), systemManager->mGraphicsSystem->spotlights.data());
	}
}


void Reset_Data()
{
	// Clear data from previous frame
	systemManager->mGraphicsSystem->m_Image2DMesh.ClearInstances();		
	systemManager->mGraphicsSystem->m_PortalMesh.ClearInstances();
	systemManager->mGraphicsSystem->m_Image2DStore.clear();
	systemManager->mGraphicsSystem->finalBoneMatrices.clear();
	systemManager->mGraphicsSystem->m_Materials.clear();
	systemManager->mGraphicsSystem->UITextures.clear();
	systemManager->mGraphicsSystem->pointLights.clear();
	systemManager->mGraphicsSystem->spotlights.clear();
}


void update_UI()
{
	auto UiInstances = systemManager->ecs->GetEntitiesWith<UIrenderer>();
	for (Entity inst : UiInstances)
	{
		if (inst.GetComponent<General>().name == "") continue;
		UIrenderer& uiRenderer = inst.GetComponent<UIrenderer>();
		Transform& uiTransform = inst.GetComponent<Transform>();

		/*if (inst.HasParent())
		{
			Transform parentTransform = inst.GetParent().GetComponent<Transform>();
			uiTransform = parentTransform;
		}*/

		float width = systemManager->mGraphicsSystem->m_Width;
		float height = systemManager->mGraphicsSystem->m_Height;
		float uiWidth = uiTransform.mScale.x * width / 2.f;
		float uiHeight = uiTransform.mScale.y * height / 2.f;
		float depth = (int)uiRenderer.mLayer;
		// Maps depth from 0-255 to 0-1
		depth /= 255.f;
		vec3 uiPosition = vec3(uiTransform.mTranslate.x * width, uiTransform.mTranslate.y * height, depth);

		unsigned texID{};
		if (uiRenderer.mTextureRef.getdata(systemManager->mResourceTySystem->m_ResourceInstance) != nullptr)
			texID = reinterpret_cast<GFX::Texture*>(uiRenderer.mTextureRef.data)->ID();

		int texIndex{};
		if (texID > 0)
			texIndex = systemManager->mGraphicsSystem->StoreUITexture(texID);
		else
			texIndex = -2;

		if (uiRenderer.mWorldTransform)
		{
			Transform xform = uiTransform;
			if (inst.HasParent())	// Compute parent's offset
			{
				vec3 parent_translate = Entity(inst.GetParent()).GetComponent<Transform>().mTranslate;

				// Compute view to world
				if (inst.GetParent().GetComponent<TAG>() == TAG::PLAYER)	// if parent is the Player
				{
					// view space --> world space
					mat4 viewToWorld = glm::inverse(systemManager->mGraphicsSystem->GetCameraViewMatrix(CAMERA_TYPE::CAMERA_TYPE_GAME));
					xform.mTranslate = vec3(viewToWorld * vec4(uiTransform.mTranslate, 1.0));
				}
				else
				{
					xform.mTranslate += parent_translate;
				}
			}
			systemManager->mGraphicsSystem->Add2DImageWorldInstance(xform, texIndex, static_cast<int>(inst.id), uiRenderer.mDegree, uiRenderer.mColor, uiRenderer.mSlider);
		}
		else
			systemManager->mGraphicsSystem->Add2DImageInstance(uiWidth, uiHeight, uiPosition, texIndex, static_cast<int>(inst.id), uiRenderer.mDegree, uiRenderer.mColor, uiRenderer.mSlider);
	}

	systemManager->mGraphicsSystem->m_Image2DMesh.PrepForDraw();
}


void update_Portals()
{
	auto portals = systemManager->ecs->GetEntitiesWith<Portal>();
	for (Entity p : portals)
	{
		systemManager->mGraphicsSystem->AddPortalInstance(p);

		Portal& portal = p.GetComponent<Portal>();

		Transform srcTransform;
		srcTransform.mTranslate = portal.mTranslate1;
		srcTransform.mRotate = portal.mRotate1;
		Transform destTransform;
		destTransform.mTranslate = portal.mTranslate2;
		destTransform.mRotate = portal.mRotate2;

		mat4 destViewProj = systemManager->mGraphicsSystem->GetPortalViewMatrix(systemManager->mGraphicsSystem->m_EditorCamera, srcTransform, destTransform);

		systemManager->mGraphicsSystem->m_Renderer.AddFrustum(destViewProj, vec4(0.f, 0.f, 1.f, 1.f));
	}

	systemManager->mGraphicsSystem->m_PortalMesh.PrepForDraw();
}


void render_Debugdraw(const mat4& camVP)
{
	systemManager->mGraphicsSystem->m_Renderer.RenderAll(camVP);
	systemManager->mGraphicsSystem->m_Renderer.ClearInstances();
}