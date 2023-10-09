#include "ResourceManager.h"
#include <filesystem>

/***************************************************************************/
/*!
\brief
	Constructor empty
*/
/**************************************************************************/
Resource::Resource() {
	// empty
}

/***************************************************************************/
/*!
\brief
	Initalize Resource Manager
*/
/**************************************************************************/
void Resource::Init() 
{
	for (int i = 0, end = (int)m_Infobuffer.size() - 1; i != end; ++i)
	{
		m_Infobuffer[i].m_pData = &m_Infobuffer[i + 1];
	}
	m_Infobuffer[m_Infobuffer.size() - 1].m_pData = nullptr;
	m_pInfoBufferEmptyHead = m_Infobuffer.data();


	std::cout << "Initializing Resource Manager.\n";

	mMeshManager.Init();
	mShaderManager.Init();
	mMaterialInstanceManager.Init();

	mesh_Loader();
	shader_Loader();
	MaterialInstance_Loader();
}
/***************************************************************************/
/*!
\brief
	Allocate resource for the Resource manager
*/
/**************************************************************************/
instance_info& Resource::AllocRscInfo(void)
{
    auto pTemp = m_pInfoBufferEmptyHead;


    instance_info* pNext = reinterpret_cast<instance_info*>(m_pInfoBufferEmptyHead->m_pData);
    m_pInfoBufferEmptyHead = pNext;

    return *pTemp;
}
/***************************************************************************/
/*!
\brief
	DeAllocate resource for the Resource manager
*/
/**************************************************************************/
void Resource::ReleaseRscInfo(instance_info& RscInfo)
{
    // Add this resource info to the empty chain

	MeshData& temp = mMeshManager.get_Mesh(RscInfo.m_GUID.id);
	mMeshManager.ReleaseRscInfo(temp);

    RscInfo.m_pData = m_pInfoBufferEmptyHead;
    m_pInfoBufferEmptyHead = &RscInfo;

	--mResouceCnt;
	--mMeshManager.mResouceCnt;
}
/***************************************************************************/
/*!
\brief
	Loads compiled mesh ( geom ) into Engine
*/
/**************************************************************************/
void Resource::mesh_Loader() 
{
	std::filesystem::path folderpath = compiled_geom_path.c_str();

	// Reads through all the files in the folder, and loads them into the mesh
	for (const auto& entry : std::filesystem::directory_iterator(folderpath))
	{
		if (std::filesystem::is_regular_file(entry))
		{
			std::cout << "============================================\n";
			std::cout << "[NOTE]>> Loading file: \t" << entry.path().filename() << "\n";

			std::string filepath = compiled_geom_path + entry.path().filename().string();

			++mResouceCnt;
			++mMeshManager.mResouceCnt;

			uid uids(filepath);
			mMeshManager.SetupMesh(filepath, uids.id);
			instance_info& tempInstance = AllocRscInfo();
			tempInstance.m_Name = filepath;
			tempInstance.m_GUID = uids.id;
			tempInstance.m_Type = MESH;
			m_Meshes.emplace(uids.id, &tempInstance);
		}
	}
}
/***************************************************************************/
/*!
\brief
	Loads GLSL shader into Engine
*/
/**************************************************************************/
void Resource::shader_Loader()
{
	// hardcode data path for now 
	// ideally, this code should read through 
	std::vector<std::pair<std::string, std::string>> shaderpaths;
	shaderpaths.emplace_back(std::pair<std::string, std::string>{ "../assets/shader_files/draw_vert.glsl", "../assets/shader_files/draw_frag.glsl" });
	shaderpaths.emplace_back(std::pair<std::string, std::string>{ "../assets/shader_files/pointLight_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" });
	shaderpaths.emplace_back(std::pair<std::string, std::string>{ "../assets/shader_files/animations_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" });
	shaderpaths.emplace_back(std::pair<std::string, std::string>{ "../assets/shader_files/UI_vert.glsl", "../assets/shader_files/UI_frag.glsl" });


	// load all the shaders
	for (const auto& x : shaderpaths)
	{
		std::string vertPath = x.first;
		std::string fragPath = x.second;
		std::string combinedPath = vertPath + fragPath;

		uid uids(combinedPath);
		mShaderManager.SetupShader(vertPath, fragPath, uids.id);

		++mResouceCnt;
		++mShaderManager.mResourceCnt;

		instance_info& tempInstance = AllocRscInfo();
		tempInstance.m_Name = combinedPath;
		tempInstance.m_GUID = uids.id;
		tempInstance.m_Type = SHADER;
		m_Shaders.emplace(uids.id, &tempInstance);
	}
}
/***************************************************************************/
/*!
\brief
	Loads Compressed texture into Engine
*/
/**************************************************************************/
void Resource::MaterialInstance_Loader()
{
	// hardcode material instance path for now 
	std::vector<std::string> materialinstancepaths;
	//materialinstancepaths.emplace_back("../assets/Compressed/Skull.ctexture");

	std::filesystem::path folderpath = compressed_texture_path.c_str();

	// Reads through all the files in the folder, and loads them into the mesh
	for (const auto& entry : std::filesystem::directory_iterator(folderpath))
	{
		std::cout << "============================================\n";
		std::cout << "[NOTE]>> Loading Compressed Texture: \t" << entry.path().filename() << "\n";

		std::string filepath = compressed_texture_path + entry.path().filename().string();
		std::string materialinstancepath = filepath;
		uid uids(materialinstancepath);
		mMaterialInstanceManager.SetupMaterialInstance(materialinstancepath, uids.id);

		++mResouceCnt;
		++mMaterialInstanceManager.mResourceCnt;

		instance_info& tempInstance = AllocRscInfo();
		tempInstance.m_Name = materialinstancepath;
		tempInstance.m_GUID = uids.id;
		tempInstance.m_Type = TEXTURE;
		m_Textures.emplace(uids.id, &tempInstance);
	}
}


/***************************************************************************/
/*!
\brief
	Allocate resource for shader manager
*/
/**************************************************************************/
ShaderData& ShaderManager::AllocRscInfo()
{
	auto pTemp = m_pInfoBufferEmptyHead;


	ShaderData* pNext = reinterpret_cast<ShaderData*>(m_pInfoBufferEmptyHead->m_pData);
	m_pInfoBufferEmptyHead = pNext;

	return *pTemp;
}
/***************************************************************************/
/*!
\brief
	DeAllocate resource for shader manager
*/
/**************************************************************************/
void ShaderManager::ReleaseRscInfo(ShaderData& RscInfo)
{
	// Add this resource info to the empty chain
	RscInfo.m_pData = m_pInfoBufferEmptyHead;
	m_pInfoBufferEmptyHead = &RscInfo;
}

ShaderData& ShaderManager::getShader(unsigned id) {
	return *mSceneShaders[id];
}
/***************************************************************************/
/*!
\brief
	Initialize shader manager
*/
/**************************************************************************/
void ShaderManager::Init()
{
	std::cout << "Initializing ShaderManager.\n";

	for (int i = 0, end = (int)m_Shaderbuffer.size() - 1; i != end; ++i)
	{
		m_Shaderbuffer[i].m_pData = &m_Shaderbuffer[i + 1];
	}
	m_Shaderbuffer[m_Shaderbuffer.size() - 1].m_pData = nullptr;
	m_pInfoBufferEmptyHead = m_Shaderbuffer.data();
}
/***************************************************************************/
/*!
\brief
	SetupShader shader manager
*/
/**************************************************************************/
void ShaderManager::SetupShader(std::string vertpath, std::string fragpath, unsigned uid)
{
	GFX::Shader localshader;
	localshader.CreateShaderFromFiles(vertpath.c_str(), fragpath.c_str());

	std::cout << " data for file path " << vertpath << "\n"; // testing 
	
	//uid uids(filepath);
	ShaderData& temp = AllocRscInfo();
	temp.shaderData = std::move(localshader);
	mSceneShaders.emplace(std::make_pair(uid, &temp));
}


/***************************************************************************/
/*!
\brief
	Allocate resource for MaterialInstanceManager manager
*/
/**************************************************************************/
MaterialInstanceData& MaterialInstanceManager::AllocRscInfo()
{
	auto pTemp = m_pInfoBufferEmptyHead;

	MaterialInstanceData* pNext = reinterpret_cast<MaterialInstanceData*>(m_pInfoBufferEmptyHead->m_pData);
	m_pInfoBufferEmptyHead = pNext;

	return *pTemp;
}
/***************************************************************************/
/*!
\brief
	DeAllocate resource for MaterialInstanceManager manager
*/
/**************************************************************************/
void MaterialInstanceManager::ReleaseRscInfo(MaterialInstanceData& RscInfo)
{
	// Add this resource info to the empty chain
	RscInfo.m_pData = m_pInfoBufferEmptyHead;
	m_pInfoBufferEmptyHead = &RscInfo;
}
/***************************************************************************/
/*!
\brief
	Return material instance reference
*/
/**************************************************************************/
MaterialInstanceData& MaterialInstanceManager::getMaterialInstance(unsigned id) {
	return *mSceneMaterialInstances[id];
}

void MaterialInstanceManager::Init()
{
	std::cout << "Initializing MaterialInstanceManager.\n";

	for (int i = 0, end = (int)m_MaterialInstancebuffer.size() - 1; i != end; ++i)
	{
		m_MaterialInstancebuffer[i].m_pData = &m_MaterialInstancebuffer[i + 1];
	}
	m_MaterialInstancebuffer[m_MaterialInstancebuffer.size() - 1].m_pData = nullptr;
	m_pInfoBufferEmptyHead = m_MaterialInstancebuffer.data();

}
/***************************************************************************/
/*!
\brief
	SetupMaterialInstance using graphics functions
*/
/**************************************************************************/
void MaterialInstanceManager::SetupMaterialInstance(std::string filepath, unsigned uid)
{
	GFX::Texture localMaterialInstance;
	localMaterialInstance.Load(filepath.c_str());

	//uid uids(filepath);
	MaterialInstanceData& temp = AllocRscInfo();
	temp.materialInstanceData = std::move(localMaterialInstance);
	mSceneMaterialInstances.emplace(std::make_pair(uid, &temp));
}


/***************************************************************************/
/*!
\brief
	Allocate resource for MeshManager manager
	*/
/**************************************************************************/
MeshData& MeshManager::AllocRscInfo()
{
	auto pTemp = m_pInfoBufferEmptyHead;


	MeshData* pNext = reinterpret_cast<MeshData*>(m_pInfoBufferEmptyHead->m_pData);
	m_pInfoBufferEmptyHead = pNext;

	return *pTemp;
}
/***************************************************************************/
/*!
\brief
	DeAllocate resource for MeshManager manager
*/
/**************************************************************************/
void MeshManager::ReleaseRscInfo(MeshData& RscInfo)
{
	// Add this resource info to the empty chain
	RscInfo.m_pData = m_pInfoBufferEmptyHead;
	m_pInfoBufferEmptyHead = &RscInfo;


}
/***************************************************************************/
/*!
\brief
	Return mesh reference
*/
/**************************************************************************/
MeshData& MeshManager::get_Mesh(unsigned id) {
	return *mSceneMeshes[id];
}
/***************************************************************************/
/*!
\brief
	Initialzie mesh Manager
*/
/**************************************************************************/
void MeshManager::Init()
{

	std::cout << "Initializing Mesh Manager.\n";

	for (int i = 0, end = (int)m_Meshbuffer.size() - 1; i != end; ++i)
	{
		m_Meshbuffer[i].m_pData = &m_Meshbuffer[i + 1];
	}
	m_Meshbuffer[m_Meshbuffer.size() - 1].m_pData = nullptr;
	m_pInfoBufferEmptyHead = m_Meshbuffer.data();

}
/***************************************************************************/
/*!
\brief
	Setup mesh using graphics engine functions
*/
/**************************************************************************/
void MeshManager::SetupMesh(std::string filepath,unsigned id)
{
	_GEOM::Geom GeomData;
	GFX::Mesh localmesh;

	Deserialization::DeserializeGeom(filepath.c_str(), GeomData);	// load the geom from the compiled geom file
	localmesh.Setup(GeomData);

	// Load animations
	if (GeomData.m_bHasAnimations && _ENABLE_ANIMATIONS)
	{
		// store the animation data of the first mesh -- there should only be be one mesh per file so far, so we just take the first index
		// We store the vector of animation data into the mesh class.
		localmesh.mAnimation = GeomData.m_pMesh[0].m_Animation;
		localmesh.mHasAnimation = true;
	}


	MeshData& temp = AllocRscInfo();
	temp.meshdata = std::move(localmesh);
	mSceneMeshes.emplace(std::make_pair(id, &temp));

}
/***************************************************************************/
/*!
\brief
	Return mesh instane reference
*/
/**************************************************************************/
GFX::Mesh& Resource::get_Mesh(std::string name) 
{
	
	//uid id(name);
	
	for (auto ins : m_Meshes) {	
		if (ins.second->m_Name == name) {
			++(m_Meshes[ins.second->m_GUID.id]->m_RefCount);
			return mMeshManager.get_Mesh(ins.second->m_GUID.id).meshdata;
		}
	}

	std::cout << "Could not find MESH Data.\n";
	return mMeshManager.get_Mesh(m_Meshes[0]->m_GUID.id).meshdata;

}
/***************************************************************************/
/*!
\brief
	Return shader Reference
*/
/**************************************************************************/
GFX::Shader& Resource::get_Shader(std::string name)
{
	for (auto ins : m_Shaders) 
	{
		if (ins.second->m_Name == name) 
		{
			++(m_Shaders[ins.second->m_GUID.id]->m_RefCount);
			return mShaderManager.getShader(ins.second->m_GUID.id).shaderData;
		}
	}
	std::cout << "Could not find SHADER Data.\n";
	return mShaderManager.getShader(m_Shaders[0]->m_GUID.id).shaderData;
}
/***************************************************************************/
/*!
\brief
	Return reference Instance for texture 
*/
/**************************************************************************/
GFX::Texture& Resource::get_MaterialInstance(std::string name)
{
	for (auto ins : m_Textures)
	{
		if (ins.second->m_Name == name)
		{
			++(m_Textures[ins.second->m_GUID.id]->m_RefCount);
			return mMaterialInstanceManager.getMaterialInstance(ins.second->m_GUID.id).materialInstanceData;
		}
	}
	std::cout << "Could not find MATERIAL_INSTANCE Data.\n";
	return mMaterialInstanceManager.getMaterialInstance(m_Textures[0]->m_GUID.id).materialInstanceData;

}