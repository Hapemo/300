/*!*************************************************************************
****
\file			geom.h
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The declarations for the geom struct, and its various sub structs

****************************************************************************
***/

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
	//!< forward declarations
	struct BoneInfo;
	struct AssimpNodeData;
	class Bone;


/***************************************************************************/
/*!
\brief
	All the information that each vertex is required to have
	This contains the full information
*/
/**************************************************************************/
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
	
/***************************************************************************/
/*!
\brief
	the bounding box of the model, in model space
*/
/**************************************************************************/
	struct bbox
	{
		glm::vec3 m_Min{};
		glm::vec3 m_Max{};

	public:
		void AddVerts(const glm::vec3* pVerts) noexcept;
		void MergeBBOX(const bbox& inputbox) noexcept;
		glm::vec3 getSize() const noexcept;
		glm::vec3 getCenter() const noexcept;
	};
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/*!
\brief
	the overall geom struct that contains everything. this is what will
	get de/serialized
*/
/**************************************************************************/
	struct Geom
	{

	/***************************************************************************/
	/*!
	\brief
		The path of the texture in the model loaded from assimp
	*/
	/**************************************************************************/
		struct Texture
		{
			//unsigned int id;
			std::string type;
			std::string path;
		};

	/***************************************************************************/
	/*!
	\brief
		The scaled down version of what each struct is required to have
	*/
	/**************************************************************************/
		struct alignas(std::uint32_t) VertexPos
		{
			glm::vec3			m_Pos;
			glm::vec3			m_Clr;
			glm::vec2			m_UV;

			int					m_BoneIDs[MAX_BONE_INFLUENCE];	// bone indices which will influence this vertex
			float				m_Weights[MAX_BONE_INFLUENCE];	// weights from each bone
		};

	/***************************************************************************/
	/*!
	\brief
		The extra stuff that the vertex has. This is stored in a different
		struct for modularity
	*/
	/**************************************************************************/
		struct alignas(std::uint32_t) VertexExtra
		{
			glm::vec3 m_Normal;
			glm::vec3 m_Tangent;
		};


	/***************************************************************************/
	/*!
	\brief
		The data that each mesh contains.
		This is unique to one per mesh, as what was intended by the model
	*/
	/**************************************************************************/
		struct Mesh
		{
			std::array<char, 64>		m_name{};			// Name of mesh
			std::vector<Animation>		m_Animation;		// Animation data of the mesh
			std::vector<Geom::Texture>	m_Texture;			// The texture that the mesh contains
			bbox						m_MeshBBOX;
		};

	/***************************************************************************/
	/*!
	\brief
		The data of each submeshes should there be any. Such as the starting
		vertex and index positions, and the total number of each
	*/
	/**************************************************************************/
		struct SubMesh
		{
			std::uint32_t			m_nFaces;			//Total number of faces(triangles) in submesh
			std::uint32_t			m_iIndices;			//Index of indices in submesh
			std::uint32_t			m_nIndices;			//Total number of indices for this submesh
			std::uint32_t			m_iVertices;		//Index of vertices in submesh
			std::uint32_t			m_nVertices;		//Total number of vertices for this submesh
			std::uint16_t			m_iMaterial;		//Index of material in submesh
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

	/***************************************************************************/
	/*!
	\brief
		SerializeGeom()
		This function is used to serialize the geom struct into binary data
	*/
	/**************************************************************************/
		static bool SerializeGeom(const std::string& filename, Geom& GeomData) noexcept;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/***************************************************************************/
	/*!
	\brief
		The skingeom is the precursor to the geom struct. It stores the data
		required to be serialized in the end
	*/
	/**************************************************************************/
	struct SkinGeom
	{

	/***************************************************************************/
	/*!
	\brief
		Contains the data for the mesh. This contains the elements that each
		vertex would need to render
	*/
	/**************************************************************************/
		struct Submesh
		{	
			std::vector<Geom::VertexPos>				m_Vertex;
			std::vector<Geom::VertexExtra>				m_Extra;
			std::vector<uint32_t>						m_Indices;
			int m_iMaterial{};
		};

	/***************************************************************************/
	/*!
	\brief
		The data unique to the mesh. Such as the name, the vector of submeshes
		if there should be more than one, as well as the bounding box that 
		encompasses the model
	*/
	/**************************************************************************/
		struct Mesh
		{
			std::string									m_Name;
			std::vector<Submesh>						m_Submeshes;
			bbox										m_MeshBBOX;
		};

		std::string										m_Filename;
		std::vector<Mesh>								m_Meshes;

		std::vector<Animation>							m_Animation;
		std::vector<Geom::Texture>						m_Textures;

		bool											m_bHasTextures = false;	
		bool											m_bHasAnimation = false;	
		bool											m_bVtxClrPresent = false;

	/***************************************************************************/
	/*!
	\brief
		Casts the data stored within skingeom into the final geom struct, that 
		is then serialized
	*/
	/**************************************************************************/
		void CastToGeomStruct(Geom& _geom) noexcept;
	};
}


namespace Serialization
{

#pragma region depreceated, these functions serializes in string format
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
#pragma endregion

}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v);
std::ostream& operator<<(std::ostream& os, const glm::vec3& v);

std::ifstream& operator>>(std::ifstream& is, glm::vec2& v);
std::ifstream& operator>>(std::ifstream& is, glm::vec3& v);

#endif // !_GEOM_H