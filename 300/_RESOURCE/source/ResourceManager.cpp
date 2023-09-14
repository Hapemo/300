#include "ResourceManager.h"
#include <filesystem>

Resource::Resource() {
	// empty
}


void Resource::Init() {
	for (int i = 0, end = (int)m_Infobuffer.size() - 1; i != end; ++i)
	{
		m_Infobuffer[i].m_pData = &m_Infobuffer[i + 1];
	}
	m_Infobuffer[m_Infobuffer.size() - 1].m_pData = nullptr;
	m_pInfoBufferEmptyHead = m_Infobuffer.data();


	std::cout << "Initializing Resource Manager.\n";

	mMeshManager.Init();


	mesh_Loader();
	shader_Loader();
}

instance_info& Resource::AllocRscInfo(void)
{
    auto pTemp = m_pInfoBufferEmptyHead;


    instance_info* pNext = reinterpret_cast<instance_info*>(m_pInfoBufferEmptyHead->m_pData);
    m_pInfoBufferEmptyHead = pNext;

    return *pTemp;
}

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

			//uid uids("dasdsadsadasdassssssssssadaddddddddddddddddddddddddddddddddddddddddddddddddddddadadsd");
			//std::cout << uids.id<< "\n";

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

void Resource::shader_Loader()
{
	// hardcode data path for now 
	const std::string vertPath = "..\_GRAPHICS\shader_files\draw_vert.glsl";
	const std::string fragPath = "..\_GRAPHICS\shader_files\draw_frag.glsl";
	std::string combinedPath = vertPath + fragPath;

	uid uids(combinedPath);
	mShaderManager.SetupShader(vertPath, fragPath, uids.id);

	instance_info& tempInstance = AllocRscInfo();
	tempInstance.m_Name = combinedPath;
	tempInstance.m_GUID = uids.id;
	tempInstance.m_Type = SHADER;
	m_Shaders.emplace(uids.id, &tempInstance);
}

void Resource::MaterialInstance_Loader()
{
	// hardcode material instance path for now 
	const std::string materialinstancepath = "..\assets\Compressed\Skull.ctexture";


}



ShaderData& ShaderManager::AllocRscInfo()
{
	auto pTemp = m_pInfoBufferEmptyHead;


	ShaderData* pNext = reinterpret_cast<ShaderData*>(m_pInfoBufferEmptyHead->m_pData);
	m_pInfoBufferEmptyHead = pNext;

	return *pTemp;
}

void ShaderManager::ReleaseRscInfo(ShaderData& RscInfo)
{
	// Add this resource info to the empty chain
	RscInfo.m_pData = m_pInfoBufferEmptyHead;
	m_pInfoBufferEmptyHead = &RscInfo;
}

ShaderData& ShaderManager::getShader(unsigned id) {
	return *mSceneShaders[id];
}

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



MaterialInstanceData& MaterialInstanceManager::AllocRscInfo()
{
	auto pTemp = m_pInfoBufferEmptyHead;


	MaterialInstanceData* pNext = reinterpret_cast<MaterialInstanceData*>(m_pInfoBufferEmptyHead->m_pData);
	m_pInfoBufferEmptyHead = pNext;

	return *pTemp;
}

void MaterialInstanceManager::ReleaseRscInfo(MaterialInstanceData& RscInfo)
{
	// Add this resource info to the empty chain
	RscInfo.m_pData = m_pInfoBufferEmptyHead;
	m_pInfoBufferEmptyHead = &RscInfo;
}

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

void MaterialInstanceManager::SetupMaterialInstance(std::string filepath, unsigned uid)
{
	GFX::Texture localMaterialInstance;
	localMaterialInstance.Load(filepath.c_str());

	std::cout << " data for file path " << filepath << "\n"; // testing 

	//uid uids(filepath);
	MaterialInstanceData& temp = AllocRscInfo();
	temp.materialInstanceData = std::move(localMaterialInstance);
	mSceneMaterialInstances.emplace(std::make_pair(uid, &temp));
}



MeshData& MeshManager::AllocRscInfo()
{
	auto pTemp = m_pInfoBufferEmptyHead;


	MeshData* pNext = reinterpret_cast<MeshData*>(m_pInfoBufferEmptyHead->m_pData);
	m_pInfoBufferEmptyHead = pNext;

	return *pTemp;
}

void MeshManager::ReleaseRscInfo(MeshData& RscInfo)
{
	// Add this resource info to the empty chain
	RscInfo.m_pData = m_pInfoBufferEmptyHead;
	m_pInfoBufferEmptyHead = &RscInfo;


}

MeshData& MeshManager::get_Mesh(unsigned id) {
	return *mSceneMeshes[id];
}

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

void MeshManager::SetupMesh(std::string filepath,unsigned id)
{
	_GEOM::Geom GeomData;
	GFX::Mesh localmesh;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;

	Deserialization::DeserializeGeom(filepath.c_str(), GeomData);	// load the geom from the compiled geom file
	//localmesh.Setup(GeomData);

	// Load animations
	if (GeomData.m_bHasAnimations)
	{
		// store the animation data of the first mesh -- there should only be be one mesh per file so far, so we just take the first index
		// We store the vector of animation data into the mesh class.
		localmesh.mAnimation = GeomData.m_pMesh[0].m_Animation;
		localmesh.mHasAnimation = true;
	}

	//mSceneMeshes.emplace_back(localmesh);							// storage of all the scene's meshes


	//uid uidd(entry.path().filename().string());

	std::cout << " data for file path " << filepath << "\n"; // testing 
	//uid uids(filepath);
	MeshData& temp = AllocRscInfo();
	temp.meshdata = std::move(localmesh);
	mSceneMeshes.emplace(std::make_pair(id, &temp));

}

GFX::Mesh& Resource::get_Mesh(std::string name) {
	
	//uid id(name);
	
	for (auto ins : m_Meshes) {	
		if (ins.second->m_Name == name) {
			++(m_Meshes[ins.second->m_GUID.id]->m_RefCount);
			return mMeshManager.get_Mesh(ins.second->m_GUID.id).meshdata;
		}
	}

	std::cout << "Could not find MESH Data.\n";

}