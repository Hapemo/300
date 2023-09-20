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
	class Mesh_Loader
	{
	public:
		struct InputMeshPart
		{
			std::string						m_MeshName;
			std::string						m_Name;
			std::vector<FullVertices>		m_Vertices;
			std::vector<Geom::Texture>		m_Textures;						
			std::vector<uint32_t>			m_Indices;
			int								m_iMaterialInstance{};
		};

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
		bool Load(const DescriptorData& _DData, SkinGeom* _skinGeom) noexcept;

		// Helper
		std::string getFileNameWithoutExtension(const std::string& ResourceFilePath);
		std::string getFileNameWithExtension(const std::string& ResourceFilePath);

		// Getters
		std::string& getMeshName() { return m_MeshName; }
	
	private:
		bool								ImportGeometryValidateMesh(const aiMesh& AssimpMesh, int& iTexture, int& iColor) noexcept;
		void								ImportData();
		void								ImportStaticData(std::vector<InputMeshPart>& _MyNodes);
		bool								SanityCheck() noexcept;
		void								MergeData(std::vector<InputMeshPart>& _MyNodes) noexcept;
		void								CreateSkinGeom(const std::vector<CompressedMeshPart>& _MyNodes) noexcept;
		void								Optimize(std::vector<InputMeshPart>& _MyNodes) noexcept;
		std::vector<CompressedMeshPart> 	Quantize(const std::vector<InputMeshPart>& _MyNodes) noexcept;

		// BONE::
		std::vector<Geom::Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) noexcept;
		void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src) noexcept;
		void DefaultInitializeVertexBoneData(FullVertices& vertex) noexcept;
		void SetVertexBoneData(FullVertices& vertex, int boneID, float weight) noexcept;
		void ExtractBoneWeightForVertex(Animation& anims, InputMeshPart& Vertex, const aiMesh& mesh) noexcept;
		void ReadMissingBones(Animation&, const aiAnimation* ) noexcept;


	private:
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

void LoadAndSerializeAllMeshes(_GEOM::DescriptorData& Desc);


#endif // !_GEOM_COMPILER
