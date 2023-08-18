#include <Geom_Compiler.h>
#include <meshoptimizer/src/meshoptimizer.h>

#include <span>
#include <limits>
#include <iostream>
#include <functional>
#include <fstream>

#pragma comment( lib, "../lib/assimp/BINARIES/Win32/lib/Release/assimp-vc142-mt.lib")

namespace _GEOM
{
	//----------------------------------------------------------------------------------------------------
	//					MESH LOADER: PUBLIC FUNCTIONS
	//----------------------------------------------------------------------------------------------------
	bool Mesh_Loader::Load(const DescriptorData& _DData, SkinGeom* _skinGeom) noexcept
	{
		std::cout << "\n\n====================================================================================================\n";
		std::cout << ">>[NOTE]: \tImporting Mesh:" << _DData.m_Filepaths[_DData.m_iMeshCurrent] << "\n";
		Assimp::Importer importer;

		uint32_t flag = aiProcess_Triangulate	   // Make sure we get triangles rather than nvert polygons
			| aiProcess_LimitBoneWeights           // 4 weights for skin model max
			| aiProcess_GenUVCoords                // Convert any type of mapping to uv mapping
			| aiProcess_TransformUVCoords          // preprocess UV transformations (scaling, translation ...)
			| aiProcess_FindInstances              // search for instanced meshes and remove them by references to one master
			| aiProcess_GenNormals                 // if it does not have normals generate them... (this may not be a good option as it may hide issues from artist)
			| aiProcess_CalcTangentSpace           // calculate tangents and bitangents if possible (definetly you will meed UVs)
			| aiProcess_RemoveRedundantMaterials   // remove redundant materials
			| aiProcess_FindInvalidData            // detect invalid model data, such as invalid normal vectors
			| aiProcess_FlipUVs                    // flip the V to match the Vulkans way of doing UVs
			;

		if (false)											// This will always be false for now, since we are already combining the model after
			flag |= aiProcess_PreTransformVertices;			// pre-transform all vertices, merge all meshes

		std::string actualPath = _DData.m_Filepaths[_DData.m_iMeshCurrent];
		std::cout << ">>[NOTE]: \tReading File\n";
		m_Scene = importer.ReadFile(actualPath, flag);

		//Failed import
		if (m_Scene == nullptr)
			return false;

		m_SkinGeom = _skinGeom;

		std::cout << ">>[NOTE]: \tSanity Check\n";
		assert(SanityCheck());

		aiVector3D scaling((ai_real)_DData.m_Scale.x, (ai_real)_DData.m_Scale.y, (ai_real)_DData.m_Scale.z);
		m_DescriptorMatrix.Scaling(scaling, m_DescriptorMatrix);
		m_DescriptorMatrix.RotationX((ai_real)_DData.m_Rotate.x, m_DescriptorMatrix);
		m_DescriptorMatrix.RotationY((ai_real)_DData.m_Rotate.y, m_DescriptorMatrix);
		m_DescriptorMatrix.RotationZ((ai_real)_DData.m_Rotate.z, m_DescriptorMatrix);
		aiVector3D translation((ai_real)_DData.m_Translate.x, (ai_real)_DData.m_Translate.y, (ai_real)_DData.m_Translate.z);
		m_DescriptorMatrix.Translation(translation, m_DescriptorMatrix);

		std::cout << ">>[NOTE]: \tImporting Data\n";

		ImportData();

		return true;
	}


	bool Mesh_Loader::ImportGeometryValidateMesh(const aiMesh& AssimpMesh, int& iTexture, int& iColor) noexcept
	{
		if (AssimpMesh.HasPositions() == false)
		{
			printf("WARNING: Found a mesh (%s) without position! mesh will be removed\n", AssimpMesh.mName.C_Str());
			return true;
		}

		if (AssimpMesh.HasFaces() == false)
		{
			printf("WARNING: Found a mesh (%s) without position! mesh will be removed\n", AssimpMesh.mName.C_Str());
			return true;
		}

		if (AssimpMesh.HasNormals() == false)
		{
			printf("WARNING: Found a mesh (%s) without normals! mesh will be removed\n", AssimpMesh.mName.C_Str());
			return true;
		}

		if (AssimpMesh.HasTangentsAndBitangents() == false)
		{
			printf("WARNING: Found a mesh (%s) without Tangets! We will create fake tangets.. but it will look bad!\n", AssimpMesh.mName.C_Str());
		}

		if (AssimpMesh.GetNumUVChannels() != 1)
		{
			if (AssimpMesh.GetNumUVChannels() == 0)
			{
				printf("WARNING: Found a mesh (%s) without UVs we will assign 0,0 to all uvs\n", AssimpMesh.mName.C_Str());
			}
			else
			{
				printf("WARNING: Found a mesh (%s) with too many UV chanels we will use only one...\n", AssimpMesh.mName.C_Str());
			}
		}

		iTexture = [&]()->int
		{
			for (auto i = 0u; i < AssimpMesh.GetNumUVChannels(); ++i)
				if (AssimpMesh.HasTextureCoords(i)) 
					return i;
			return -1;
		}();

		iColor = [&]()->int
		{
			for (auto i = 0u; i < AssimpMesh.GetNumColorChannels(); ++i)
				if (AssimpMesh.HasVertexColors(i)) 
					return i;
			return -1;
		}();

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	//					MESH LOADER: PRIVATE FUNCTIONS
	//----------------------------------------------------------------------------------------------------
	void Mesh_Loader::ImportData()
	{
		std::vector<InputMeshPart> myNodes;

		ImportStaticData(myNodes);

		MergeData(myNodes);

		Optimize(myNodes);

		CreateSkinGeom(Quantize(myNodes));
	}


	void Mesh_Loader::ImportStaticData(std::vector<InputMeshPart>& _MyNodes)
	{
		// process the provided node mesh by extractomg data from the assimp mesh and storing it into my own data structure
		auto ProcessMesh = [&](const aiMesh& AssimpMesh, const aiMatrix4x4& Transform, InputMeshPart& MeshPart, const int iTexCordinates, const int iColors)
		{
			// get the rotation for the normals
			aiQuaternion presentRotation;
			{
				aiVector3D p;
				Transform.DecomposeNoScaling(presentRotation, p);
			}

			MeshPart.m_Name = AssimpMesh.mName.C_Str();
			MeshPart.m_iMaterialInstance = AssimpMesh.mMaterialIndex;

			MeshPart.m_Vertices.resize(AssimpMesh.mNumVertices);
			for (auto i = 0u; i < AssimpMesh.mNumVertices; ++i)
			{
				FullVertices& Vertex = MeshPart.m_Vertices[i];		// the mesh's vertices
					
				auto L = Transform * AssimpMesh.mVertices[i];		// adds pretransformation to the vertices

				// VERTEX POSITIONS //
				Vertex.m_Position = glm::vec3						// populating the transformed vertex position
				(	static_cast<float>(L.x)
					, static_cast<float>(L.y)
					, static_cast<float>(L.z)
				);

				// UV COORDINATES //
				if (iTexCordinates == -1)
				{
					Vertex.m_Texcoord = glm::vec2(0, 0);			// default initializes uv coordinates channels if not present
					assert(false);
				}
				else
				{
					Vertex.m_Texcoord = glm::vec2 
					(
						static_cast<float>(AssimpMesh.mTextureCoords[iTexCordinates][i].x)
						, static_cast<float>(AssimpMesh.mTextureCoords[iTexCordinates][i].y)
					);
				}

				// COLORS //
				if (iColors == -1)
				{
					Vertex.m_Color = glm::vec4(255, 255, 255, 255);	// default initializes color to white if not present
				}
				else
				{
					Vertex.m_Color = glm::vec4
					(
						static_cast<float>(AssimpMesh.mColors[iColors][i].r)
						, static_cast<float>(AssimpMesh.mColors[iColors][i].g)
						, static_cast<float>(AssimpMesh.mColors[iColors][i].b)
						, static_cast<float>(AssimpMesh.mColors[iColors][i].a)
					);
				}

				// NORMALS, TANGENTS, AND BITANGENTS //
				if (AssimpMesh.HasTangentsAndBitangents())
				{
					const auto T = presentRotation.Rotate(AssimpMesh.mTangents[i]);
					const auto B = presentRotation.Rotate(AssimpMesh.mBitangents[i]);
					const auto N = presentRotation.Rotate(AssimpMesh.mNormals[i]);

					Vertex.m_fNormal	= glm::vec3(N.x, N.y, N.z);
					Vertex.m_fTangent	= glm::vec3(T.x, T.y, T.z);
					Vertex.m_fBitangent = glm::vec3(B.x, B.y, B.z);

					Vertex.m_fNormal = glm::normalize(Vertex.m_fNormal);
					Vertex.m_fTangent = glm::normalize(Vertex.m_fTangent);
					Vertex.m_fBitangent = glm::normalize(Vertex.m_fBitangent);

					// compressing the floating point tangent to an 8 bit unsigned integer
					Vertex.m_Tangent.r = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fTangent.x < 0 ? std::max(-128, static_cast<int>(Vertex.m_fTangent.x * 128)) : std::min(127, static_cast<int>(Vertex.m_fTangent.x * 127))));
					Vertex.m_Tangent.g = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fTangent.y < 0 ? std::max(-128, static_cast<int>(Vertex.m_fTangent.y * 128)) : std::min(127, static_cast<int>(Vertex.m_fTangent.y * 127))));
					Vertex.m_Tangent.b = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fTangent.z < 0 ? std::max(-128, static_cast<int>(Vertex.m_fTangent.z * 128)) : std::min(127, static_cast<int>(Vertex.m_fTangent.z * 127))));
					Vertex.m_Tangent.a = 0;

					assert(AssimpMesh.HasNormals());
					Vertex.m_Normal.r = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fNormal.x < 0 ? std::max(-128, static_cast<int>(Vertex.m_fNormal.x * 128)) : std::min(127, static_cast<int>(Vertex.m_fNormal.x * 127))));
					Vertex.m_Normal.g = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fNormal.y < 0 ? std::max(-128, static_cast<int>(Vertex.m_fNormal.y * 128)) : std::min(127, static_cast<int>(Vertex.m_fNormal.y * 127))));
					Vertex.m_Normal.b = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fNormal.z < 0 ? std::max(-128, static_cast<int>(Vertex.m_fNormal.z * 128)) : std::min(127, static_cast<int>(Vertex.m_fNormal.z * 127))));
					Vertex.m_Normal.a = static_cast<std::uint8_t>(static_cast<std::int8_t>(glm::dot(glm::cross(Vertex.m_fTangent, Vertex.m_fNormal), Vertex.m_fBitangent) > 0 ? 127 : -128));
					//Vertex.m_Normal.a = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fTangent.Cross(Vertex.m_fNormal).Dot(Vertex.m_fBitangent) > 0 ? 127 : -128));
				}
				else
				{
					Vertex.m_Tangent.r = 0xff;
					Vertex.m_Tangent.g = 0;
					Vertex.m_Tangent.b = 0;
					Vertex.m_Tangent.a = 0;

					const auto N = presentRotation.Rotate(AssimpMesh.mNormals[i]);

					Vertex.m_fNormal	= glm::vec3(N.x, N.y, N.z);
					Vertex.m_fTangent	= glm::vec3(1, 0, 0);
					Vertex.m_fBitangent = glm::vec3(1, 0, 0);

					Vertex.m_fNormal = glm::normalize(Vertex.m_fNormal);

					Vertex.m_Normal.r = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fNormal.x < 0 ? std::max(-128, static_cast<int>(Vertex.m_fNormal.x * 128)) : std::min(127, static_cast<int>(Vertex.m_fNormal.x * 127))));
					Vertex.m_Normal.g = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fNormal.y < 0 ? std::max(-128, static_cast<int>(Vertex.m_fNormal.y * 128)) : std::min(127, static_cast<int>(Vertex.m_fNormal.y * 127))));
					Vertex.m_Normal.b = static_cast<std::uint8_t>(static_cast<std::int8_t>(Vertex.m_fNormal.z < 0 ? std::max(-128, static_cast<int>(Vertex.m_fNormal.z * 128)) : std::min(127, static_cast<int>(Vertex.m_fNormal.z * 127))));
					Vertex.m_Normal.a = 127;
				}
			}

			// Copy the indices
			for (auto i = 0u; i < AssimpMesh.mNumFaces; ++i)
			{
				const auto& Face = AssimpMesh.mFaces[i];
				for (auto j = 0u; j < Face.mNumIndices; ++j)
				{
					MeshPart.m_Indices.push_back(Face.mIndices[j]);
				}
			}
		};

		// Recurse the scene from the root node and process each mesh we find
		std::function<void(const aiNode&, const aiMatrix4x4&)> RecurseScene = [&](const aiNode& Node, const aiMatrix4x4& ParentTransform)
		{
			const aiMatrix4x4 Transform = ParentTransform * Node.mTransformation;
			auto        iBase = _MyNodes.size();

			// Collect all the meshes
			_MyNodes.resize(iBase + m_Scene->mNumMeshes);
			for (auto i = 0u, end = Node.mNumMeshes; i < end; ++i)
			{
				aiMesh& AssimpMesh = *m_Scene->mMeshes[Node.mMeshes[i]];

				// Check if the mesh is valid, and get its texture coordinates and color
				int iTexCordinates, iColor;
				if (ImportGeometryValidateMesh(AssimpMesh, iTexCordinates, iColor)) 
					continue;

				ProcessMesh(AssimpMesh, Transform, _MyNodes[iBase++], iTexCordinates, iColor);
			}

			// Make sure the base matches what should be in the vector
			if (iBase != _MyNodes.size())
			{
				_MyNodes.erase(_MyNodes.begin() + iBase, _MyNodes.end());
			}

			// Do the children
			for (auto i = 0u; i < Node.mNumChildren; ++i)
			{
				RecurseScene(*Node.mChildren[i], Transform);
			}
		};

		aiMatrix4x4 L2W = m_DescriptorMatrix;				// the pretransform data that was gotten from the descriptor file
		RecurseScene(*m_Scene->mRootNode, L2W);
	}


	bool Mesh_Loader::SanityCheck() noexcept
	{
		m_MeshRefs.resize(m_Scene->mNumMeshes);

		// populates m_MeshRefs
		std::function<void(const aiNode& node)> processNode = [&](const aiNode& Node) noexcept
		{
			for (auto i = 0u, end = Node.mNumMeshes; i < end; ++i)
			{
				aiMesh* pMesh = m_Scene->mMeshes[Node.mMeshes[i]];

				m_MeshRefs[Node.mMeshes[i]].m_Nodes.push_back(&Node);
			}

			for (auto i = 0u; i < Node.mNumChildren; ++i)
			{
				processNode(*Node.mChildren[i]);
			}
		};

		processNode(*m_Scene->mRootNode);

		// Checks if the mesh is skinned, and if it's capable of vertex animations
		for (auto iMesh = 0u; iMesh < m_Scene->mNumMeshes; ++iMesh)
		{
			const aiMesh& AssimpMesh = *m_Scene->mMeshes[iMesh];
			const auto& Refs = m_MeshRefs[iMesh].m_Nodes;

			if (Refs.size() == 0u)
			{
				printf("ERROR: I had a mesh but no reference to it in the scene... very strange\n");
				return false;
			}

			if (AssimpMesh.HasBones())
			{
				if (Refs.size() > 1)
				{
					printf("INFO: I had a skin mesh (%s) that is reference in the scene %zd times. This mesh is capable of vertex animations.\n", AssimpMesh.mName.C_Str(), Refs.size());
					return true;
				}
			}
			else
			{
				if (Refs.size() > 1)
				{
					printf("INFO: I will be duplicating mesh %s, %zd times. This mesh is NOT capable of vertex animations\n", AssimpMesh.mName.C_Str(), Refs.size());
				}
			}
		}

		return true;	// no error
	}


	void _GEOM::Mesh_Loader::MergeData(std::vector<InputMeshPart>& _MyNodes) noexcept
	{
		// Remove Mesh parts with zero vertices
		for (auto i = 0u; i < _MyNodes.size(); ++i)
		{
			if (_MyNodes[i].m_Vertices.size() == 0 || _MyNodes[i].m_Indices.size() == 0)
			{
				_MyNodes.erase(_MyNodes.begin() + i);
				--i;
			}
		}

		// Merge any mesh part based on Mesh and iMaterial...
		for (auto i = 0u; i < _MyNodes.size(); ++i)
		{
			for (auto j = i + 1; j < _MyNodes.size(); ++j)
			{
				// Lets find a candidate to merge... We judge this based on the material instance and mesh name (each mesh can only have one material)
				if (_MyNodes[i].m_iMaterialInstance == _MyNodes[j].m_iMaterialInstance
					&& _MyNodes[i].m_MeshName == _MyNodes[j].m_MeshName)
				{
					const int  iBaseVertex = static_cast<int>(_MyNodes[i].m_Vertices.size());
					const auto iBaseIndex = _MyNodes[i].m_Indices.size();
					_MyNodes[i].m_Vertices.insert(_MyNodes[i].m_Vertices.end(), _MyNodes[j].m_Vertices.begin(), _MyNodes[j].m_Vertices.end());
					_MyNodes[i].m_Indices.insert(_MyNodes[i].m_Indices.end(), _MyNodes[j].m_Indices.begin(), _MyNodes[j].m_Indices.end());

					// Fix the indices
					for (auto I = iBaseIndex; I < _MyNodes[i].m_Indices.size(); ++I)
					{
						_MyNodes[i].m_Indices[I] += iBaseVertex;
					}

					_MyNodes.erase(_MyNodes.begin() + j);
					--j;
				}
			}
		}
	}


	void _GEOM::Mesh_Loader::CreateSkinGeom(const std::vector<CompressedMeshPart>& _MyNodes) noexcept
	{
		// Create final structure using the compressed nodes
		for (auto& E : _MyNodes)
		{
			int iFinalMesh = -1;
			for (auto i = 0u; i < m_SkinGeom->m_Meshes.size(); ++i)
			{
				if (m_SkinGeom->m_Meshes[i].m_Name == E.m_MeshName)
				{
					iFinalMesh = i;
					break;
				}
			}

			if (iFinalMesh == -1)
			{
				iFinalMesh = static_cast<int>(m_SkinGeom->m_Meshes.size());
				m_SkinGeom->m_Meshes.emplace_back();
				m_SkinGeom->m_Meshes.back().m_Name = E.m_MeshName;
			}


			auto& FinalMesh = m_SkinGeom->m_Meshes[iFinalMesh];
			FinalMesh.m_Submeshes.emplace_back();
			auto& SubMesh = FinalMesh.m_Submeshes.back();

			SubMesh.m_Pos = E.m_Pos;
			SubMesh.m_Extra = E.m_Extra;
			SubMesh.m_Indices = E.m_Indices;
			SubMesh.m_iMaterial = E.m_iMaterialInstance;
			SubMesh.m_PosCompressionOffset = E.m_PosCompressionOffset;
			SubMesh.m_UVCompressionOffset = E.m_UVCompressionOffset;
		}
	}


	void _GEOM::Mesh_Loader::Optimize(std::vector<InputMeshPart>& _MyNodes) noexcept
	{
		std::cout << ">>[NOTE]: \tOptimizing Mesh Data\n";

		//Vertex and index optimization
		std::vector<InputMeshPart> optimizedMeshParts;
		for (auto& meshPart : _MyNodes)
		{
			std::size_t index_count = meshPart.m_Indices.size();
			std::vector<uint32_t> remap(index_count);
			std::size_t vertex_count = meshopt_generateVertexRemap(&remap[0], meshPart.m_Indices.data(), index_count, meshPart.m_Vertices.data(), index_count, sizeof(FullVertices));

			InputMeshPart optimized_submesh;
			optimized_submesh.m_Indices.resize(index_count);
			optimized_submesh.m_Vertices.resize(vertex_count);
			optimized_submesh.m_iMaterialInstance = meshPart.m_iMaterialInstance;

			//Remap indices
			meshopt_remapIndexBuffer(optimized_submesh.m_Indices.data(), meshPart.m_Indices.data(), index_count, &remap[0]);

			//Remap vertices
			meshopt_remapVertexBuffer(optimized_submesh.m_Vertices.data(), meshPart.m_Vertices.data(), index_count, sizeof(FullVertices), &remap[0]);

			//Optimize vertex cache
			meshopt_optimizeVertexCache(optimized_submesh.m_Indices.data(), optimized_submesh.m_Indices.data(), index_count, vertex_count);

			//Optimize overdraw
			meshopt_optimizeOverdraw(optimized_submesh.m_Indices.data(), optimized_submesh.m_Indices.data(), index_count, &optimized_submesh.m_Vertices[0].m_Position.x, vertex_count, sizeof(FullVertices), 1.05f);

			//Optimize vertex fetch
			optimized_submesh.m_Vertices.resize(meshopt_optimizeVertexFetch(optimized_submesh.m_Vertices.data(), optimized_submesh.m_Indices.data(), index_count, optimized_submesh.m_Vertices.data(), vertex_count, sizeof(FullVertices)));

			optimizedMeshParts.push_back(optimized_submesh);
		}

		_MyNodes = optimizedMeshParts;
	}


	std::vector<Mesh_Loader::CompressedMeshPart> Mesh_Loader::Quantize(const std::vector<InputMeshPart>& _MyNodes) noexcept
	{
		std::cout << ">>== \t\tQuantizing mesh data...\n";
		std::vector<CompressedMeshPart> CompressedMeshParts(_MyNodes.size());		// create a return vector with the same size as the input vector

		// Create a bounding box for each submesh
		bbox GLOBAL_PosBBox;
		bbox GLOBAL_UVBBox;
		{
			GLOBAL_PosBBox.m_Min = glm::vec3(0.f);		// initialize the min to 0
			GLOBAL_UVBBox.m_Min	= glm::vec3(0.f);

			auto iSubmesh = 0u;
			for (auto& Submesh : _MyNodes)
			{
				bbox PosBBox;
				bbox UVBBox;

				// looping through all the vertices in the submesh
				for (auto& V : Submesh.m_Vertices)
				{
					// VERTEX POSITION BOUNDS //
					PosBBox.AddVerts(&V.m_Position, 1);

					// VERTEX UV COORDINATES BOUNDS //
					glm::vec3 UV(V.m_Texcoord.x, V.m_Texcoord.y, 0.f);
					UVBBox.AddVerts(&UV, 1);
				}

				// GLOBAL BOUNDS //
				{
					// add both bboxes to the global total
					glm::vec3 Pos_Span(PosBBox.getSize());
					glm::vec3 UV_Span(UVBBox.getSize());

					GLOBAL_PosBBox.AddVerts(&Pos_Span, 1);
					GLOBAL_UVBBox.AddVerts(&UV_Span, 1);
				}

				// OFFSET STORAGE //
				{
					// this compression offset is getting updated for each submesh
					auto& RenderSubmesh = CompressedMeshParts[iSubmesh++];
					RenderSubmesh.m_PosCompressionOffset = PosBBox.getCenter();										
					RenderSubmesh.m_UVCompressionOffset = glm::vec2(UVBBox.getCenter().x, UVBBox.getCenter().y);	
				}
			}
		}

		// Set the global scale for the quantization, this will allow us to normalize
		m_PosCompressionScale = GLOBAL_PosBBox.getSize();
		m_UVCompressionScale = glm::vec2(GLOBAL_UVBBox.getSize().x, GLOBAL_UVBBox.getSize().y);

		////////////////////////////////////
		// QUANTIZE VERTEX DATA 
		////////////////////////////////////
		{
			// For each submesh inside my mesh...
			int iSubmesh = 0u;
			for (const auto& submesh : _MyNodes)
			{
				auto& RenderSubmesh = CompressedMeshParts[iSubmesh++];
				std::vector<Geom::VertexExtra>	VEContainer;
				std::vector<Geom::VertexPos>	VPContainer;

				// For each vertex inside the submesh...
				for (const auto& V : submesh.m_Vertices)
				{
					// == QUANTIZE NORMAL, TANGENT, BITANGENT (SIGN) AND UV COORDINATES //
					{
						Geom::VertexExtra VE;
						float f;

						// Quantize the Y component of the normal
						VE.m_QNormalY = static_cast<std::int16_t>(V.m_fNormal.y * (V.m_fNormal.y >= 0 ? 0x1FF : 0x200));

						// Quantize the X and Y components of the tangent
						VE.m_QTangentX = static_cast<std::int16_t>(V.m_fTangent.x * (V.m_fTangent.x >= 0 ? 0x1FF : 0x200));
						VE.m_QTangentY = static_cast<std::int16_t>(V.m_fTangent.y * (V.m_fTangent.y >= 0 ? 0x1FF : 0x200));

						// Store the tangent z sign here so it will be [-1 || 1]
						VE.m_QAlpha = V.m_fTangent.z >= 0 ? 0x1 : 0x3;

						// == Compress the UVs == //

						// Convert the U texture coordinate to [-1, 1], and scale to a 16-bit signed integer
						f = (V.m_Texcoord.x - RenderSubmesh.m_UVCompressionOffset.x) / m_UVCompressionScale.x;
						VE.m_U = static_cast<std::int16_t>(f >= 0 ? f * 0x7FFF : f * 0x8000);

						// Convert the V texture coordinate to [-1, 1], and scale to a 16-bit signed integer
						f = (V.m_Texcoord.y - RenderSubmesh.m_UVCompressionOffset.y) / m_UVCompressionScale.y;
						VE.m_V = static_cast<std::int16_t>(f >= 0 ? f * 0x7FFF : f * 0x8000);

						// Add the compressed vertex to the container
						VEContainer.push_back(VE);
					}
					
					// == QUANTIZE VERTEX POSITION COORDINATES //
					{
						Geom::VertexPos VP;

						float f;

						// Quantize the X, Y, and Z components of the position to fit within a 16-bit signed integer
						f = (V.m_Position.x - RenderSubmesh.m_PosCompressionOffset.x) / m_PosCompressionScale.x;
						assert(f >= -1 && f <= 1);
						VP.m_QPosition_X = static_cast<std::int16_t>(f >= 0 ? f * 0x7FFF : f * 0x8000);

						f = (V.m_Position.y - RenderSubmesh.m_PosCompressionOffset.y) / m_PosCompressionScale.y;
						assert(f >= -1 && f <= 1);
						VP.m_QPosition_Y = static_cast<std::int16_t>(f >= 0 ? f * 0x7FFF : f * 0x8000);

						f = (V.m_Position.z - RenderSubmesh.m_PosCompressionOffset.z) / m_PosCompressionScale.z;
						assert(f >= -1 && f <= 1);
						VP.m_QPosition_Z = static_cast<std::int16_t>(f >= 0 ? f * 0x7FFF : f * 0x8000);

						// Store the Normal.X and some bits in the W part... 
						// Now we must convert the normal to 14 bits... (we reserve the last 2 bit for something else...)
						std::int16_t Nx = std::min((short)0x3FFF, static_cast<std::int16_t>(((V.m_fNormal.x + 1) / 2.0f) * 0x3FFF));

						// Store the sign bit of Normal.Z in the last bit of the normal
						// If NX is zero we will steal a little bit of precision to insert the negative sign...
						if (V.m_fNormal.z < 0)
						{
							if (Nx == 0)
							{
								Nx = -1;
							}
							else
							{
								Nx = -Nx;
							}
						}

						// We are also going to store the bitangent sign bit in bit 14
						if (V.m_Normal.a == 128)
						{
							if (Nx < 0)  Nx = -((1 << 14) | (-Nx));
							else         Nx |= (1 << 14);
						}

						// Store the quantized normal X component with extra information in VPContainer
						VP.m_QPosition_QNormalX = Nx;
						VPContainer.push_back(VP);
					}
				}
				RenderSubmesh.m_Pos = VPContainer;
				RenderSubmesh.m_Extra = VEContainer;
			}
		}

		//Copy other data into the compressed mesh parts
		for (int i = 0; i < _MyNodes.size(); ++i)
		{
			CompressedMeshParts[i].m_iMaterialInstance = _MyNodes[i].m_iMaterialInstance;
			CompressedMeshParts[i].m_Indices = _MyNodes[i].m_Indices;			//might have to quantize this also
			CompressedMeshParts[i].m_MeshName = _MyNodes[i].m_MeshName;
			CompressedMeshParts[i].m_Name = _MyNodes[i].m_Name;
		}

		return CompressedMeshParts;
	}


	void SkinGeom::CastToGeomStruct(Geom& _geom, const glm::vec3& posScaleDecompress, const glm::vec2& UVScaleDecompress) noexcept
	{
		//Get total sizes
		std::size_t totalMeshes = m_Meshes.size();
		std::size_t totalSubMeshes = 0;
		std::size_t totalVertices = 0;
		std::size_t totalExtras = 0;
		std::size_t totalIndices = 0;

		for (auto& _mesh : m_Meshes)
		{
			totalSubMeshes += _mesh.m_Submeshes.size();
			for (auto& _submesh : _mesh.m_Submeshes)
			{
				totalVertices += _submesh.m_Pos.size();
				totalExtras += _submesh.m_Extra.size();
				totalIndices += _submesh.m_Indices.size();
			}
		}

		//Allocate memory
		auto uMesh = std::make_unique<Geom::Mesh[]>(totalMeshes);
		auto uSubMesh = std::make_unique<Geom::SubMesh[]>(totalSubMeshes);
		auto uPos = std::make_unique<Geom::VertexPos[]>(totalVertices);
		auto uExtras = std::make_unique<Geom::VertexExtra[]>(totalExtras);
		auto uIndices = std::make_unique<std::uint32_t[]>(totalIndices);

		auto Mesh = std::span{ uMesh.get(), static_cast<std::size_t>(totalMeshes) };
		auto SubMesh = std::span{ uSubMesh.get(), static_cast<std::size_t>(totalSubMeshes) };
		auto Pos = std::span{ uPos.get(), static_cast<std::size_t>(totalVertices) };
		auto Extras = std::span{ uExtras.get(), static_cast<std::size_t>(totalExtras) };
		auto Indices = std::span{ uIndices.get(), static_cast<std::size_t>(totalIndices) };

		//Start copying data in
		_geom.m_nMeshes = (std::uint32_t)totalMeshes;
		_geom.m_nSubMeshes = (std::uint32_t)totalSubMeshes;
		_geom.m_nVertices = (std::uint32_t)totalVertices;
		_geom.m_nExtras = (std::uint32_t)totalExtras;
		_geom.m_nIndices = (std::uint32_t)totalIndices;
		_geom.m_PosCompressionScale = posScaleDecompress;
		_geom.m_UVCompressionScale = UVScaleDecompress;

		std::size_t iVertex = 0, iIndices = 0, iExtra = 0;
		for (std::size_t i = 0; i < totalMeshes; ++i)
		{
			//Copy name of mesh in
			strcpy_s(Mesh[i].m_name.data(), Mesh[i].m_name.size(), m_Meshes[i].m_Name.c_str());

			//Submesh data
			for (std::size_t j = 0; j < totalSubMeshes; ++j)
			{
				SubMesh[j].m_nFaces = (std::uint32_t)m_Meshes[i].m_Submeshes[j].m_Indices.size() / 3;
				SubMesh[j].m_iIndices = (std::uint32_t)iIndices;
				SubMesh[j].m_iVertices = (std::uint32_t)iVertex;
				SubMesh[j].m_iMaterial = (std::uint16_t)m_Meshes[i].m_Submeshes[j].m_iMaterial;
				SubMesh[j].m_PosCompressionOffset = m_Meshes[i].m_Submeshes[j].m_PosCompressionOffset;
				SubMesh[j].m_UVCompressionOffset = m_Meshes[i].m_Submeshes[j].m_UVCompressionOffset;

				std::size_t vertSize = m_Meshes[i].m_Submeshes[j].m_Pos.size();
				std::size_t extraSize = m_Meshes[i].m_Submeshes[j].m_Extra.size();
				std::size_t indexSize = m_Meshes[i].m_Submeshes[j].m_Indices.size();

				const auto& submesh = m_Meshes[i].m_Submeshes[j];

				//Position
				{
					for (const auto& _pos : submesh.m_Pos)
					{
						auto& pos = Pos[iVertex++];

						pos.m_QPosition_X = _pos.m_QPosition_X;
						pos.m_QPosition_Y = _pos.m_QPosition_Y;
						pos.m_QPosition_Z = _pos.m_QPosition_Z;
						pos.m_QPosition_QNormalX = _pos.m_QPosition_QNormalX;
					}
				}

				//Extras
				{
					for (const auto& _extra : submesh.m_Extra)
					{
						auto& extra = Extras[iExtra++];

						extra.m_Packed = _extra.m_Packed;
						extra.m_U = _extra.m_U;
						extra.m_V = _extra.m_V;
					}
				}

				//Indices
				{
					for (const auto& _indices : submesh.m_Indices)
					{
						auto& indice = Indices[iIndices++];

						indice = _indices;
					}
				}

				SubMesh[j].m_nVertices = (std::uint32_t)vertSize;
				SubMesh[j].m_nIndices = (std::uint32_t)indexSize;
			}
		}

		_geom.m_pMesh		= std::move(uMesh);
		_geom.m_pSubMesh	= std::move(uSubMesh);
		_geom.m_pPos		= std::move(uPos);
		_geom.m_pExtras		= std::move(uExtras);
		_geom.m_pIndices	= std::move(uIndices);
	}


	bool Geom::SerializeGeom(const std::string& assetfilepath, const DescriptorData& Desc, Geom& GeomData) noexcept
	{
		std::string filepath = Desc.m_sOutputPath + assetfilepath;
		std::ofstream outfile(filepath.c_str());
		assert(outfile.is_open());

		outfile << "[Number of Meshes]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nMeshes);

		outfile << std::endl << "[Number of SubMeshes]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nSubMeshes);

		outfile << std::endl << "[Number of Vertices]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nVertices);

		outfile << std::endl << "[Number of Extras]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nExtras);

		outfile << std::endl << "[Number of Indices]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nIndices);

		outfile << std::endl << "[Pos Compression Scale]: ";
		Serialization::SerializeVec3(outfile, GeomData.m_PosCompressionScale);

		outfile << std::endl << "[UV Compression Scale]: ";
		Serialization::SerializeVec2(outfile, GeomData.m_UVCompressionScale);

		outfile << std::endl << "[Meshes]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nMeshes);
		Serialization::SerializeMesh(outfile, GeomData);

		outfile << std::endl << "[SubMeshes]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nSubMeshes);
		Serialization::SerializeSubMesh(outfile, GeomData);

		outfile << std::endl << "[Vertex Positions]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nVertices);
		Serialization::SerializeVertexPos(outfile, GeomData);

		outfile << std::endl << "[Vertex Extras]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nExtras);
		Serialization::SerializeVertexExtra(outfile, GeomData);

		outfile << std::endl << "[Indices]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nIndices);
		Serialization::SerializeIndices(outfile, GeomData);

		outfile.close();

		return true; // no errors
	}


	bool DeserializeGeom(const std::string Filepath, Geom& GeomData) noexcept
	{
		std::ifstream infile("../compiled_geom/Skull_textured.geom");
		assert(infile.is_open());

		Serialization::ReadUnsigned(infile, GeomData.m_nMeshes);
		Serialization::ReadUnsigned(infile, GeomData.m_nSubMeshes);
		Serialization::ReadUnsigned(infile, GeomData.m_nVertices);
		Serialization::ReadUnsigned(infile, GeomData.m_nExtras);
		Serialization::ReadUnsigned(infile, GeomData.m_nIndices);

		Serialization::ReadVec3WithHeader(infile, GeomData.m_PosCompressionScale);
		Serialization::ReadVec2WithHeader(infile, GeomData.m_UVCompressionScale);

		Serialization::ReadMesh(infile, GeomData);
		Serialization::ReadSubMesh(infile, GeomData);
		Serialization::ReadVertexPos(infile, GeomData);
		Serialization::ReadVertexExtra(infile, GeomData);
		Serialization::ReadIndices(infile, GeomData);

		return true;
	}
}