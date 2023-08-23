#pragma once

#ifndef _GEOM_COMPILER
#define _GEOM_COMPILER

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include <descriptor.h>
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
			std::vector<uint32_t>			m_Indices;
			int								m_iMaterialInstance;
			std::vector<Geom::Texture>		m_Textures;
		};

		struct CompressedMeshPart
		{
			std::string						m_MeshName;
			std::string						m_Name;
			std::vector<Geom::VertexPos>	m_Pos;
			std::vector<Geom::VertexExtra>	m_Extra;
			std::vector<uint32_t>			m_Indices;
			int						        m_iMaterialInstance;
		};

	public:
		bool Load(const DescriptorData& _DData, SkinGeom* _skinGeom) noexcept;

		//glm::vec3							m_PosCompressionScale;
		//glm::vec2							m_UVCompressionScale;
	
	private:
		bool								ImportGeometryValidateMesh(const aiMesh& AssimpMesh, int& iTexture, int& iColor) noexcept;
		void								ImportData();
		void								ImportStaticData(std::vector<InputMeshPart>& _MyNodes);
		bool								SanityCheck() noexcept;
		void								MergeData(std::vector<InputMeshPart>& _MyNodes) noexcept;
		void								CreateSkinGeom(const std::vector<CompressedMeshPart>& _MyNodes) noexcept;
		void								Optimize(std::vector<InputMeshPart>& _MyNodes) noexcept;
		std::vector<CompressedMeshPart> 	Quantize(const std::vector<InputMeshPart>& _MyNodes) noexcept;

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
