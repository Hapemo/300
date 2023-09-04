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
			std::vector<Geom::Texture>		m_Textures;						// we have not used this in _rendering
			std::vector<uint32_t>			m_Indices;
			int								m_iMaterialInstance{};
		};

		struct CompressedMeshPart
		{
			std::string						m_MeshName; 
			std::string						m_Name;
			std::vector<Geom::VertexPos>	m_Vertex;
			std::vector<Geom::VertexExtra>	m_Extra;
			std::vector<Geom::Texture>		m_Textures;						// we have not used this in _rendering
			std::vector<uint32_t>			m_Indices;
			int						        m_iMaterialInstance{};
		};

	public:
		bool Load(const DescriptorData& _DData, SkinGeom* _skinGeom) noexcept;
	
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
		void ExtractBoneWeightForVertex(InputMeshPart& Vertex, const aiMesh& mesh) noexcept;
		void ReadMissingBones(const aiAnimation* animation) noexcept;


	private:
		struct Refs
		{
			std::vector<const aiNode*>	m_Nodes;
		};

		const aiScene*					m_Scene;
		std::vector<Refs>				m_MeshRefs;
		SkinGeom*						m_SkinGeom;
		aiMatrix4x4						m_DescriptorMatrix;
	};
}


#endif // !_GEOM_COMPILER
