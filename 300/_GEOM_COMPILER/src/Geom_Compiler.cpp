/*!*************************************************************************
****
\file			Geom_Compiler.cpp
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The main functions used to execute the geom compiler functions

****************************************************************************
***/

#include <Geom_Compiler.h>
#include <Helper.h>

#include <span>
#include <limits>
#include <iostream>
#include <functional>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include <meshoptimizer.h>

#pragma comment( lib, "../lib/assimp/BINARIES/Win32/lib/Release/assimp-vc142-mt.lib")

// Artist create different sized models -- units
// nodes are different sizes -- units

namespace _GEOM
{
	//----------------------------------------------------------------------------------------------------
	//					MESH LOADER: PUBLIC FUNCTIONS
	//----------------------------------------------------------------------------------------------------

/***************************************************************************/
/*!
\brief
	loads the mesh given the data extracted from my descriptor file
*/
/**************************************************************************/
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
			//| aiProcess_FindInvalidData            // detect invalid model data, such as invalid normal vectors. 
			| aiProcess_FlipUVs                    // flip the V to match the Vulkans way of doing UVs
			;

		if (false)											// This will always be false for now, since we are already combining the model after
			flag |= aiProcess_PreTransformVertices;			// pre-transform all vertices, merge all meshes

		std::string actualPath = _DData.m_Filepaths[_DData.m_iMeshCurrent];
		std::cout << ">>[NOTE]: \tReading File\n";
		m_Scene = importer.ReadFile(actualPath, flag);

#pragma region create descriptor file

		// Handles the creation of the descriptor files
		std::string geompath;
		CheckAndCreateDescriptorFileMESH(actualPath, geompath);

#pragma endregion

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


/***************************************************************************/
/*!
\brief
	Import the data, and filter out what kind of mesh is being extracted
*/
/**********************************ImportGeometryValidateMesh****************************************/
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
				printf("WARNING: Found a mesh (%s) with (%d) UV chanels we will use only one...\n", AssimpMesh.mName.C_Str(), AssimpMesh.GetNumUVChannels());
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
/***************************************************************************/
/*!
\brief
	wrapper function to deserialize and sort, calls internal functions.
*/
/**************************************************************************/
	void Mesh_Loader::ImportData()
	{
		std::vector<InputMeshPart> myNodes;

		ImportStaticData(myNodes);

		MergeData(myNodes);

		Optimize(myNodes);

		CreateSkinGeom(Quantize(myNodes));
	}

/***************************************************************************/
/*!
\brief
	obtains the initial data from the assimp mesh
	contains recurseScene, processMesh
	Animation data is being extracted here
*/
/**************************************************************************/
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

			MeshPart.m_MeshName				= m_MeshName;
			MeshPart.m_Name					= AssimpMesh.mName.C_Str();
			MeshPart.m_iMaterialInstance	= AssimpMesh.mMaterialIndex;

			// getting the mesh's material
			if (AssimpMesh.mMaterialIndex >= 0)
			{
				aiMaterial* material = m_Scene->mMaterials[AssimpMesh.mMaterialIndex];
				std::vector<Geom::Texture> diffusemaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				MeshPart.m_Textures.insert(MeshPart.m_Textures.end(), diffusemaps.begin(), diffusemaps.end());

				std::vector<Geom::Texture> specularmaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				MeshPart.m_Textures.insert(MeshPart.m_Textures.end(), specularmaps.begin(), specularmaps.end());
			}

			// Copy the vertices
			MeshPart.m_Vertices.resize(AssimpMesh.mNumVertices);
			for (auto i = 0u; i < AssimpMesh.mNumVertices; ++i)
			{
				FullVertices& Vertex = MeshPart.m_Vertices[i];		// the mesh's vertices
				DefaultInitializeVertexBoneData(Vertex);			// default initializes the bone data
					
				// VERTEX POSITIONS //
				auto L = Transform * AssimpMesh.mVertices[i];		// adds pretransformation to the vertices
				Vertex.m_Position = AssimpHelper::GetGLMVec(L);

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
					Vertex.m_Color = glm::vec4(1.f, 1.f, 1.f, 1.f);	// default initializes color to white if not present
				}
				else
				{
					m_SkinGeom->m_bVtxClrPresent = true;
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

					assert(AssimpMesh.HasNormals());
				}
				else
				{
					const auto N = presentRotation.Rotate(AssimpMesh.mNormals[i]);

					Vertex.m_fNormal	= glm::vec3(N.x, N.y, N.z);
					Vertex.m_fTangent	= glm::vec3(1, 0, 0);
					Vertex.m_fBitangent = glm::vec3(1, 0, 0);

					Vertex.m_fNormal = glm::normalize(Vertex.m_fNormal);
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

			// Copy the bones
			if (AssimpMesh.HasBones()) 
			{
				Animation newanimation;
				ExtractBoneWeightForVertex(newanimation, MeshPart, AssimpMesh);
				m_SkinGeom->m_bHasAnimation = true;
				m_SkinGeom->m_Animation.emplace_back(newanimation);
			}
		};

		// Recurse the scene from the root node and process each mesh we find
		std::function<void(const aiNode&, const aiMatrix4x4&)> RecurseScene = [&](const aiNode& Node, const aiMatrix4x4& ParentTransform)
		{
			// set to identity matrix
			aiMatrix4x4 Transform;

			auto        iBase = _MyNodes.size();

			// Collect all the meshes
			_MyNodes.resize(iBase + m_Scene->mNumMeshes);
			for (auto i = 0u, end = Node.mNumMeshes; i < end; ++i)
			{
				aiMesh& AssimpMesh = *m_Scene->mMeshes[Node.mMeshes[i]];

				// Get the transformation only if the mesh is non animated. Updated only once
				if (!AssimpMesh.HasBones() && i == 0) {
					Transform = ParentTransform * Node.mTransformation;
				}

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
		
		if (m_SkinGeom->m_bHasAnimation)
		{
			std::cout << ">>[NOTE]: \tImporting Bones\n";
			for (unsigned numanims{}; numanims < m_Scene->mNumAnimations; ++numanims)
			{
				auto& animation = m_Scene->mAnimations[numanims];

				if (m_SkinGeom->m_Animation.size() < numanims + 1)
					m_SkinGeom->m_Animation.emplace_back(Animation());

				m_SkinGeom->m_Animation[numanims].m_Duration = static_cast<float>(animation->mDuration);					// set the duration of the animation
				m_SkinGeom->m_Animation[numanims].m_TicksPerSecond = static_cast<float>(animation->mTicksPerSecond);		// set the ticks per second of the animation
				ReadHierarchyData(m_SkinGeom->m_Animation[numanims].m_RootNode, m_Scene->mRootNode);
				ReadMissingBones(m_SkinGeom->m_Animation[numanims], animation);
			}

			CleanAnimationData(m_SkinGeom->m_Animation);
			std::cout << ">>[NOTE]: \tFinished Importing Bones\n";
		}
	}

/***************************************************************************/
/*!
\brief
	contains m_meshrefs. sanity checks on the type of the incoming mesh
	contains processNode
*/
/**************************************************************************/
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

		// uses m_meshrefs to check if the mesh is skinned, and if it's capable of vertex animations
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
				if (Refs.size() >= 1)
				{
					printf("INFO: I had a skin mesh (%s) that is reference in the scene %zd times. This mesh is capable of vertex animations.\n", AssimpMesh.mName.C_Str(), Refs.size());
					return true;
				}
			}
			else
			{
				if (Refs.size() >= 1)
				{
					printf("INFO: I will be duplicating mesh %s, %zd times. This mesh is NOT capable of vertex animations\n", AssimpMesh.mName.C_Str(), Refs.size());
				}
			}
		}

		return true;	// no error
	}

/***************************************************************************/
/*!
\brief
	merges the submeshes into one big mesh so we have lesser things to
	de/serialize. makes things neater
*/
/**************************************************************************/
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
					_MyNodes[i].m_Textures.insert(_MyNodes[i].m_Textures.end(), _MyNodes[j].m_Textures.begin(), _MyNodes[j].m_Textures.end());

					// removing duplicate textures
					std::sort(_MyNodes[i].m_Textures.begin(), _MyNodes[i].m_Textures.end(), [](const Geom::Texture& lhs, const Geom::Texture& rhs) {
						return lhs.path < rhs.path;
						});
					auto newEnd = std::unique(_MyNodes[i].m_Textures.begin(), _MyNodes[i].m_Textures.end(), [](const Geom::Texture& lhs, const Geom::Texture& rhs){
						return lhs.path == rhs.path;
						});
					_MyNodes[i].m_Textures.erase(newEnd, _MyNodes[i].m_Textures.end());

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


/***************************************************************************/
/*!
\brief
	casts the compressed nodes into a skin geom
*/
/**************************************************************************/
	void _GEOM::Mesh_Loader::CreateSkinGeom(const std::vector<CompressedMeshPart>& _MyNodes) noexcept
	{
		// Create final structure using the compressed nodes
		for (auto& E : _MyNodes)
		{
			int iFinalMesh = -1;
			for (auto i = 0u; i < m_SkinGeom->m_Meshes.size(); ++i)					// if there is already an existing skingeom mesh
			{
				if (m_SkinGeom->m_Meshes[i].m_Name == E.m_MeshName)
				{
					iFinalMesh = i;
					break;
				}
			}

			if (iFinalMesh == -1)													// if there is no existing skingeom mesh, create an empty one
			{
				iFinalMesh = static_cast<int>(m_SkinGeom->m_Meshes.size());			
				m_SkinGeom->m_Meshes.emplace_back();
				m_SkinGeom->m_Meshes.back().m_Name = E.m_MeshName;
			}

			if (E.m_Textures.size() > 0) {
				m_SkinGeom->m_bHasTextures = true;
				m_SkinGeom->m_Textures = E.m_Textures;								// add the textures here
			}

			auto& FinalMesh = m_SkinGeom->m_Meshes[iFinalMesh];						

			// Merging all the bboxes into a final bbox for the mesh
			FinalMesh.m_MeshBBOX.MergeBBOX(E.m_Bbox);

			FinalMesh.m_Submeshes.emplace_back();									// create a new submesh instance for the mesh
			auto& SubMesh = FinalMesh.m_Submeshes.back();

			// Copy the data over
			SubMesh.m_Vertex	= E.m_Vertex;
			SubMesh.m_Extra		= E.m_Extra;
			SubMesh.m_Indices	= E.m_Indices;
			SubMesh.m_iMaterial = E.m_iMaterialInstance;
		}
	}

/***************************************************************************/
/*!
\brief
	using meshoptimizer to optimize the mesh
*/
/**************************************************************************/
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
			optimized_submesh.m_Textures = meshPart.m_Textures;

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

			optimized_submesh.m_MeshName = meshPart.m_MeshName;
			optimized_submesh.m_Name = meshPart.m_Name;

			optimizedMeshParts.push_back(optimized_submesh);
		}

		_MyNodes = optimizedMeshParts;
	}

/***************************************************************************/
/*!
\brief
	quantizes the data
	(WIP)
*/
/**************************************************************************/
	std::vector<Mesh_Loader::CompressedMeshPart> Mesh_Loader::Quantize(const std::vector<InputMeshPart>& _MyNodes) noexcept
	{
		std::cout << ">>== \t\tQuantizing mesh data...\n";
		std::vector<CompressedMeshPart> CompressedMeshParts(_MyNodes.size());		// create a return vector with the same size as the input vector


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

				RenderSubmesh.m_Bbox.m_Min = glm::vec3(0.f);	// initialize the min to 0

				// For each vertex inside the submesh...
				for (const auto& V : submesh.m_Vertices)
				{
					RenderSubmesh.m_Bbox.AddVerts(&V.m_Position);

					// == NORMAL, TANGENT, BITANGENT (SIGN) AND UV COORDINATES //
					{
						Geom::VertexExtra VE{};
						
						VE.m_Normal.x = V.m_fNormal.x;
						VE.m_Normal.y = V.m_fNormal.y;
						VE.m_Normal.z = V.m_fNormal.z;
						
						// Quantize the X and Y components of the tangent
						VE.m_Tangent.x = V.m_fTangent.x;
						VE.m_Tangent.y = V.m_fTangent.y;
						VE.m_Tangent.z = V.m_fTangent.z;

						// Add the compressed vertex to the container
						VEContainer.push_back(VE);
					}
					
					// == VERTEX POSITION COORDINATES //
					{
						Geom::VertexPos VP{};
						
						VP.m_Pos[0] = V.m_Position.x;
						VP.m_Pos[1] = V.m_Position.y;
						VP.m_Pos[2] = V.m_Position.z;	

						VP.m_Clr[0] = V.m_Color.x;
						VP.m_Clr[1] = V.m_Color.y;
						VP.m_Clr[2] = V.m_Color.z;

						VP.m_UV[0] = V.m_Texcoord.x;
						VP.m_UV[1] = V.m_Texcoord.y;

						for (int i{}; i < MAX_BONE_INFLUENCE; ++i)
						{
							VP.m_BoneIDs[i] = V.m_BoneIDs[i];
							VP.m_Weights[i] = V.m_Weights[i];
						}

						VPContainer.push_back(VP);
					}
				}

				RenderSubmesh.m_Vertex = VPContainer;
				RenderSubmesh.m_Extra = VEContainer;

				// Copy the rest of the data over to the compressed mesh part
				RenderSubmesh.m_Textures			= submesh.m_Textures;	
				RenderSubmesh.m_iMaterialInstance	= submesh.m_iMaterialInstance;
				RenderSubmesh.m_Indices				= submesh.m_Indices;
				RenderSubmesh.m_MeshName			= submesh.m_MeshName;
				RenderSubmesh.m_Name				= submesh.m_Name;
				RenderSubmesh.m_Textures			= submesh.m_Textures;
			}
		}

		return CompressedMeshParts;
	}

/***************************************************************************/
/*!
\brief
	casts to the geom struct from the skingeom struct
	this is the final struct that will be serialized into binary
*/
/**************************************************************************/
	void SkinGeom::CastToGeomStruct(Geom& _geom) noexcept
	{
		//Get total sizes
		std::size_t totalMeshes = m_Meshes.size();
		std::size_t totalSubMeshes = 0;
		std::size_t totalVertices = 0;
		std::size_t totalExtras = 0;
		std::size_t totalIndices = 0;

		// accumulate the total sizes, for memory allocation
		for (auto& _mesh : m_Meshes)
		{
			totalSubMeshes += _mesh.m_Submeshes.size();
			for (auto& _submesh : _mesh.m_Submeshes)
			{
				totalVertices += _submesh.m_Vertex.size();
				totalExtras += _submesh.m_Extra.size();
				totalIndices += _submesh.m_Indices.size();
			}
		}

		// Allocate memory
		auto uMesh		= std::make_unique<Geom::Mesh[]>(totalMeshes);
		auto uSubMesh	= std::make_unique<Geom::SubMesh[]>(totalSubMeshes);
		auto uPos		= std::make_unique<Geom::VertexPos[]>(totalVertices);
		auto uExtras	= std::make_unique<Geom::VertexExtra[]>(totalExtras);
		auto uIndices	= std::make_unique<std::uint32_t[]>(totalIndices);

		auto Mesh		= std::span{ uMesh.get(), static_cast<std::size_t>(totalMeshes) };				// Geom Struct
		auto SubMesh	= std::span{ uSubMesh.get(), static_cast<std::size_t>(totalSubMeshes) };		// Geom Struct
		auto Pos		= std::span{ uPos.get(), static_cast<std::size_t>(totalVertices) };				// Geom Struct
		auto Extras		= std::span{ uExtras.get(), static_cast<std::size_t>(totalExtras) };			// Geom Struct
		auto Indices	= std::span{ uIndices.get(), static_cast<std::size_t>(totalIndices) };			// Geom Struct
																										// Geom Struct
		// Start copying static data into geom struct
		_geom.m_nMeshes			= (std::uint32_t)totalMeshes;
		_geom.m_nSubMeshes		= (std::uint32_t)totalSubMeshes;
		_geom.m_nVertices		= (std::uint32_t)totalVertices;
		_geom.m_nExtras			= (std::uint32_t)totalExtras;
		_geom.m_nIndices		= (std::uint32_t)totalIndices;

		std::size_t iVertex = 0, iIndices = 0, iExtra = 0;
		for (std::size_t i = 0; i < totalMeshes; ++i)
		{
			// Copy name of mesh in
			strcpy_s(Mesh[i].m_name.data(), Mesh[i].m_name.size(), m_Meshes[i].m_Name.c_str());
			
			// Copy the animation data in
			if (m_bHasAnimation) {
				Mesh[i].m_Animation = std::move(m_Animation);
			}

			if(m_bHasTextures) {
				Mesh[i].m_Texture = std::move(m_Textures);
			}

			// push the bounding box over
			Mesh[i].m_MeshBBOX = std::move(m_Meshes[i].m_MeshBBOX);

			// Copy the bool statuses in
			_geom.m_bHasAnimations	= m_bHasAnimation ? true : _geom.m_bHasAnimations;
			_geom.m_bVtxClrPresent	= m_bVtxClrPresent ? true : _geom.m_bVtxClrPresent;
			_geom.m_bHasTextures	= m_bHasTextures ? true : _geom.m_bHasTextures;

			//Submesh data
			for (std::size_t j = 0; j < totalSubMeshes; ++j)
			{
				SubMesh[j].m_nFaces		= (std::uint32_t)m_Meshes[i].m_Submeshes[j].m_Indices.size() / 3;
				SubMesh[j].m_iIndices	= (std::uint32_t)iIndices;
				SubMesh[j].m_iVertices	= (std::uint32_t)iVertex;
				SubMesh[j].m_iMaterial	= (std::uint16_t)m_Meshes[i].m_Submeshes[j].m_iMaterial;

				std::size_t vertSize = m_Meshes[i].m_Submeshes[j].m_Vertex.size();
				std::size_t extraSize = m_Meshes[i].m_Submeshes[j].m_Extra.size();
				std::size_t indexSize = m_Meshes[i].m_Submeshes[j].m_Indices.size();

				const auto& submesh = m_Meshes[i].m_Submeshes[j];

				// VertexPos
				{
					for (const auto& _pos : submesh.m_Vertex)
					{
						auto& savepos = Pos[iVertex++];

						savepos.m_Pos = _pos.m_Pos;
						savepos.m_Clr = _pos.m_Clr;
						savepos.m_UV = _pos.m_UV;

						for (int i{}; i < MAX_BONE_INFLUENCE; ++i)
						{
							savepos.m_BoneIDs[i] = _pos.m_BoneIDs[i];
							savepos.m_Weights[i] = _pos.m_Weights[i];
						}
					}
				}

				// VertexExtra
				{
					for (const auto& _extra : submesh.m_Extra)
					{
						auto& saveextra = Extras[iExtra++];
						
						saveextra.m_Normal = _extra.m_Normal;
						saveextra.m_Tangent = _extra.m_Tangent;
					}
				}

				// Indices
				{
					for (const auto& _indices : submesh.m_Indices)
					{
						auto& saveindice = Indices[iIndices++];

						saveindice = _indices;
					}
				}

				SubMesh[j].m_nVertices = (std::uint32_t)vertSize;
				SubMesh[j].m_nIndices  = (std::uint32_t)indexSize;
			}
		}

		// release the memory to the geom struct
		_geom.m_pMesh		= std::move(uMesh);
		_geom.m_pSubMesh	= std::move(uSubMesh);
		_geom.m_pPos		= std::move(uPos);
		_geom.m_pExtras		= std::move(uExtras);
		_geom.m_pIndices	= std::move(uIndices);
	}


	//===== Serialize the Geom Struct =====//
/***************************************************************************/
/*!
\brief
	serializes the geom struct into binary
*/
/**************************************************************************/
	bool Geom::SerializeGeom(const std::string& assetfilepath, Geom& GeomData) noexcept
	{
#if 1
		std::string filepath = assetfilepath;
		std::ofstream outfile(filepath.c_str(), std::ios::binary);
		assert(outfile.is_open());

		// Output values
		outfile.write((char*)&GeomData.m_nMeshes, sizeof(GeomData.m_nMeshes));			// m_nMeshes
		outfile.write((char*)&GeomData.m_nSubMeshes, sizeof(GeomData.m_nSubMeshes));	// m_nSubMeshes
		outfile.write((char*)&GeomData.m_nVertices, sizeof(GeomData.m_nVertices));		// m_nVertices
		outfile.write((char*)&GeomData.m_nExtras, sizeof(GeomData.m_nExtras));			// m_nExtras
		outfile.write((char*)&GeomData.m_nIndices, sizeof(GeomData.m_nIndices));		// m_nIndices

		// Flags
		outfile.write((char*)&GeomData.m_bHasAnimations, sizeof(bool));					// bAnimations
		outfile.write((char*)&GeomData.m_bHasTextures, sizeof(bool));					// bTextures
		outfile.write((char*)&GeomData.m_bVtxClrPresent, sizeof(bool));					// bColors

		// Vertex Positions, UVs, Colors, bone weights and IDs
		for (uint32_t i{}; i < GeomData.m_nVertices; ++i) {
			outfile.write((char*)&GeomData.m_pPos[i], sizeof(Geom::VertexPos));
		}

		// Indices
		for (uint32_t i{}; i < GeomData.m_nIndices; ++i) {
			outfile.write((char*)&GeomData.m_pIndices[i], sizeof(std::uint32_t));
		}

		// Meshes
		for (uint32_t i{}; i < GeomData.m_nMeshes; ++i) 
		{
			auto& meshInst = GeomData.m_pMesh[i];
			outfile.write((char*)&meshInst.m_name, sizeof(char) * 64);						// name of the mesh
			outfile.write((char*)&meshInst.m_MeshBBOX, sizeof(bbox));						// the bounding box of the mesh

			// Texture Data
			if (GeomData.m_bHasTextures)
			{
				uint8_t numberoftextures = static_cast<uint8_t>(meshInst.m_Texture.size());
				outfile.write((char*)&numberoftextures, sizeof(uint8_t));

				for (int j{}; j < meshInst.m_Texture.size(); ++j)
				{
					auto& textureinst = meshInst.m_Texture[j];

					uint8_t strlen = static_cast<uint8_t>(textureinst.path.size());
					outfile.write((char*)&strlen, sizeof(uint8_t));							// path name length
					outfile.write((char*)textureinst.path.c_str(), strlen);						// path name

					strlen = static_cast<uint8_t>(textureinst.type.size());
					outfile.write((char*)&strlen, sizeof(uint8_t));							// type name length
					outfile.write((char*)textureinst.type.c_str(), strlen);						// type name					
				}
			}

			// Animation Data
			if (GeomData.m_bHasAnimations) 
			{
				uint8_t animationSize = (uint8_t)meshInst.m_Animation.size();
				outfile.write((char*)&animationSize, sizeof(uint8_t));						// Number of animations

				for (int j{}; j < meshInst.m_Animation.size(); ++j)
				{
					auto& animation = meshInst.m_Animation[j];

					uint32_t numberofbones = static_cast<uint32_t>(animation.m_Bones.size());
					outfile.write((char*)&numberofbones, sizeof(uint32_t));					// Number of bones
					outfile.write((char*)&animation.m_BoneCounter, sizeof(uint32_t));		// Number of boneinfomaps
					outfile.write((char*)&animation.m_Duration, sizeof(float));				// Duration
					outfile.write((char*)&animation.m_TicksPerSecond, sizeof(float));		// Ticks Per Second

					// Bone info map
					for (const auto& boneinfo : animation.m_BoneInfoMap)
					{
						uint8_t strlen = (uint8_t)boneinfo.first.size();
						outfile.write((char*)&strlen, sizeof(uint8_t));						// name length
						outfile.write((char*)boneinfo.first.c_str(), strlen);				// name
						outfile.write((char*)&boneinfo.second, sizeof(_GEOM::BoneInfo));	// boneinfo
					}

					// Bones
					//for (int j{}; j < animation.m_Bones.size(); ++j)
					for(auto& bone : animation.m_Bones)
					{
						auto& boneinst = bone.second;
						
						uint8_t strlen = (uint8_t)boneinst.m_Name.size();
						outfile.write((char*)&strlen, sizeof(uint8_t));						// name length
						outfile.write((char*)boneinst.m_Name.c_str(), strlen);				// name	

						outfile.write((char*)&boneinst.m_ID, sizeof(int));					// id

						outfile.write((char*)&boneinst.m_NumPositions, sizeof(int));		// numpos
						outfile.write((char*)&boneinst.m_NumRotations, sizeof(int));		// numrot
						outfile.write((char*)&boneinst.m_NumScalings, sizeof(int));			// numscale

						// local transform
						outfile.write((char*)&boneinst.m_LocalTransform, sizeof(glm::mat4));

						// keypositions
						for (int k{}; k < boneinst.m_NumPositions; ++k) {
							outfile.write((char*)&boneinst.m_Positions[k], sizeof(_GEOM::KeyPosition));
						}

						// keyrotations
						for (int k{}; k < boneinst.m_NumRotations; ++k) {
							outfile.write((char*)&boneinst.m_Rotations[k], sizeof(_GEOM::KeyRotation));
						}

						// keyscalings
						for (int k{}; k < boneinst.m_NumScalings; ++k) {
							outfile.write((char*)&boneinst.m_Scales[k], sizeof(_GEOM::KeyScale));
						}
					}

					// AssimpNodeData
					Serialization::SerializeAssimpNodeData(outfile, animation.m_RootNode);
				}
			}	// animation data
		}

		// Submeshes
		for (uint32_t i{}; i < GeomData.m_nSubMeshes; ++i) {
			_GEOM::Geom::SubMesh test = GeomData.m_pSubMesh[i];
			outfile.write((char*)&GeomData.m_pSubMesh[i], sizeof(Geom::SubMesh));
		}

		// Vertex Extra
		for (uint32_t i{}; i < GeomData.m_nExtras; ++i) {
			outfile.write((char*)&GeomData.m_pExtras[i], sizeof(Geom::VertexExtra));
		}

		outfile.close();
									
#else											
		std::string filepath = assetfilepath;
		std::ofstream outfile(filepath.c_str());
		assert(outfile.is_open());

		outfile << std::fixed << std::setprecision(3);	// set precision to 3 decimal places

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

		outfile << std::endl << "[Meshes]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nMeshes);
		Serialization::SerializeMesh(outfile, GeomData);

		outfile << std::endl << "[SubMeshes]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nSubMeshes);
		Serialization::SerializeSubMesh(outfile, GeomData);

		outfile << std::endl << "[Vertex Positions]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nVertices);
		Serialization::SerializeVertexPos(outfile, GeomData);

		//outfile << std::endl << "[Vertex Extras]: ";						// == We dont need these data for now ==	
		//Serialization::SerializeUnsigned(outfile, GeomData.m_nExtras);
		//Serialization::SerializeVertexExtra(outfile, GeomData);

		outfile << std::endl << "[Indices]: ";
		Serialization::SerializeUnsigned(outfile, GeomData.m_nIndices);
		Serialization::SerializeIndices(outfile, GeomData);

		outfile << std::endl << "[Vertex Animation]: ";		//!< reorder this, for deserialization
		if (GeomData.m_bHasAnimations) {
			outfile << "VTX_ANIMATION_YES ";
			Serialization::SerializeAnimation(outfile, GeomData);
		}
		else {
			outfile << "VTX_ANIMATION_NO ";
		}
#endif

		return true; // no errors
	}


	////////////////////////////////////////////////////
	//===== Loading Helper Function Definitions =====//
	////////////////////////////////////////////////////

/***************************************************************************/
/*!
\brief
	loads the material textures such as the filename to the textures
*/
/**************************************************************************/
	std::vector<Geom::Texture> Mesh_Loader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) noexcept
	{
		std::vector<Geom::Texture> textures;
		for (unsigned int i{}; i < mat->GetTextureCount(type); ++i)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			Geom::Texture texture;
			texture.type = typeName;
			texture.path = getFileNameWithExtension(str.C_Str());
			textures.emplace_back(texture);
		}

		return textures;
	};


/***************************************************************************/
/*!
\brief
	// BONE:: Read Hierarchy data. This is for the bone matrix offset
*/
/**************************************************************************/
	void Mesh_Loader::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src) noexcept
	{
		assert(src);

		dest.m_Name = src->mName.data;
		dest.m_Transformation = AssimpHelper::ConvertMatrixToGLMFormat(src->mTransformation);
		dest.m_NumChildren = src->mNumChildren;

		for (unsigned i{}; i < src->mNumChildren; ++i)
		{
			AssimpNodeData newData;
			ReadHierarchyData(newData, src->mChildren[i]);
			dest.m_Children.emplace_back(newData);
		}
	};


/***************************************************************************/
/*!
\brief
	// BONE:: Initialize bone data
*/
/**************************************************************************/
	void Mesh_Loader::DefaultInitializeVertexBoneData(FullVertices& vertex) noexcept
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			vertex.m_BoneIDs[i] = -1;
			vertex.m_Weights[i] = 0.0f;
		}
	};

/***************************************************************************/
/*!
\brief
	// BONE:: Set bone data
*/
/**************************************************************************/
	void Mesh_Loader::SetVertexBoneData(FullVertices& vertex, int boneID, float weight) noexcept
	{
		// this particular vertex can only be influenced by a maximum number of
		// bones that is equal to MAX_BONE_INFLUENCE
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			// look for an empty slot for the weights.
			if (vertex.m_BoneIDs[i] < 0)
			{
				vertex.m_Weights[i] = weight;	// how much this vertex is being influenced
				vertex.m_BoneIDs[i] = boneID;	// this vertex is influenced by this particular bone
				break;
			}
		}
	};

/***************************************************************************/
/*!
\brief
	// BONE:: Load bone data
*/
/**************************************************************************/
	void Mesh_Loader::ExtractBoneWeightForVertex(Animation& anims, Mesh_Loader::InputMeshPart& Vertex, const aiMesh& mesh) noexcept
	{
		auto& rBoneInfoMap	= anims.m_BoneInfoMap;
		int& iBoneCount		= anims.m_BoneCounter;

		for (unsigned boneindex = 0; boneindex < mesh.mNumBones; ++boneindex)
		{
			int boneID = -1;
			std::string boneName = mesh.mBones[boneindex]->mName.C_Str();

			// >> check if the bone already exists
			if (rBoneInfoMap.find(boneName) == rBoneInfoMap.end())
			{
				// add the new bone to the bone map if it doesn't exist
				BoneInfo newboneinfo{};
				newboneinfo.id = iBoneCount;
				newboneinfo.offset = AssimpHelper::ConvertMatrixToGLMFormat(mesh.mBones[boneindex]->mOffsetMatrix);
				rBoneInfoMap[boneName] = newboneinfo;
				boneID = iBoneCount++;
			}
			else
			{
				boneID = rBoneInfoMap[boneName].id;
			}
			assert(boneID != -1);

			// >> populating the weight each vertex has on the bone
			const auto& weights = mesh.mBones[boneindex]->mWeights;
			int numWeights = mesh.mBones[boneindex]->mNumWeights;
			for (int weightindex{}; weightindex < numWeights; ++weightindex)
			{
				int vertexID = weights[weightindex].mVertexId;
				float weight = weights[weightindex].mWeight;
				assert(vertexID <= Vertex.m_Vertices.size());
				SetVertexBoneData(Vertex.m_Vertices[vertexID], boneID, weight);
			}
		}
	};

/***************************************************************************/
/*!
\brief
	// BONE:: Read missing bones and populate bone vector
*/
/**************************************************************************/
	void Mesh_Loader::ReadMissingBones(Animation& myanimation, const aiAnimation* sceneanimation) noexcept
	{
		int size = sceneanimation->mNumChannels;

		auto& boneInfoMap = myanimation.m_BoneInfoMap;
		int& boneCount = myanimation.m_BoneCounter;

		// Reading channels (bones engaged in an animation and their keyframes)
		for (int i{}; i < size; ++i)
		{
			auto channel = sceneanimation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			// sanitycheck for any misisng boneinfomap which should already be initialized before
			// the call to this function
			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].id = boneCount++;
			}

			// emplace back the bone data, based off the boneinfomap and channel data
			Bone bone(boneName, boneInfoMap[channel->mNodeName.data].id, channel);

			myanimation.m_Bones[bone.m_Name] = bone;
		}
	};


	void Mesh_Loader::CleanAnimationData(std::vector<Animation>& skinGeomAnimation) noexcept
	{
		int lastAnimation{};
		for (; lastAnimation < skinGeomAnimation.size(); ++lastAnimation)
		{
			if(skinGeomAnimation[lastAnimation].m_Bones.size() == 0)
				break;
		}

		skinGeomAnimation.erase(skinGeomAnimation.begin() + lastAnimation, skinGeomAnimation.end());
	}
}