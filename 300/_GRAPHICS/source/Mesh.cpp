#include "Mesh.hpp"
#include <filesystem>
#include <array>

// Follows the format in the shader code
/******************************************
	location = 0 --> Pos;
	location = 1 --> VertexColor;
	location = 2 --> LTW;
******************************************/

void GFX::Mesh::LoadFromGeom(const _GEOM::Geom& GeomData, std::vector<vec3>& positions, std::vector<glm::vec2>& uvs, std::vector<unsigned int>& indices)
{
	const int reservenumber = GeomData.m_nVertices;
	indices.reserve(reservenumber);
	positions.reserve(reservenumber);
	uvs.reserve(reservenumber);

	for (size_t iSM{}; iSM < GeomData.m_nSubMeshes; ++iSM)
	{
		// Load indices
		for (size_t iIndex{}, ui{ GeomData.m_pSubMesh[iSM].m_iIndices }; iIndex < GeomData.m_pSubMesh[iSM].m_nIndices; ++iIndex, ++ui)
		{
			indices.emplace_back(GeomData.m_pIndices[iIndex]);
		}

		// Load positions and uvs
		for (size_t iIndex{}, ui{ GeomData.m_pSubMesh[iSM].m_iVertices }; iIndex < GeomData.m_pSubMesh[iSM].m_nVertices; ++iIndex, ++ui)
		{
			positions.emplace_back(vec3(GeomData.m_pPos[iIndex].m_Pos.x, GeomData.m_pPos[iIndex].m_Pos.y, GeomData.m_pPos[iIndex].m_Pos.z));
			uvs.emplace_back(vec2(GeomData.m_pPos[iIndex].m_UV.x, GeomData.m_pPos[iIndex].m_UV.y));
		}
	}
}


void GFX::Mesh::LoadAnimationDataFromGeom(const _GEOM::Geom& GeomData, std::vector<std::array<int,MAX_BONE_INFLUENCE>>& boneIDs, std::vector<std::array<float,MAX_BONE_INFLUENCE>>& boneWeights)
{
	const int reservenumber = GeomData.m_nVertices;
	boneIDs.reserve(reservenumber);
	boneWeights.reserve(reservenumber);

	for (size_t iSM{}; iSM < GeomData.m_nSubMeshes; ++iSM)
	{
		// Load bone weights, and bone IDs
		for (size_t iIndex{}, ui{ GeomData.m_pSubMesh[iSM].m_iVertices }; iIndex < GeomData.m_pSubMesh[iSM].m_nVertices; ++iIndex, ++ui)
		{
			std::array<int, MAX_BONE_INFLUENCE> lBoneIDs;
			std::array<float, MAX_BONE_INFLUENCE> lBoneWeights;
			const auto& localgeom = GeomData.m_pPos[iIndex];

			lBoneIDs = { localgeom.m_BoneIDs[0], localgeom.m_BoneIDs[1], localgeom.m_BoneIDs[2], localgeom.m_BoneIDs[3] };
			lBoneWeights = { localgeom.m_Weights[0], localgeom.m_Weights[1] , localgeom.m_Weights[2] , localgeom.m_Weights[3] };

			boneIDs.emplace_back(lBoneIDs);
			boneWeights.emplace_back(lBoneWeights);
		}
	}
}


void GFX::Mesh::Setup(const _GEOM::Geom& GeomData)
{
	std::vector<glm::vec3>								positions;
	std::vector<glm::vec2>								uvs;
	std::vector<unsigned int>							indices;
	std::vector<std::array<int,MAX_BONE_INFLUENCE>>		boneIDs;
	std::vector<std::array<float,MAX_BONE_INFLUENCE>>	boneWeights;

	LoadFromGeom(GeomData, positions, uvs, indices);

	if (GeomData.m_bHasAnimations)
	{
		LoadAnimationDataFromGeom(GeomData, boneIDs, boneWeights);
	}

	// Create VAO
	mVao.Create();

	/////////////////////////////////////////
	// POSITIONS
	// Create VBO for mesh model. Attach VBO for positions to VAO
	mVbo.Create(positions.size() * sizeof(vec3));
	mVao.AddAttribute(0, 0, 3, GL_FLOAT);											// location 0, binding vao index 0
	mVbo.AttachData(0, positions.size() * sizeof(vec3), positions.data());			// Attach mesh data to VBO
	mVao.AttachVertexBuffer(mVbo.GetID(), 0, 0, sizeof(vec3));						// Attach to index 0

	/////////////////////////////////////////
	// COLORS
	// Create VBO for Color data. Attach Color VBO and divisor to VAO		
	mColorVbo.Create(sizeof(vec4) * MAX_INSTANCES);
	mVao.AddAttribute(1, 1, 4, GL_FLOAT);											// location 1, binding vao index 1
	mVao.AddAttributeDivisor(1, 1);													// divisor at vao index 1
	mVao.AttachVertexBuffer(mColorVbo.GetID(), 1, 0, sizeof(vec4));					// Attach to index 1

	/////////////////////////////////////////
	// TEXTURE COORDINATES
	// Create VBO for Tex Coordinates data. Attach TexCoord VBO to VAO
	mTexCoordVbo.Create(sizeof(vec2) * uvs.size());
	mVao.AddAttribute(2, 2, 2, GL_FLOAT);											// location 2, binding vao index 2
	mTexCoordVbo.AttachData(0, uvs.size() * sizeof(vec2), uvs.data());				// Attach mesh data to VBO
	mVao.AttachVertexBuffer(mTexCoordVbo.GetID(), 2, 0, sizeof(vec2));				// Attach to index 2

	/////////////////////////////////////////
	// Local To World
	// Create local-to-world VBO
	mLTWVbo.Create(sizeof(mat4) * MAX_INSTANCES);

	// Add attributes and divisor as vec4
	for (int i = 0; i < 4; ++i)
	{
		mVao.AddAttribute(5 + i, 5, 4, GL_FLOAT, sizeof(vec4) * i);					// location 5, binding vao index 5
		mVao.AddAttributeDivisor(5, 1);												// divisor at vao index 5
	}
	// Attach LTW VBO to VAO
	mVao.AttachVertexBuffer(mLTWVbo.GetID(), 5, 0, sizeof(vec4) * 4);

	/////////////////////////////////////////
	// EBO
	mEbo.Create(indices.size() * sizeof(GLuint));
	mEbo.AttachData(0, indices.size() * sizeof(GLuint), indices.data());
	glVertexArrayElementBuffer(mVao.GetID(), mEbo.GetID());


	/////////////////////////////////////////
	// Bone_IDs, and Weights (TODO) thank you sergeant.
	// It is an array of structs. Stored the same way as the positions and uvs.
	// The data and weights are stored in this format below::
		//GeomData.m_pPos[0].m_BoneIDs;
		//GeomData.m_pPos[0].m_Weights;
		
	if (GeomData.m_bHasAnimations)
	{
		// Create VBO for Bone IDs. Attach VBO for Bone IDs to VAO
		mBoneIDVbo.Create(positions.size() * sizeof(int) * MAX_BONE_INFLUENCE);										// 1 for each vertex
		mVao.AddAttribute(3, 3, 4, GL_INT);																			// location 3, binding vao index 3
		// TODO: need to somehow get the data of all Bone IDs
		mBoneIDVbo.AttachData(0, boneIDs.size() * sizeof(int) * MAX_BONE_INFLUENCE, boneIDs.data());				// Attach mesh data to VBO
		mVao.AttachVertexBuffer(mBoneIDVbo.GetID(), 3, 0, sizeof(int) * MAX_BONE_INFLUENCE);						// Attach to index 3

		// Create VBO for Bone weights. Attach VBO for Bone weights to VAO
		mBoneWeightVbo.Create(positions.size() * sizeof(float) * MAX_BONE_INFLUENCE);								// 1 for each vertex
		mVao.AddAttribute(4, 4, 4, GL_FLOAT);																		// location 4, binding vao index 4
		// TODO: need to somehow get the data of all Bone IDs
		mBoneWeightVbo.AttachData(0, boneWeights.size() * sizeof(float) * MAX_BONE_INFLUENCE, boneWeights.data());	// Attach mesh data to VBO
		mVao.AttachVertexBuffer(mBoneWeightVbo.GetID(), 4, 0, sizeof(float) * MAX_BONE_INFLUENCE);					// Attach to index 4
	}

	mVao.Unbind();

	// Store mesh stats
	mVertexCount = static_cast<int>(positions.size());
	mIndexCount = static_cast<int>(indices.size());
}


// to remove when i'm done with animation.. keeping as reference for now
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
	mVao.AttachVertexBuffer(mVbo.GetID(), 0, 0, sizeof(vec3));						// Attach to index 0

	/////////////////////////////////////////
	// COLORS
	/////////////////////////////////////////
	// Create VBO for Color data
	mColorVbo.Create(sizeof(vec4) * MAX_INSTANCES);

	// Attach Color VBO and divisor to VAO
	mVao.AddAttribute(1, 1, 4, GL_FLOAT);											// location 1, binding vao index 1
	mVao.AddAttributeDivisor(1, 1);													// divisor at vao index 1
	mVao.AttachVertexBuffer(mColorVbo.GetID(), 1, 0, sizeof(vec4));					// Attach to index 1

	/////////////////////////////////////////
	// TEXTURE COORDINATES
	/////////////////////////////////////////
	// Create VBO for Tex Coordinates data
	mTexCoordVbo.Create(sizeof(vec2) * TexCoords.size());

	// Attach TexCoord VBO to VAO
	mVao.AddAttribute(2, 2, 2, GL_FLOAT);											// location 2, binding vao index 2
	mTexCoordVbo.AttachData(0, TexCoords.size() * sizeof(vec2), TexCoords.data());	// Attach mesh data to VBO
	mVao.AttachVertexBuffer(mTexCoordVbo.GetID(), 2, 0, sizeof(vec2));				// Attach to index 2

	// Create local-to-world VBO
	mLTWVbo.Create(sizeof(mat4) * MAX_INSTANCES);

	// Add attributes and divisor as vec4
	for (int i = 0; i < 4; ++i)		
	{
		mVao.AddAttribute(3 + i, 3, 4, GL_FLOAT, sizeof(vec4) * i);					// location 3, binding vao index 3
		mVao.AddAttributeDivisor(3, 1);												// divisor at vao index 3
	}
	// Attach LTW VBO to VAO
	mVao.AttachVertexBuffer(mLTWVbo.GetID(), 3, 0, sizeof(vec4) * 4);

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

void GFX::Mesh::DrawAllInstances()
{
	mVao.Bind();		// Bind mesh
	PrepForDraw();		// Copy data from local buffer into opengl buffer
	glDrawElementsInstanced(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, nullptr, mLTW.size());
	mVao.Unbind();		// Unbind mesh
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



//GFX::MeshData& GFX::MeshManager::AllocRscInfo()
//{
//	auto pTemp = m_pInfoBufferEmptyHead;
//
//
//	MeshData* pNext = reinterpret_cast<MeshData*>(m_pInfoBufferEmptyHead->m_pData);
//	m_pInfoBufferEmptyHead = pNext;
//
//	return *pTemp;
//}
//
//void GFX::MeshManager::ReleaseRscInfo(MeshData& RscInfo)
//{
//	// Add this resource info to the empty chain
//	RscInfo.m_pData = m_pInfoBufferEmptyHead;
//	m_pInfoBufferEmptyHead = &RscInfo;
//}


void GFX::MeshManager::Init()
{
	//for (int i = 0, end = (int)m_Meshbuffer.size() - 1; i != end; ++i)
	//{
	//	m_Meshbuffer[i].m_pData = &m_Meshbuffer[i + 1];
	//}
	//m_Meshbuffer[m_Meshbuffer.size() - 1].m_pData = nullptr;
	//m_pInfoBufferEmptyHead = m_Meshbuffer.data();


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
			SetupMesh(filepath);

			//uid uidd(entry.path().filename().string());
			//uid uids("gayed");
			//MeshData&  temp =AllocRscInfo();
			//temp.meshdata = std::move(localmesh);
			//mSceneMeshes.emplace(std::make_pair(uids.id, &temp));

		}
	}
}


void GFX::MeshManager::SetupMesh(std::string filepath)
{
	_GEOM::Geom GeomData;
	Mesh localmesh;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;

	Deserialization::DeserializeGeom(filepath.c_str(), GeomData);	// load the geom from the compiled geom file
	localmesh.Setup(GeomData);

	// Load animations
	if (GeomData.m_bHasAnimations)
	{
		// store the animation data of the first mesh -- there should only be be one mesh per file so far, so we just take the first index
		// We store the vector of animation data into the mesh class.
		localmesh.mAnimation = GeomData.m_pMesh[0].m_Animation;
		localmesh.mHasAnimation = true;
	}

	mSceneMeshes.emplace_back(localmesh);							// storage of all the scene's meshes
}


namespace Deserialization
{
	void DeserializeAssimpNodeData(std::ifstream& infile, _GEOM::AssimpNodeData& Node)
	{
		uint8_t strlen;
		char cname[128];

		infile.read((char*)&strlen, sizeof(uint8_t));		// read the length of the string
		infile.read(cname, strlen);							// read the string	
		Node.m_Name = std::string(cname, strlen);			// set the name
			   
		infile.read((char*)&Node.m_NumChildren, sizeof(int));				// number of children
		infile.read((char*)&Node.m_Transformation, sizeof(glm::mat4));		// local transformation matrix


		// go and deserialize the children all the way to the leaf
		for (int i{}; i < Node.m_NumChildren; ++i)
		{
			Node.m_Children.emplace_back(_GEOM::AssimpNodeData());
			DeserializeAssimpNodeData(infile, Node.m_Children[i]);
		}
	}


	bool DeserializeGeom(const std::string Filepath, _GEOM::Geom& GeomData) noexcept
	{
#if 1
		std::ifstream infile(Filepath.c_str(), std::ios::binary);
		assert(infile.is_open());
		assert(infile.good());

		// Input Values
		infile.read((char*)&GeomData.m_nMeshes, sizeof(GeomData.m_nMeshes));				// m_nMeshes
		infile.read((char*)&GeomData.m_nSubMeshes, sizeof(GeomData.m_nSubMeshes));			// m_nSubMeshes
		infile.read((char*)&GeomData.m_nVertices, sizeof(GeomData.m_nVertices));			// m_nVertices
		infile.read((char*)&GeomData.m_nExtras, sizeof(GeomData.m_nExtras));				// m_nExtras
		infile.read((char*)&GeomData.m_nIndices, sizeof(GeomData.m_nIndices));				// m_nIndices

		// Input Booleans
		infile.read((char*)&GeomData.m_bHasAnimations, sizeof(bool));						// bAnimations
		infile.read((char*)&GeomData.m_bHasTextures, sizeof(bool));							// bTextures
		infile.read((char*)&GeomData.m_bVtxClrPresent, sizeof(bool));						// bColors

		// VertexPos
		GeomData.m_pPos = std::make_shared<_GEOM::Geom::VertexPos[]>(GeomData.m_nVertices);
		for (uint32_t i{}; i < GeomData.m_nVertices; ++i) {									
			infile.read((char*)&GeomData.m_pPos[i], sizeof(_GEOM::Geom::VertexPos));		
		}

		// Indices
		GeomData.m_pIndices = std::make_shared<uint32_t[]>(GeomData.m_nIndices);
		for (uint32_t i{}; i < GeomData.m_nIndices; ++i) {									
			infile.read((char*)&GeomData.m_pIndices[i], sizeof(std::uint32_t));				
		}

		// Meshes
		GeomData.m_pMesh = std::make_shared<_GEOM::Geom::Mesh[]>(GeomData.m_nMeshes);
		for (uint32_t i{}; i < GeomData.m_nMeshes; ++i) 
		{				
			infile.read((char*)&GeomData.m_pMesh[i].m_name, sizeof(char) * 64);

			// Animation Data
			if (GeomData.m_bHasAnimations)
			{
				uint8_t numberofanimations{};
				infile.read((char*)&numberofanimations, sizeof(uint8_t));

				std::cout << "[MESH DESERIALIZATION]>> This mesh contains animations\n";
				std::cout << "[MESH DESERIALIZATION]>> Number of animations: " << (int)numberofanimations << "\n";

				for (int j{}; j < numberofanimations; ++j)
				{
					_GEOM::Animation animation;

					infile.read((char*)&animation.m_BoneCounter, sizeof(uint32_t));
					infile.read((char*)&animation.m_Duration, sizeof(float));
					infile.read((char*)&animation.m_TicksPerSecond, sizeof(float));

					// Bone info map
					for (unsigned j{}; j < animation.m_BoneCounter; ++j)
					{
						_GEOM::BoneInfo temp;
						char cname[64];
						uint8_t strlen{};

						infile.read((char*)&strlen, sizeof(uint8_t));						// Bone name length
						infile.read(cname, strlen);											// Bone name							
						infile.read((char*)&temp, sizeof(_GEOM::BoneInfo));					// Bone info	

						std::string name(cname, strlen);

						animation.m_BoneInfoMap.emplace(name, temp);
					}

					// Bones
					animation.m_Bones.resize(animation.m_BoneCounter);
					for (unsigned j{}; j < animation.m_BoneCounter; ++j)
					{
						auto& boneinst = animation.m_Bones[j];

						char cname[64];
						uint8_t strlen{};

						infile.read((char*)&strlen, sizeof(uint8_t));						// Bone name length
						infile.read(cname, strlen);											// Bone name
						boneinst.m_Name = std::string(cname, strlen);

						infile.read((char*)&boneinst.m_ID, sizeof(int));					// Bone ID

						infile.read((char*)&boneinst.m_NumPositions, sizeof(int));			// num positions
						infile.read((char*)&boneinst.m_NumRotations, sizeof(int));			// num rotations
						infile.read((char*)&boneinst.m_NumScalings, sizeof(int));			// num scalings

						boneinst.m_Positions.resize(boneinst.m_NumPositions);
						boneinst.m_Rotations.resize(boneinst.m_NumRotations);
						boneinst.m_Scales.resize(boneinst.m_NumScalings);

						// local transform
						infile.read((char*)&boneinst.m_LocalTransform, sizeof(glm::mat4));

						// key positions
						for (int k{}; k < boneinst.m_NumPositions; ++k) {
							infile.read((char*)&boneinst.m_Positions[k], sizeof(_GEOM::KeyPosition));
						}

						// key rotations
						for (int k{}; k < boneinst.m_NumRotations; ++k) {
							infile.read((char*)&boneinst.m_Rotations[k], sizeof(_GEOM::KeyRotation));
						}

						// key scalings
						for (int k{}; k < boneinst.m_NumScalings; ++k) {
							infile.read((char*)&boneinst.m_Scales[k], sizeof(_GEOM::KeyScale));
						}
					}

					// AssimpNodeData
					Deserialization::DeserializeAssimpNodeData(infile, animation.m_RootNode);

					GeomData.m_pMesh[i].m_Animation.emplace_back(animation);
				}
			
				std::cout << "[MESH DESERIALIZATION]>> Animation Deserialization completed\n";
			}

		}

		// Submeshes
		GeomData.m_pSubMesh = std::make_shared<_GEOM::Geom::SubMesh[]>(GeomData.m_nSubMeshes);
		for (uint32_t i{}; i < GeomData.m_nMeshes; ++i) {									
			infile.read((char*)&GeomData.m_pSubMesh[i], sizeof(_GEOM::Geom::SubMesh));
		}

		// Vertex Extras
		GeomData.m_pExtras = std::make_shared<_GEOM::Geom::VertexExtra[]>(GeomData.m_nExtras);
		for (uint32_t i{}; i < GeomData.m_nExtras; ++i) {									
			infile.read((char*)&GeomData.m_pExtras[i], sizeof(_GEOM::Geom::m_nExtras));
		}

		infile.close();
		return true;

#else	////////////////////////////////////////////////////////////////////////////////

		std::ifstream infile(Filepath.c_str());
		assert(infile.is_open());

		Serialization::ReadUnsigned(infile, GeomData.m_nMeshes);
		Serialization::ReadUnsigned(infile, GeomData.m_nSubMeshes);
		Serialization::ReadUnsigned(infile, GeomData.m_nVertices);
		Serialization::ReadUnsigned(infile, GeomData.m_nExtras);
		Serialization::ReadUnsigned(infile, GeomData.m_nIndices);

		Serialization::ReadMesh(infile, GeomData);
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

#if 0
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