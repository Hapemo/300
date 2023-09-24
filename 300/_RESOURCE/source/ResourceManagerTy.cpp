#include "ResourceManagerTy.h"
#include <memory>

ResourceTy::ResourceTy() {
	//type<int> temp ;
	//temp.data = 2;	
}




void ResourceTy::Init()
{
	for (int i = 0, end = (int)m_Infobuffer.size() - 1; i != end; ++i)
	{
		m_Infobuffer[i].m_pData = &m_Infobuffer[i + 1];
	}
	m_Infobuffer[m_Infobuffer.size() - 1].m_pData = nullptr;
	m_pInfoBufferEmptyHead = m_Infobuffer.data();


	mesh_Loader();
	std::cout << "Initializing Resource Manager.\n";
}

void ResourceTy::Exit() {

	for (auto& data : m_ResourceInstance) {

		delete  reinterpret_cast<GFX::Mesh*>(data.second->m_pData);
	}


}

void ResourceTy::mesh_Loader()
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
			//++mMeshManager.mResouceCnt;

			//std::cout << filepath << "im ur father\n";

			uid uids(filepath);
			GFX::Mesh* meshPtr = SetupMesh(filepath, uids.id);
			instance_infos& tempInstance = AllocRscInfo();


			tempInstance.m_pData = reinterpret_cast<void*>(meshPtr);
			tempInstance.m_Name = filepath;
			tempInstance.m_GUID = uids.id;
			tempInstance.m_Type = _MESH;

			
			m_ResourceInstance.emplace(uids.id, &tempInstance);
		}

		//std::cout << m_ResourceInstance.size()<<"zxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
	}
}

GFX::Mesh* ResourceTy::get_mesh(unsigned id) {

	return reinterpret_cast<GFX::Mesh*>(m_ResourceInstance[id]->m_pData);

}


GFX::Mesh* ResourceTy::SetupMesh(std::string filepath, unsigned id)
{
	_GEOM::Geom GeomData;
	GFX::Mesh localmesh;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;

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

	//uid uidd(entry.path().filename().string());

	std::cout << " data for file path " << filepath << "\n"; // testing 
	//uid uids(filepath);
	//MeshData& temp = AllocRscInfo();
	//temp.meshdata = std::move(localmesh);
	//mSceneMeshes.emplace(std::make_pair(id, &temp));

	auto Meshret = std::make_unique<GFX::Mesh>(localmesh);
	return Meshret.release();


}


instance_infos& ResourceTy::AllocRscInfo(void)
{
	auto pTemp = m_pInfoBufferEmptyHead;


	instance_infos* pNext = reinterpret_cast<instance_infos*>(m_pInfoBufferEmptyHead->m_pData);
	m_pInfoBufferEmptyHead = pNext;

	return *pTemp;
}

void ResourceTy::ReleaseRscInfo(instance_infos& RscInfo)
{
	// Add this resource info to the empty chain

	//MeshData& temp = mMeshManager.get_Mesh(RscInfo.m_GUID.id);
	//mMeshManager.ReleaseRscInfo(temp);

	RscInfo.m_pData = m_pInfoBufferEmptyHead;
	m_pInfoBufferEmptyHead = &RscInfo;

	--mResouceCnt;
	//--mMeshManager.mResouceCnt;
}