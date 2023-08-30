#include "Mesh.hpp"
#include <filesystem>

// Follows the format in the shader code
/******************************************
	location = 0 --> Pos;
	location = 1 --> VertexColor;
	location = 2 --> LTW;
******************************************/

void GFX::Mesh::LoadFromGeom(const _GEOM::Geom& GeomData, std::vector<vec3>& positions, std::vector<glm::vec2>& uvs, std::vector<unsigned int>& indices)
{
	//for (size_t iSM{}; iSM < GeomData.m_nSubMeshes; ++iSM)
	{
		// Load indices
		//for (size_t iIndex{}, ui{ GeomData.m_pSubMesh[iSM].m_iIndices }; iIndex < GeomData.m_pSubMesh[iSM].m_nIndices; ++iIndex, ++ui)
		for (size_t iIndex{}; iIndex < GeomData.m_nIndices; ++iIndex)
		{
			indices.emplace_back(GeomData.m_pIndices[iIndex]);
		}

		// Load positions
		//for (size_t iPos{}, up{ GeomData.m_pSubMesh[iSM].m_iVertices }; iPos < GeomData.m_pSubMesh[iSM].m_nVertices; ++iPos, ++up)
		for (size_t iPos{}; iPos < GeomData.m_nVertices; ++iPos)
		{
			//positions.emplace_back(vec3(GeomData.m_pPos[up].m_Pos.x, GeomData.m_pPos[up].m_Pos.y, GeomData.m_pPos[up].m_Pos.z));
			positions.emplace_back(vec3(GeomData.m_pPos[iPos].m_Pos.x, GeomData.m_pPos[iPos].m_Pos.y, GeomData.m_pPos[iPos].m_Pos.z));
			uvs.emplace_back(vec2(GeomData.m_pPos[iPos].m_UV.x, GeomData.m_pPos[iPos].m_UV.y));
		}

		// Load UVs
		//for (size_t iUV{}, uuv{ GeomData.m_pSubMesh[iSM].m_iVertices }; iUV < GeomData.m_pSubMesh[iSM].m_nVertices; ++iUV, ++uuv)
		{
			//uvs.emplace_back(vec2(GeomData.m_pPos[uuv].m_UV.x, GeomData.m_pPos[uuv].m_UV.y));
		}
	}
}


void GFX::Mesh::Setup(std::vector<vec3> const& positions, std::vector<unsigned int> const& indices, std::vector<vec2> const& TexCoords)
{
	// Create VAO
	mVao.Create();

	/////////////////////////////////////////
	// POSITIONS
	/////////////////////////////////////////
	// Create VBO for mesh model
	mVbo.Create(positions.size() * sizeof(vec3));

	// Attach VBO for positions to VAO
	mVao.AddAttribute(0, 0, 3, GL_FLOAT);											// location 0, binding vao index 0
	mVbo.AttachData(0, positions.size() * sizeof(vec3), positions.data());			// Attach mesh data to VBO
	mVao.AttachVerterBuffer(mVbo.GetID(), 0, 0, sizeof(vec3));						// Attach to index 0

	/////////////////////////////////////////
	// COLORS
	/////////////////////////////////////////
	// Create VBO for Color data
	mColorVbo.Create(sizeof(vec4) * MAX_INSTANCES);

	// Attach Color VBO and divisor to VAO
	mVao.AddAttribute(1, 1, 4, GL_FLOAT);											// location 1, binding vao index 1
	mVao.AddAttributeDivisor(1, 1);													// divisor at vao index 1
	mVao.AttachVerterBuffer(mColorVbo.GetID(), 1, 0, sizeof(vec4));					// Attach to index 1

	/////////////////////////////////////////
	// TEXTURE COORDINATES
	/////////////////////////////////////////
	// Create VBO for Tex Coordinates data
	mTexCoordVbo.Create(sizeof(vec2) * TexCoords.size());

	// Attach TexCoord VBO to VAO
	mVao.AddAttribute(2, 2, 2, GL_FLOAT);											// location 2, binding vao index 2
	mTexCoordVbo.AttachData(0, TexCoords.size() * sizeof(vec2), TexCoords.data());	// Attach mesh data to VBO
	mVao.AttachVerterBuffer(mTexCoordVbo.GetID(), 2, 0, sizeof(vec2));				// Attach to index 2

	// Create local-to-world VBO
	mLTWVbo.Create(sizeof(mat4) * MAX_INSTANCES);

	// Add attributes and divisor as vec4
	for (int i = 0; i < 4; ++i)		
	{
		mVao.AddAttribute(3 + i, 3, 4, GL_FLOAT, sizeof(vec4) * i);					// location 3, binding vao index 3
		mVao.AddAttributeDivisor(3, 1);												// divisor at vao index 3
	}
	// Attach LTW VBO to VAO
	mVao.AttachVerterBuffer(mLTWVbo.GetID(), 3, 0, sizeof(vec4) * 4);

	// Element Buffer Object
	mEbo.Create(indices.size() * sizeof(GLuint));
	mEbo.AttachData(0, indices.size() * sizeof(GLuint), indices.data());
	glVertexArrayElementBuffer(mVao.GetID(), mEbo.GetID());

	mVao.Unbind();

	// Store mesh stats
	mVertexCount = static_cast<int>(positions.size());
	mIndexCount = static_cast<int>(indices.size());
}

void GFX::Mesh::ClearInstances()
{
	mLTW.clear();
	mColors.clear();
}

void GFX::Mesh::BindVao()
{
	mVao.Bind();
}

void GFX::Mesh::UnbindVao()
{
	mVao.Unbind();
}

void GFX::Mesh::PrepForDraw()
{
	// Attach data to vbo
	mColorVbo.AttachData(0, mColors.size() * sizeof(vec4), mColors.data());
	mLTWVbo.AttachData(0, mLTW.size() * sizeof(mat4), mLTW.data());
}

void GFX::Mesh::Destroy()
{
	mVao.Destroy();
	mVbo.Destroy();
	mEbo.Destroy();
	mLTWVbo.Destroy();
	mColorVbo.Destroy();
}

void GFX::MeshManager::Init()
{
	std::filesystem::path folderpath = compiled_geom_path.c_str();

	for (const auto& entry : std::filesystem::directory_iterator(folderpath))
	{
		if (std::filesystem::is_regular_file(entry))
		{
			std::cout << "Loading file: " << entry.path().filename() << "\n";

			_GEOM::Geom GeomData;
			Mesh localmesh;
			std::vector<glm::vec3> positions;
			std::vector<glm::vec2> uvs;
			std::vector<unsigned int> indices;

			std::string filepath = compiled_geom_path + entry.path().filename().string();
			Deserialization::DeserializeGeom(filepath.c_str(), GeomData);
			localmesh.LoadFromGeom(GeomData, positions, uvs, indices);
			localmesh.Setup(positions, indices, uvs);

			mSceneMeshes.emplace_back(localmesh);
		}
	}
}


namespace Deserialization
{
	bool DeserializeGeom(const std::string Filepath, _GEOM::Geom& GeomData) noexcept
	{
#if 0
		//std::ifstream infile("../compiled_geom/Skull_textured.geom");
		std::ifstream infile(Filepath.c_str());
		assert(infile.is_open());

		infile.read((char*)&GeomData.m_nMeshes, sizeof(GeomData.m_nMeshes));				// m_nMeshes
		infile.read((char*)&GeomData.m_nSubMeshes, sizeof(GeomData.m_nSubMeshes));			// m_nSubMeshes
		infile.read((char*)&GeomData.m_nVertices, sizeof(GeomData.m_nVertices));			// m_nVertices
		infile.read((char*)&GeomData.m_nExtras, sizeof(GeomData.m_nExtras));				// m_nExtras
		infile.read((char*)&GeomData.m_nIndices, sizeof(GeomData.m_nIndices));				// m_nIndices

		GeomData.m_pPos = std::make_shared<_GEOM::Geom::VertexPos[]>(GeomData.m_nVertices);
		for (uint32_t i{}; i < GeomData.m_nVertices; ++i) 
		{
			infile.read((char*)&GeomData.m_pPos[i].m_UV, sizeof(glm::vec2));				// UV Coords
			//infile.read((char*)&GeomData.m_pPos[i].m_Pos, sizeof(glm::vec3));				// Vtx Position

			std::cout << GeomData.m_pPos[i].m_UV.x << ", " << GeomData.m_pPos[i].m_UV.y << "\n";
			//std::cout << GeomData.m_pPos[i].m_Pos.x << ", " << GeomData.m_pPos[i].m_Pos.y << ", " << GeomData.m_pPos[i].m_Pos.z << "\n";
		}

		GeomData.m_pIndices = std::make_shared<uint32_t[]>(GeomData.m_nIndices);
		for (uint32_t i{}; i < GeomData.m_nIndices; ++i) {
			uint32_t idx{};
			infile.read((char*)&idx, sizeof(std::uint32_t));					// m_Indices
			std::cout << idx << "\n";
		}

		//assert(infile.good());
		infile.close();
		return true;
#else
		std::ifstream infile(Filepath.c_str());
		assert(infile.is_open());

		Serialization::ReadUnsigned(infile, GeomData.m_nMeshes);
		Serialization::ReadUnsigned(infile, GeomData.m_nSubMeshes);
		Serialization::ReadUnsigned(infile, GeomData.m_nVertices);
		Serialization::ReadUnsigned(infile, GeomData.m_nExtras);
		Serialization::ReadUnsigned(infile, GeomData.m_nIndices);

		Serialization::ReadMesh(infile, GeomData);
		Serialization::ReadSubMesh(infile, GeomData);
		Serialization::ReadVertexPos(infile, GeomData);
		//Serialization::ReadVertexExtra(infile, GeomData);		// == We dont need these data for now ==	
		Serialization::ReadIndices(infile, GeomData);

		assert(infile.good());
		infile.close();
		return true;
#endif
	}



#if 0
	// ====================================================================================================
	//										NEW DESERIALIZATION CODE BLOCK
	// ====================================================================================================
	bool ReadIndices(std::ifstream & inFile, _GEOM::Geom & GeomData) noexcept
	{
		ReadUnsigned(inFile, GeomData.m_nIndices);
		std::unique_ptr<std::uint32_t[]> indices = std::make_unique<std::uint32_t[]>(GeomData.m_nIndices);

		std::string IndexStr;
		std::getline(inFile, IndexStr);
		std::stringstream Stream(IndexStr);

		for (unsigned i{}; i < GeomData.m_nIndices; ++i)
		{
			Stream >> indices[i];
		}

		GeomData.m_pIndices = std::move(indices);
		return true;
	}


	bool ReadVertexExtra(std::ifstream & inFile, _GEOM::Geom & GeomData) noexcept
	{
		ReadUnsigned(inFile, GeomData.m_nExtras);
		std::unique_ptr<_GEOM::Geom::VertexExtra[]> extras = std::make_unique<_GEOM::Geom::VertexExtra[]>(GeomData.m_nExtras);

		for (unsigned i{}; i < GeomData.m_nExtras; ++i)
		{
			std::string VertexExtraStr;
			std::getline(inFile, VertexExtraStr);
			std::stringstream Stream(VertexExtraStr);

			Stream >> extras[i].m_Packed;
			Stream >> extras[i].m_U;
			Stream >> extras[i].m_V;
		}

		GeomData.m_pExtras = std::move(extras);
		return true;
	}


	bool ReadVertexPos(std::ifstream & inFile, _GEOM::Geom & GeomData) noexcept
	{
		ReadUnsigned(inFile, GeomData.m_nVertices);
		std::unique_ptr<_GEOM::Geom::VertexPos[]> pos = std::make_unique<_GEOM::Geom::VertexPos[]>(GeomData.m_nVertices);

		for (unsigned i{}; i < GeomData.m_nVertices; ++i)
		{
			std::string VertexPosStr;
			std::getline(inFile, VertexPosStr);
			std::stringstream Stream(VertexPosStr);

			Stream >> pos[i].pos[0];
			Stream >> pos[i].pos[1];
			Stream >> pos[i].pos[2];
			//Stream >> pos[i].m_QPosition_QNormalX;
		}

		GeomData.m_pPos = std::move(pos);
		return true;
	}


	bool ReadSubMesh(std::ifstream & inFile, _GEOM::Geom & GeomData) noexcept
	{
		ReadUnsigned(inFile, GeomData.m_nSubMeshes);
		std::unique_ptr<_GEOM::Geom::SubMesh[]> subMesh = std::make_unique<_GEOM::Geom::SubMesh[]>(GeomData.m_nSubMeshes);

		char ch;
		for (unsigned i{}; i < GeomData.m_nSubMeshes; ++i)
		{
			std::string SubMeshStr;
			std::getline(inFile, SubMeshStr);
			std::stringstream Stream(SubMeshStr);

			Stream >> subMesh[i].m_nFaces;
			Stream >> subMesh[i].m_iIndices;
			Stream >> subMesh[i].m_nIndices;
			Stream >> subMesh[i].m_iVertices;
			Stream >> subMesh[i].m_nVertices;
			Stream >> subMesh[i].m_iMaterial;

			Stream >> ch >> subMesh[i].m_PosCompressionOffset.x >> ch >> subMesh[i].m_PosCompressionOffset.y >> ch >> subMesh[i].m_PosCompressionOffset.z >> ch;
			Stream >> ch >> subMesh[i].m_UVCompressionOffset.x >> ch >> subMesh[i].m_UVCompressionOffset.y >> ch;
		}

		GeomData.m_pSubMesh = std::move(subMesh);
		return true;
	}


	bool ReadMesh(std::ifstream & inFile, _GEOM::Geom & GeomData) noexcept
	{
		ReadUnsigned(inFile, GeomData.m_nMeshes);
		std::unique_ptr<_GEOM::Geom::Mesh[]> uMesh = std::make_unique<_GEOM::Geom::Mesh[]>(GeomData.m_nMeshes);

		std::string MeshStr;
		std::getline(inFile, MeshStr);
		std::istringstream Stream(MeshStr);

		for (unsigned i{}; i < GeomData.m_nMeshes; ++i)
		{
			std::string NameBuffer;
			Stream >> NameBuffer;

			// if the next line starts with another serialization, means that this mesh has no name to deserialize
			if (NameBuffer == "DEFAULT,")
				continue;

			for (unsigned j{}; j < NameBuffer.size(); ++j)
			{
				uMesh[i].m_name[j] = NameBuffer[j];
			}
		}

		GeomData.m_pMesh = std::move(uMesh);
		return true;
	}


	bool ReadUnsigned(std::ifstream & inFile, std::uint32_t & value) noexcept
	{
		std::string line;
		std::getline(inFile, line);

		size_t colonPos = line.find(':');
		assert(colonPos != std::string::npos);

		std::string unsignedStr = line.substr(colonPos + 1);
		std::istringstream(unsignedStr) >> value;

		if (unsignedStr.empty() && unsignedStr.substr(1).empty()) {
			std::cout << "[ERROR]>> Failed to Read unsigned value\n";
			return false;
		}

		return true;
	}


	bool ReadSigned(std::ifstream & inFile, std::int16_t & value) noexcept
	{
		std::string line;
		std::getline(inFile, line);

		size_t colonPos = line.find(':');
		assert(colonPos != std::string::npos);

		std::string signedStr = line.substr(colonPos + 1);
		std::istringstream(signedStr) >> value;

		if (signedStr.empty() && signedStr.substr(1).empty()) {
			std::cout << "[ERROR]>> Failed to Read unsigned value\n";
			return false;
		}

		return true;
	}


	bool ReadVec3WithHeader(std::ifstream & inFile, glm::vec3 & value) noexcept
	{
		std::string line;
		char ch;
		std::getline(inFile, line);

		size_t colonPos = line.find(':');
		assert(colonPos != std::string::npos);

		std::string VecStr = line.substr(colonPos + 1);
		std::istringstream Stream(VecStr);

		for (int i{}; i < 3; ++i) {
			Stream >> ch;
			Stream >> value[i];
		}

		return true;	// no errors
	}


	bool ReadVec2WithHeader(std::ifstream & inFile, glm::vec2 & value) noexcept
	{
		std::string line;
		char ch;
		std::getline(inFile, line);

		size_t colonPos = line.find(':');
		assert(colonPos != std::string::npos);

		std::string VecStr = line.substr(colonPos + 1);
		std::istringstream Stream(VecStr);

		for (int i{}; i < 2; ++i) {
			Stream >> ch;
			Stream >> value[i];
		}

		return true;	// no errors
	}
#endif
}

#if _ASSIMP_LOADING
namespace AssimpImporter
{
	void processnode(aiNode* node, const aiScene* scene) 
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			GFX::Mesh::assimpLoadedMeshes.push_back(processmesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processnode(node->mChildren[i], scene);
		}
	};

	GFX::Mesh processmesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<glm::vec3> positions;
		std::vector<unsigned int> indices;

		for (unsigned int i{}; i < mesh->mNumVertices; ++i)
		{
			positions.emplace_back( glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z) );
		}

		for (unsigned int i{}; i < mesh->mNumFaces; ++i)
		{
			const aiFace& face = mesh->mFaces[i];
			for (unsigned int j{}; j < face.mNumIndices; ++j)
			{
				indices.emplace_back(face.mIndices[j]);
			}
		}

		// material stuff, we leave it empty for now.
		//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		GFX::Mesh retmesh;
		retmesh.mPositions = std::move(positions);
		retmesh.mIndices = std::move(indices);
		return retmesh;
	}

	void loadModel(const std::string& filepath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile("../assets/demon-skull-textured/source/Skull_textured.fbx", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			assert(false);
		}

		// process ASSIMP's root node recursively
		processnode(scene->mRootNode, scene);
	}
}
#endif