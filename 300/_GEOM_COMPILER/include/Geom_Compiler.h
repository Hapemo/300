/*!*************************************************************************
****
\file			Geom_Compiler.h
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The function declarations for the mesh loading class. These functions
	help in loading the mesh data from assimp

****************************************************************************
***/

#pragma once

#ifndef _GEOM_COMPILER
#define _GEOM_COMPILER

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/version.h>

#include <vector>

#include <descriptor.h>
#include <Bone.h>
#include <geom.h>

namespace _GEOM
{

/***************************************************************************/
/*!
\brief
	This class contains the functionalities and variables required
	to load the mesh from assimp
*/
/**************************************************************************/
	class Mesh_Loader
	{
	public:

	/***************************************************************************/
	/*!
	\brief
		The uncompressed version of the data collected from assimp
	*/
	/**************************************************************************/
		struct InputMeshPart
		{
			std::string						m_MeshName;
			std::string						m_Name;
			std::vector<FullVertices>		m_Vertices;
			std::vector<Geom::Texture>		m_Textures;						
			std::vector<uint32_t>			m_Indices;
			int								m_iMaterialInstance{};
		};

	/***************************************************************************/
	/*!
	\brief
		The compressed version of the data collected from assimp
	*/
	/**************************************************************************/
		struct CompressedMeshPart
		{
			std::vector<Geom::VertexPos>	m_Vertex;
			std::vector<Geom::VertexExtra>	m_Extra;
			std::vector<Geom::Texture>		m_Textures;						
			std::vector<uint32_t>			m_Indices;
			std::string						m_MeshName; 
			std::string						m_Name;
			bbox							m_Bbox;
			int						        m_iMaterialInstance{};
		};

	public:

		/***************************************************************************/
		/*!
		\brief
			Loads the mesh given the data extracted from my descriptor file
		*/
		/**************************************************************************/
		bool Load(const DescriptorData& _DData, SkinGeom* _skinGeom) noexcept;

		/***************************************************************************/
		/*!
		\brief
			Helper functions to manipulate the format of the filepath's string
		*/
		/**************************************************************************/
		std::string getFileNameWithoutExtension(const std::string& ResourceFilePath);
		std::string getFileNameWithExtension(const std::string& ResourceFilePath);

		/***************************************************************************/
		/*!
		\brief
			Getter functions
		*/
		/**************************************************************************/
		std::string& getMeshName() { return m_MeshName; }
	
	private:

		/***************************************************************************/
		/*!
		\brief
			Import the data, and filter out what kind of mesh is being extracted
		*/
		/**************************************************************************/
		bool								ImportGeometryValidateMesh(const aiMesh& AssimpMesh, int& iTexture, int& iColor) noexcept;

		/***************************************************************************/
		/*!
		\brief
			Wrapper function to deserialize and optimize the mesh gotten from asismp.
			Calls internal functions
		*/
		/**************************************************************************/
		void								ImportData();

		/***************************************************************************/
		/*!
		\brief
			Obtains he initial data from the assimp mesh. contains recurseScene
			and processMesh.
			Animation data is being extracted here
		*/
		/**************************************************************************/
		void								ImportStaticData(std::vector<InputMeshPart>& _MyNodes);

		/***************************************************************************/
		/*!
		\brief
			contains m_meshrefs. sanity checks on the type of the incoming mesh
			(the number of channels, whether it is an animation mesh)
			contains processNode
		*/
		/**************************************************************************/
		bool								SanityCheck() noexcept;

		/***************************************************************************/
		/*!
		\brief
			merges the submeshes into one big mesh so we have lesser things to
			de/serialize. 
			makes things neater
		*/
		/**************************************************************************/
		void								MergeData(std::vector<InputMeshPart>& _MyNodes) noexcept;

		/***************************************************************************/
		/*!
		\brief
			casts the compressed nodes into a skingeom
		*/
		/**************************************************************************/
		void								CreateSkinGeom(const std::vector<CompressedMeshPart>& _MyNodes) noexcept;
		
		/***************************************************************************/
		/*!
		\brief
			uses meshoptimizer to optimize the mesh
		*/
		/**************************************************************************/
		void								Optimize(std::vector<InputMeshPart>& _MyNodes) noexcept;
		
		/***************************************************************************/
		/*!
		\brief
			quantizes the data so that it takes up less storage space in binary
			format
		*/
		/**************************************************************************/
		std::vector<CompressedMeshPart> 	Quantize(const std::vector<InputMeshPart>& _MyNodes) noexcept;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//	BONES BONES BONES BONES BONES BONES BONES BONES BONES BONES BONES BONES
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/***************************************************************************/
		/*!
		\brief
			loads the material textures such as the filenames of the 
			textures being used
		*/
		/**************************************************************************/
		std::vector<Geom::Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) noexcept;

		/***************************************************************************/
		/*!
		\brief
			Read the hierarchy data. This is for the bone matrix offset
		*/
		/**************************************************************************/
		void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src) noexcept;

		/***************************************************************************/
		/*!
		\brief
			initializes bone data
		*/
		/**************************************************************************/
		void DefaultInitializeVertexBoneData(FullVertices& vertex) noexcept;

		/***************************************************************************/
		/*!
		\brief
			sets the bone data of the vertex according to the id and weights
		*/
		/**************************************************************************/
		void SetVertexBoneData(FullVertices& vertex, int boneID, float weight) noexcept;

		/***************************************************************************/
		/*!
		\brief
			loads the bone data
		*/
		/**************************************************************************/
		void ExtractBoneWeightForVertex(Animation& anims, InputMeshPart& Vertex, const aiMesh& mesh) noexcept;

		/***************************************************************************/
		/*!
		\brief
			read missing bones and populate the bone vector
		*/
		/**************************************************************************/
		void ReadMissingBones(Animation&, const aiAnimation* ) noexcept;


	private:

	/***************************************************************************/
	/*!
	\brief
		the reference of the assimp's aiNode. this is for the sanity check
	*/
	/**************************************************************************/
		struct Refs
		{
			std::vector<const aiNode*>	m_Nodes;
		};

		const aiScene*					m_Scene;
		std::vector<Refs>				m_MeshRefs;
		SkinGeom*						m_SkinGeom;
		aiMatrix4x4						m_DescriptorMatrix;
		std::string						m_MeshName;
	};
}

/***************************************************************************/
/*!
\brief
	loads and serializes all the meshes
*/
/**************************************************************************/
void LoadAndSerializeAllMeshes(_GEOM::DescriptorData& Desc);


#endif // !_GEOM_COMPILER
