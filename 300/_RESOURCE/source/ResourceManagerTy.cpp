#include "ResourceManagerTy.h"
#include <map>
#include <memory>

#include <descriptor.h>
#include <Guid.h>

#include <iostream>
#include <fstream>

#include "filereadstream.h"
#include "filewritestream.h"
#include <document.h>
#include <writer.h>
#include "prettywriter.h"
#include <stringbuffer.h>

#include <filesystem>

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

	for (auto& data : m_Shaders) {
		delete reinterpret_cast<GFX::Shader*>(data.second.second.data);

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
			meshPtr->mMeshDescriptorPath = descfilepath;
			_GEOM::DescriptorData::DeserializeGEOM_DescriptorDataFromFile(meshPtr->mMeshDescriptorData, descfilepath);

			instance_infos& meshInstance = AllocRscInfo();
			meshInstance.m_pData = reinterpret_cast<void*>(meshPtr);
			meshInstance.m_Name = filepath;
			meshInstance.m_GUID = guid;
			meshInstance.m_Type = _MESH;

			m_ResourceInstance.emplace(guid, &meshInstance);
		}
	}
}

/***************************************************************************/
/*!
\brief
	Return mesh pointer
*/
/**************************************************************************/
GFX::Mesh* ResourceTy::get_mesh(unsigned id) 
{
	if (m_ResourceInstance.find(id) == m_ResourceInstance.end())
		return nullptr;

	return reinterpret_cast<GFX::Mesh*>(m_ResourceInstance[id]->m_pData);

}


/***************************************************************************/
/*!
\brief
	Mesh collider. returns by reference the vertices and indices of the mesh
*/
/**************************************************************************/
void ResourceTy::mesh_GetVerticesAndIndices(std::string filepath, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices)
{
	_GEOM::Geom GeomData;

	Deserialization::DeserializeGeom(filepath.c_str(), GeomData);	// load the geom from the compiled geom file
	
	GFX::Mesh::LoadFromGeom(GeomData, vertices, indices);

	float minx, maxx, miny, maxy, minz, maxz;
	minx = miny = minz = INFINITY;
	maxx = maxy = maxz = -INFINITY;
	for (const glm::vec3& vtx : vertices)
	{
		if (vtx.x < minx)
			minx = vtx.x;
		if (vtx.x > maxx)
			maxx = vtx.x;
		if (vtx.y < miny)
			miny = vtx.y;
		if (vtx.y > maxy)
			maxy = vtx.y;
		if (vtx.z < minz)
			minz = vtx.z;
		if (vtx.z > maxz)
			maxz = vtx.z;
	}
	for (glm::vec3& vtx : vertices)
	{
		vtx.x /= (maxx - minx);
		vtx.y /= (maxy - miny);
		vtx.z /= (maxz - minz);
	}
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

	auto getFilename = [](std::string filepath) -> std::string
	{
		// returns AT-AT
		std::string ret_str = filepath.substr(filepath.find_last_of("/") + 1);
		ret_str = ret_str.substr(0, ret_str.find_first_of("."));
		return ret_str;
	};

	std::filesystem::path folderpath = compressed_texture_path.c_str();

	// Reads through all the files in the folder, and loads them into the mesh
	for (const auto& entry : std::filesystem::directory_iterator(folderpath))
	{

		if (check_extensions(entry.path().filename().string(), ".desc"))
			continue;

		std::cout << "============================================\n";
		std::cout << "[NOTE]>> Loading Compressed Texture: \t" << entry.path().filename() << "\n";


		std::string compressedfilepath = compressed_texture_path + getFilename(entry.path().filename().string()) + ".ctexture";
		std::string uncompressedfilepath = uncompressed_texture_path + entry.path().filename().string();
		std::string descfilepath;

		bool descFilePresent = _GEOM::CheckAndCreateDescriptorFileTEXTURE(uncompressedfilepath, descfilepath, compressedfilepath);
		unsigned guid = _GEOM::GetGUID(descfilepath);

		auto texPtr = SetupMaterialInstance(compressedfilepath);

		++mResouceCnt;
		instance_infos& tempInstance = AllocRscInfo();
		tempInstance.m_Name = compressedfilepath;
		tempInstance.m_GUID = guid;
		tempInstance.m_pData = reinterpret_cast<void*>(texPtr);

		tempInstance.m_Type = _TEXTURE;
		m_ResourceInstance.emplace(guid, &tempInstance);
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


void ResourceTy::texture_Load(std::string texturename, unsigned uid)
{
	std::cout << "============================================\n";
	std::cout << "[NOTE]>> Loading Compressed Texture: \t" << texturename << "\n";

	std::string filepath = compressed_texture_path + texturename + ".ctexture";
	std::string materialinstancepath = filepath;

	auto texPtr = SetupMaterialInstance(materialinstancepath);
	++mResouceCnt;
	instance_infos& tempInstance = AllocRscInfo();
	tempInstance.m_Name = materialinstancepath;
	tempInstance.m_GUID = uid;
	tempInstance.m_pData = reinterpret_cast<void*>(texPtr);

	tempInstance.m_Type = _TEXTURE;
	m_ResourceInstance.emplace(uid, &tempInstance);
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
GFX::Texture* ResourceTy::getMaterialInstance(unsigned id) 
{
	if (m_ResourceInstance.find(id) == m_ResourceInstance.end())
		return nullptr;

	return reinterpret_cast<GFX::Texture*>(m_ResourceInstance[id]->m_pData);
}




/***************************************************************************/
/*!
\brief
	Return material instance pointer
*/
/**************************************************************************/
void ResourceTy::shader_Loader() {
	//std::map<std::string,std::pair<std::string, std::string>> shaderpaths;
	//shaderpaths.emplace("DefaultShader",std::pair<std::string, std::string>{ "../assets/shader_files/draw_vert.glsl", "../assets/shader_files/draw_frag.glsl" });
	//shaderpaths.emplace("PointLightShader", std::pair<std::string, std::string>{ "../assets/shader_files/pointLight_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" });
	//shaderpaths.emplace("AnimationShader", std::pair<std::string, std::string>{ "../assets/shader_files/animations_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" });
	//shaderpaths.emplace("UIShader", std::pair<std::string, std::string>{ "../assets/shader_files/UI_vert.glsl", "../assets/shader_files/UI_frag.glsl" });
	//shaderpaths.emplace("GaussianBlurShader", std::pair<std::string, std::string>{ "../assets/shader_files/Quad2D_vert.glsl", "../assets/shader_files/GaussianBlur_frag.glsl" });
	//shaderpaths.emplace("AdditiveBlendShader", std::pair<std::string, std::string>{ "../assets/shader_files/Quad2D_vert.glsl", "../assets/shader_files/AdditiveBlending_frag.glsl" });

	// load all the shaders
	for (const auto& entry : std::filesystem::directory_iterator(shader_program_path))
	{
	//	serialize_Shader(x.first, x.second);
		//std::string shaderDe = "../assets/ShaderProgram/" + x.first;
		std::pair<std::string, std::pair<std::string, std::string>> shaderData;
		try {
			shaderData = deserialize_Shader(entry.path().string());

		}
		catch (std::exception e) {
			std::cout << e.what();
		}


		uid uids(shaderData.first);
		GFX::Shader localshader;
		localshader.CreateShaderFromFiles(shaderData.second.first.c_str(), shaderData.second.second.c_str());
		auto shaderRef = std::make_unique<GFX::Shader>(localshader);
		//++mResouceCnt;
		//instance_infos& tempInstance = AllocRscInfo();
		//tempInstance.m_Name = x.first;
		//tempInstance.m_GUID = uids;
		//tempInstance.m_pData = reinterpret_cast<void*>(shaderRef.release());

		//tempInstance.m_Type = _SHADER;
		//m_ResourceInstance.emplace(uids.id, &tempInstance);


		ref data{};
		data.data_uid = uids;
		data.data = reinterpret_cast<void*>(shaderRef.release());
		m_Shaders.emplace(uids.id, std::pair(shaderData.first, data));


	}
}

/***************************************************************************/
/*!
\brief
	Return material instance pointer
*/
/**************************************************************************/
GFX::Shader* ResourceTy::get_Shader(unsigned id) {
	//return reinterpret_cast<GFX::Shader*>(m_ResourceInstance[id]->m_pData);

	return reinterpret_cast<GFX::Shader*>(m_Shaders[id].second.data);
}


void ResourceTy::create_Shader(std::string ShaderPrgm,std::string vertPath, std::string fragPath) {
	GFX::Shader localshader;
	localshader.CreateShaderFromFiles(vertPath.c_str(), fragPath.c_str());
	auto shaderRef = std::make_unique<GFX::Shader>(localshader);

	uid uids(ShaderPrgm);
	ref data{};
	data.data_uid = uids;
	data.data = reinterpret_cast<void*>(shaderRef.release());
	m_Shaders.emplace(uids.id, std::pair(ShaderPrgm, data));

	serialize_Shader(ShaderPrgm, std::pair(vertPath,fragPath));
}


bool ResourceTy::serialize_Shader(std::string shaderProgram, std::pair < std::string,std::string> shaderPair) {

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	const char* programPathChar = "../assets/ShaderProgram/";
	std::string programPath(programPathChar);
	programPath += shaderProgram;
	programPath += ".shdr";

	// Serialize to a file
	std::ofstream file(programPath.c_str());
	if (file.is_open())
	{
		std::cout << "JSON data serialized to " << programPath << std::endl;
	}
	else {
		std::cerr << "Failed to open the file for writing." << std::endl;
		return false;
	}


	rapidjson::StringBuffer ss;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(ss);

	writer.StartArray();
	writer.StartObject();

	writer.String("ShaderProgram");
	writer.String(shaderProgram.c_str());

	writer.String("Vertex");
	writer.String(shaderPair.first.c_str());

	writer.String("Fragment");
	writer.String(shaderPair.second.c_str());
	writer.EndObject();
	writer.EndArray();

	file << ss.GetString();

	file.close();

	return true;
}

std::pair<std::string, std::pair<std::string, std::string>> ResourceTy::deserialize_Shader(std::string filename) {

	std::pair<std::string, std::pair<std::string, std::string>> data;

	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Failed to DeserializeFile " + filename << '\n';
		//return false;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	rapidjson::Document doc;


	if (buffer.str().empty())
	{
		throw std::exception("Buffer is empty!\n");
		//return false;
	}

	std::string validJSON(buffer.str());
	if (doc.Parse(validJSON.c_str()).HasParseError()) {
		//return false;

		throw std::exception("Parsing Error!\n");
	}

	for (rapidjson::Value::ConstValueIterator ci = doc.Begin(); ci != doc.End(); ++ci)
	{

		if (ci->HasMember("ShaderProgram"))
		{
			data.first = (*ci)["ShaderProgram"].GetString();
		}
		if (ci->HasMember("Vertex"))
		{
			data.second.first = (*ci)["Vertex"].GetString();
		}

		if (ci->HasMember("Fragment"))
		{
			data.second.second = (*ci)["Fragment"].GetString();
		}
	}


	 return data;
}






