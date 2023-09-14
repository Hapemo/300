#pragma once

#ifndef _GEOM_H
#define _GEOM_H

#define MAX_BONE_INFLUENCE 4

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <array>
#include <string>
#include <vector>
#include <fstream>

#include <Animation.h>
#include <descriptor.h>

namespace _GEOM
{
	struct BoneInfo;
	struct AssimpNodeData;
	class Bone;

	struct FullVertices
	{
		glm::vec4			m_Color;			//Color of vertex
		glm::vec3			m_Position;			//Position of vertex
		glm::vec2			m_Texcoord;			//UVs of vertex
		glm::vec3			m_fNormal;			//Normal of vertex
		glm::vec3			m_fTangent;			//Tangent of vertex
		glm::vec3			m_fBitangent;		//Bi-tangent of vertex

		int					m_BoneIDs[MAX_BONE_INFLUENCE];	// bone indices which will influence this vertex
		float				m_Weights[MAX_BONE_INFLUENCE];	// weights from each bone
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
			glm::vec3			m_Pos;
			glm::vec3			m_Clr;
			glm::vec2			m_UV;

			int					m_BoneIDs[MAX_BONE_INFLUENCE];	// bone indices which will influence this vertex
			float				m_Weights[MAX_BONE_INFLUENCE];	// weights from each bone
		};

		struct alignas(std::uint32_t) VertexExtra
		{
			glm::vec3 m_Normal;
			glm::vec3 m_Tangent;
		};

		struct Mesh
		{
			std::array<char, 64>	m_name;				// Name of mesh
			std::vector<Animation>	m_Animation;		// Animation data of the mesh
		};

		struct SubMesh
		{
			std::uint32_t			m_nFaces;			//Total number of faces(triangles) in submesh
			std::uint32_t			m_iIndices;			//Index of indices in submesh
			std::uint32_t			m_nIndices;			//Total number of indices for this submesh
			std::uint32_t			m_iVertices;		//Index of vertices in submesh
			std::uint32_t			m_nVertices;		//Total number of vertices for this submesh
			std::uint16_t			m_iMaterial;		//Index of material in submesh
		};

		// TODO:: add texture path de/serialization from fbx
		struct Texture
		{
			//unsigned int id;
			std::string type;
			std::string path;
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

		bool					m_bHasTextures = false;		// does the mesh contain textures
		bool					m_bHasAnimations = false;
		bool					m_bVtxClrPresent = false;	// when this boolean is false, it means that the vertex colors are not present within the mesh.
															// defaults to (1, 1, 1, 1)

		static bool SerializeGeom(const std::string& filename, Geom& GeomData) noexcept;
		//static bool DeserializeGeom(const std::string Filepath, Geom& GeomRef) noexcept;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct SkinGeom
	{
		struct Submesh
		{	
			std::vector<Geom::VertexPos>				m_Vertex;
			std::vector<Geom::VertexExtra>				m_Extra;
			std::vector<uint32_t>						m_Indices;
			int m_iMaterial;
		};

		struct Mesh
		{
			std::string									m_Name;
			std::vector<Submesh>						m_Submeshes;
		};

		std::string										m_Filename;
		std::vector<Mesh>								m_Meshes;

		// TODO:
		// because skingeom has no concept of what mesh contains which animations, we have to change this
		// to start storing data inside the mesh struct.
		// Either skingeom puts animation into the mesh, or geomdata takes animation out of the mesh.
		std::vector<Animation>							m_Animation;	

		bool											m_bHasTextures = false;		// TODO:: this has not been fully implemented yet. 
		bool											m_bHasAnimation = false;	
		bool											m_bVtxClrPresent = false;

		void CastToGeomStruct(Geom& _geom) noexcept;
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
	bool SerializeQuaternion(std::ofstream& outFile, const glm::quat& value) noexcept;	// {0.0, 0.0, 0.0, 0.0}
	bool SerializeVec3(std::ofstream& outFile, const glm::vec3& value) noexcept;	// {0.0, 0.0, 0.0}
	bool SerializeVec2(std::ofstream& outFile, const glm::vec2& value) noexcept;	// {0.0, 0.0}

	bool SerializeMesh(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;
	bool SerializeSubMesh(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;
	bool SerializeVertexPos(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;
	bool SerializeVertexExtra(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;
	bool SerializeIndices(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;	
	bool SerializeAnimation(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept;
	bool SerializeBoneInfoMap(std::ofstream& outFile, const _GEOM::Animation animation) noexcept;

	void SerializeAssimpNodeData(std::ofstream& outfile, const _GEOM::AssimpNodeData& Node);

	std::string ReadUnsigned(std::ifstream& inFile, std::uint32_t& value) noexcept;
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