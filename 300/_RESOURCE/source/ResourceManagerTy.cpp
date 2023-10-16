#include "ResourceManagerTy.h"
#include <map>
#include <memory>


#define  _ENABLE_ANIMATIONS 1
/***************************************************************************/
/*!
\brief
	Constructor for Resource Manager
*/
/**************************************************************************/
ResourceTy::ResourceTy() {
}



/***************************************************************************/
/*!
\brief
	Initalize Resource Manager
*/
/**************************************************************************/
void ResourceTy::Init()
{
	for (int i = 0, end = (int)m_Infobuffer.size() - 1; i != end; ++i)
	{
		m_Infobuffer[i].m_pData = &m_Infobuffer[i + 1];
	}
	m_Infobuffer[m_Infobuffer.size() - 1].m_pData = nullptr;
	m_pInfoBufferEmptyHead = m_Infobuffer.data();


	shader_Loader();
	mesh_LoadFolder();
	MaterialInstance_Loader();
	MaterialEditor_Loader();
	std::cout << "Initializing Resource Manager.\n";
}
/***************************************************************************/
/*!
\brief
	Exit (delete necessary data allocated) Resource Manager
*/
/**************************************************************************/
void ResourceTy::Exit() {

	for (auto& data : m_ResourceInstance) {
		if( data.second->m_Type == _MESH)
			delete  reinterpret_cast<GFX::Mesh*>(data.second->m_pData);
		else if (data.second->m_Type == _TEXTURE)
			delete  reinterpret_cast<GFX::Texture*>(data.second->m_pData);
		else if (data.second->m_Type == _SHADER)
			delete  reinterpret_cast<GFX::Shader*>(data.second->m_pData);
	}

	for (auto& data : m_EditorTextures) {
		delete  reinterpret_cast<GFX::Texture*>(data.second);
	}
}
/***************************************************************************/
/*!
\brief
	Loads mesh (geom) from file
*/
/**************************************************************************/
void ResourceTy::mesh_Load(std::string filepath, unsigned uid)
{
	std::cout << "[NOTE]>> Loading file: \t" << filepath << "\n";
	std::cout << "[NOTE]>> Loading uid: \t" << uid << "\n";

	++mResouceCnt;
	GFX::Mesh* meshPtr = SetupMesh(filepath, uid);
	instance_infos& tempInstance = AllocRscInfo();

	tempInstance.m_pData = reinterpret_cast<void*>(meshPtr);
	tempInstance.m_Name = filepath;
	tempInstance.m_GUID = uid;
	tempInstance.m_Type = _MESH;
			
	m_ResourceInstance.emplace(uid, &tempInstance);
}

bool check_extensions(std::string file, std::string extension) {
	size_t path_length = file.length();
	std::string path_extension = file.substr(path_length - extension.length());
	if (path_extension == extension)
		return true;

	return false;
}

void ResourceTy::mesh_LoadFolder()
{
	std::filesystem::path folderpath = compiled_geom_path.c_str();

	// Reads through all the files in the folder, and loads them into the mesh
	for (const auto& entry : std::filesystem::directory_iterator(folderpath))
	{
		if (std::filesystem::is_regular_file(entry))
		{
			std::cout << "============================================\n";
			std::cout << "[NOTE]>> Loading file: \t" << entry.path().filename() << "\n";

			if(!check_extensions(entry.path().filename().string(), ".geom"))
				continue;

			std::string filepath = compiled_geom_path + entry.path().filename().string();

			++mResouceCnt;
			//uid uids(filepath);

			std::string descfilepath = filepath + ".desc";
			unsigned guid = _GEOM::GetGUID(descfilepath);

			GFX::Mesh* meshPtr = SetupMesh(filepath, guid);
			instance_infos& tempInstance = AllocRscInfo();


			tempInstance.m_pData = reinterpret_cast<void*>(meshPtr);
			tempInstance.m_Name = filepath;
			tempInstance.m_GUID = guid;
			tempInstance.m_Type = _MESH;


			m_ResourceInstance.emplace(guid, &tempInstance);
		}
	}
}

/***************************************************************************/
/*!
\brief
	Return mesh pointer
*/
/**************************************************************************/
GFX::Mesh* ResourceTy::get_mesh(unsigned id) {

	return reinterpret_cast<GFX::Mesh*>(m_ResourceInstance[id]->m_pData);

}

/***************************************************************************/
/*!
\brief
	Setup mesh through graphics engine
*/
/**************************************************************************/
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


	std::cout << " data for file path " << filepath << "\n"; // testing 

	auto Meshret = std::make_unique<GFX::Mesh>(localmesh);
	return Meshret.release();


}

/***************************************************************************/
/*!
\brief
	Allocated data for Resource Manager
*/
/**************************************************************************/
instance_infos& ResourceTy::AllocRscInfo(void)
{
	auto pTemp = m_pInfoBufferEmptyHead;


	instance_infos* pNext = reinterpret_cast<instance_infos*>(m_pInfoBufferEmptyHead->m_pData);
	m_pInfoBufferEmptyHead = pNext;

	return *pTemp;
}
/***************************************************************************/
/*!
\brief
	DeAllocated data for Resource Manager
*/
/**************************************************************************/
void ResourceTy::ReleaseRscInfo(instance_infos& RscInfo)
{
	// Add this resource info to the empty chain
	RscInfo.m_pData = m_pInfoBufferEmptyHead;
	m_pInfoBufferEmptyHead = &RscInfo;

	--mResouceCnt;
}

/***************************************************************************/
/*!
\brief
	Loads Material instance(texture comrpessed) from file
*/
/**************************************************************************/
void ResourceTy::MaterialInstance_Loader() {

	std::filesystem::path folderpath = compressed_texture_path.c_str();

	// Reads through all the files in the folder, and loads them into the mesh
	for (const auto& entry : std::filesystem::directory_iterator(folderpath))
	{
		std::cout << "============================================\n";
		std::cout << "[NOTE]>> Loading Compressed Texture: \t" << entry.path().filename() << "\n";

		std::string filepath = compressed_texture_path + entry.path().filename().string();
		std::string materialinstancepath = filepath;

		std::cout << " i neeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeed texture " << filepath << " \n";

		uid uids(materialinstancepath);
		auto texPtr = SetupMaterialInstance(materialinstancepath);
		++mResouceCnt;
		instance_infos& tempInstance = AllocRscInfo();
		tempInstance.m_Name = materialinstancepath;
		tempInstance.m_GUID = uids;
		tempInstance.m_pData = reinterpret_cast<void*>(texPtr);

		tempInstance.m_Type = _TEXTURE;
		m_ResourceInstance.emplace(uids.id, &tempInstance);
	}
}
/***************************************************************************/
/*!
\brief
	Load texture necessary for editor to run
*/
/**************************************************************************/
void ResourceTy::MaterialEditor_Loader() {


	std::filesystem::path folderpath = compressed_Editor_path.c_str();

	// Reads through all the files in the folder, and loads them into the mesh
	for (const auto& entry : std::filesystem::directory_iterator(folderpath))
	{
		std::cout << "============================================\n";
		std::cout << "[NOTE]>> Loading Compressed Texture: \t" << entry.path().filename() << "\n";

		std::string filepath = compressed_Editor_path + entry.path().filename().string();
		std::string materialinstancepath = filepath;

		std::string saved;
		
		saved = entry.path().filename().string().substr(0,entry.path().filename().string().find_last_of("."));
		

		auto texPtr = SetupEditorlInstance(materialinstancepath);
		m_EditorTextures.emplace(saved, texPtr);




	}
}
/***************************************************************************/
/*!
\brief
	Setup texture for editor
*/
/**************************************************************************/
GFX::Texture* ResourceTy::SetupEditorlInstance(std::string filepath) {
	GFX::Texture localMaterialInstance;
	localMaterialInstance.Load(filepath.c_str());


	auto Texret = std::make_unique<GFX::Texture>(localMaterialInstance);
	return Texret.release();
}
/***************************************************************************/
/*!
\brief
	Setup general texture used
*/
/**************************************************************************/
GFX::Texture* ResourceTy::SetupMaterialInstance(std::string filepath) {
	GFX::Texture localMaterialInstance;
	localMaterialInstance.Load(filepath.c_str());


	auto Texret = std::make_unique<GFX::Texture>(localMaterialInstance);
	return Texret.release();
}

/***************************************************************************/
/*!
\brief
	Return material instance pointer
*/
/**************************************************************************/
GFX::Texture* ResourceTy::getMaterialInstance(unsigned id) {
	return reinterpret_cast<GFX::Texture*>(m_ResourceInstance[id]->m_pData);
}




/***************************************************************************/
/*!
\brief
	Return material instance pointer
*/
/**************************************************************************/
void ResourceTy::shader_Loader() {
	std::map<std::string,std::pair<std::string, std::string>> shaderpaths;
	shaderpaths.emplace("DefaultShader",std::pair<std::string, std::string>{ "../assets/shader_files/draw_vert.glsl", "../assets/shader_files/draw_frag.glsl" });
	shaderpaths.emplace("PointLightShader", std::pair<std::string, std::string>{ "../assets/shader_files/pointLight_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" });
	shaderpaths.emplace("AnimationShader", std::pair<std::string, std::string>{ "../assets/shader_files/animations_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" });
	shaderpaths.emplace("UIShader", std::pair<std::string, std::string>{ "../assets/shader_files/UI_vert.glsl", "../assets/shader_files/UI_frag.glsl" });


	// load all the shaders
	for (const auto& x : shaderpaths)
	{
		std::string vertPath = x.second.first;
		std::string fragPath = x.second.second;
		std::string combinedPath = vertPath + fragPath;

		uid uids(x.first);
		GFX::Shader localshader;
		localshader.CreateShaderFromFiles(vertPath.c_str(), fragPath.c_str());
		auto shaderRef = std::make_unique<GFX::Shader>(localshader);
		++mResouceCnt;
		instance_infos& tempInstance = AllocRscInfo();
		tempInstance.m_Name = x.first;
		tempInstance.m_GUID = uids;
		tempInstance.m_pData = reinterpret_cast<void*>(shaderRef.release());

		tempInstance.m_Type = _SHADER;
		m_ResourceInstance.emplace(uids.id, &tempInstance);

	}
}

/***************************************************************************/
/*!
\brief
	Return material instance pointer
*/
/**************************************************************************/
GFX::Shader* ResourceTy::get_Shader(unsigned id) {
	return reinterpret_cast<GFX::Shader*>(m_ResourceInstance[id]->m_pData);
}