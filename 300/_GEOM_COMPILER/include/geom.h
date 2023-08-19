#pragma once

#ifndef _GEOM_H
#define _GEOM_H

#include <glm/glm.hpp>
#include <array>
#include <string>
#include <vector>
#include <fstream>

#include <descriptor.h>

namespace _GEOM
{
	class SharedVertexPos;

	struct FullVertices
	{
		glm::vec3			m_Position;			//Position of vertex
		glm::vec2			m_Texcoord;			//UVs of vertex
		glm::vec3			m_fNormal;			//Normal of vertex
		glm::vec3			m_fTangent;			//Tangent of vertex
		glm::vec3			m_fBitangent;		//Bi-tangent of vertex
		glm::ivec4			m_Tangent;			//Tangent of vertex
		glm::ivec4			m_Bitangent;		//Bi-tangent of vertex
		glm::ivec4			m_Normal;			//Normal of vertex
		glm::ivec4			m_Color;			//Color of vertex
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	struct bbox
	{
		glm::vec3 m_Min{};
		glm::vec3 m_Max{};

	public:
		bbox AddVerts(const glm::vec3* pVerts, std::int32_t nVerts) noexcept;
		glm::vec3 getSize() const noexcept;
		glm::vec3 getCenter() const noexcept;
	};
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct Geom
	{

		struct alignas(std::uint32_t) VertexPos
		{
			std::int16_t    m_QPosition_X		// All these go together they are represented by
				, m_QPosition_Y					// SINT16_4D_NORMALIZED
				, m_QPosition_Z					//
				, m_QPosition_QNormalX;			// The normal X dimension plus a sign bit
		};

		struct alignas(std::uint32_t) VertexExtra
		{
			union
			{
				struct
				{
					std::uint32_t   m_QNormalY : 10		// This contains two signs bits (For the Tangent.z, and the Bitangent direction)
						, m_QTangentX : 10				// All these guys go together, they are represented by
						, m_QTangentY : 10				// SINT_A2RGB10_4D_NORMALIZED
						, m_QAlpha : 2;
				};
				std::uint32_t m_Packed;					// Packed union
			};

			std::int16_t    m_U							// These two go together, they are represented by
				, m_V;									// SINT16_2D_NORMALIZED
		};

		struct Mesh
		{
			std::array<char, 64>	m_name;				//Name of mesh
			//std::uint16_t			m_nSubMeshes;		//Total number of submeshes in mesh
			//std::uint16_t			m_iSubMesh;			//Index of submesh
			//std::uint16_t			m_nLODs{ 1 };		//Total number of LODs in mesh
			//std::uint16_t			m_iLOD{ 1 };		//Index of LOD of mesh
		};

		struct SubMesh
		{
			std::uint32_t			m_nFaces;			//Total number of faces(triangles) in submesh
			std::uint32_t			m_iIndices;			//Index of indices in submesh
			std::uint32_t			m_nIndices;			//Total number of indices for this submesh
			std::uint32_t			m_iVertices;		//Index of vertices in submesh
			std::uint32_t			m_nVertices;		//Total number of vertices for this submesh
			std::uint16_t			m_iMaterial;		//Index of material in submesh
			glm::vec3				m_PosCompressionOffset;
			glm::vec2				m_UVCompressionOffset;
		};

	public:
		std::shared_ptr<Mesh[]>				m_pMesh;		//Pointer to Mesh
		std::shared_ptr<SubMesh[]>			m_pSubMesh;		//Pointer to SubMesh
		std::shared_ptr<VertexPos[]>		m_pPos;			//Pointer to Position
		std::shared_ptr<VertexExtra[]>		m_pExtras;		//Pointer to Extras
		std::shared_ptr<std::uint32_t[]>	m_pIndices;		//Pointer to Indices

		std::uint32_t			m_nMeshes;
		std::uint32_t			m_nSubMeshes;
		std::uint32_t			m_nVertices;
		std::uint32_t			m_nExtras;
		std::uint32_t			m_nIndices;
		glm::vec3				m_PosCompressionScale;
		glm::vec2				m_UVCompressionScale;

		static bool SerializeGeom(const std::string& filename, const DescriptorData& Desc, Geom& GeomData) noexcept;
		static bool DeserializeGeom(const std::string Filepath, Geom& GeomRef) noexcept;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct SkinGeom
	{
		struct Submesh
		{	
			std::vector<Geom::VertexPos>	m_Pos;
			std::vector<Geom::VertexExtra>	m_Extra;
			std::vector<uint32_t>			m_Indices;
			int m_iMaterial;
			glm::vec3						m_PosCompressionOffset;
			glm::vec2						m_UVCompressionOffset;
		};

		struct Mesh
		{
			std::string						m_Name;
			std::vector<Submesh>			m_Submeshes;
		};

		std::string							m_Filename;
		std::vector<Mesh>					m_Meshes;

		void								CastToGeomStruct(Geom& _geom, const glm::vec3& posScaleDecompress, const glm::vec2& UVScaleDecompress) noexcept;
	};
}


namespace Serialization
{
	/*
		FORMAT:
		[Mesh Data Number]: 5		// for unsigned int
		[Mesh Data]: 0 1 2 3 4		// for unsigned int array
		[Mesh Name]: Mesh001
		[Offset]: {0.0, 0.0, 0.0}	// for vec3
		[Submesh Size]: 55			// for shared_ptr, we need to serialize the size too
		 1 2 3 4 5 6 {0.0, 0.0, 0.0} {0.0, 0.0}
		 1 2 3 4 5 6 {0.0, 0.0, 0.0} {0.0, 0.0}
		 ...

	*/
	bool SerializeUnsigned(std::ofstream& outFile, const std::uint32_t& value) noexcept;
	bool SerializeSigned(std::ofstream& outFile, const std::int16_t& value) noexcept;
	bool SerializeVec3(std::ofstream& outFile, const glm::vec3& value) noexcept;	// {0.0, 0.0, 0.0}
	bool SerializeVec2(std::ofstream& outFile, const glm::vec2& value) noexcept;	// {0.0, 0.0}
	bool SerializeMesh(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;
	bool SerializeSubMesh(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;
	bool SerializeVertexPos(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;
	bool SerializeVertexExtra(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;
	bool SerializeIndices(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;	

	bool ReadUnsigned(std::ifstream& inFile, std::uint32_t& value) noexcept;
	bool ReadSigned(std::ifstream& inFile, std::int16_t& value) noexcept;
	bool ReadVec3WithHeader(std::ifstream& inFile, glm::vec3& value) noexcept;
	bool ReadVec2WithHeader(std::ifstream& inFile, glm::vec2& value) noexcept;
	bool ReadMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
	bool ReadSubMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
	bool ReadVertexPos(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
	bool ReadVertexExtra(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
	bool ReadIndices(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v);
std::ostream& operator<<(std::ostream& os, const glm::vec3& v);

std::ifstream& operator>>(std::ifstream& is, glm::vec2& v);
std::ifstream& operator>>(std::ifstream& is, glm::vec3& v);

#endif // !_GEOM_H