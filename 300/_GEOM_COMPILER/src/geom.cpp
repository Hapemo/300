#include <geom.h>
#include <iostream>
#include <sstream>

namespace _GEOM
{
	// considers the vertices, and returns the bounding box with all the vertices inside it
	bbox bbox::AddVerts(const glm::vec3* pVerts, std::int32_t nVerts) noexcept
	{
		assert(pVerts != nullptr);
		assert(nVerts > 0);

		for (std::int32_t i{}; i < nVerts; ++i)
		{
			m_Min = glm::vec3
			(
				glm::min(m_Min.x, pVerts[i].x),
				glm::min(m_Min.y, pVerts[i].y),
				glm::min(m_Min.z, pVerts[i].z)
			);

			m_Max = glm::vec3
			(
				glm::max(m_Max.x, pVerts[i].x),
				glm::max(m_Max.y, pVerts[i].y),
				glm::max(m_Max.z, pVerts[i].z)
			);
		}

		return *this;
	}


	glm::vec3 bbox::getSize() const noexcept
	{
		return m_Max - m_Min;
	}


	glm::vec3 bbox::getCenter() const noexcept
	{
		return (m_Min + m_Max) * 0.5f;
	}

}

/// ///////////////////////////////////////////////////////////////////////////////////////////

namespace Serialization
{

		// ====================================================================================================
		//										NEW SERIALIZATION CODE BLOCK
		// ====================================================================================================

		bool SerializeUnsigned(std::ofstream& outFile, const std::uint32_t& value) noexcept
		{
			outFile << value << " ";
			return true;	// no error
		}


		bool SerializeSigned(std::ofstream& outFile, const std::int16_t& value) noexcept
		{
			outFile << value << " ";
			return true;	// no error
		}


		bool SerializeVec3(std::ofstream& outFile, const glm::vec3& value) noexcept
		{
			outFile << "{ " << value.x << ", " << value.y << ", " << value.z << " }" << " ";
			return true;	// no error
		}

		bool SerializeVec2(std::ofstream& outFile, const glm::vec2& value) noexcept
		{
			outFile << "{ " << value.x << ", " << value.y << " }" << " ";
			return true;	// no error
		}


		bool SerializeMesh(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept
		{
			outFile << std::endl;
			for (unsigned i{}; i < GeomData.m_nMeshes; ++i)
			{
				std::string MeshName(GeomData.m_pMesh[i].m_name.data(), GeomData.m_pMesh[i].m_name.size());
				if (MeshName[0] == '\0') {
					outFile << "DEFAULT" << ", ";
					continue;
				}
				outFile << MeshName << ", ";
			}

			return true;	// no errors
		}


		bool SerializeSubMesh(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept
		{
			for (unsigned i{}; i < GeomData.m_nSubMeshes; ++i)
			{
				outFile << std::endl;
				SerializeUnsigned(outFile, GeomData.m_pSubMesh[i].m_nFaces);
				SerializeUnsigned(outFile, GeomData.m_pSubMesh[i].m_iIndices);
				SerializeUnsigned(outFile, GeomData.m_pSubMesh[i].m_nIndices);
				SerializeUnsigned(outFile, GeomData.m_pSubMesh[i].m_iVertices);
				SerializeUnsigned(outFile, GeomData.m_pSubMesh[i].m_nVertices);
				SerializeUnsigned(outFile, GeomData.m_pSubMesh[i].m_iMaterial);
			}

			return true;
		}


		bool SerializeVertexPos(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept
		{
			// Vertex color present
			if(GeomData.m_bVtxClrPresent)
				outFile << " VTXCLR_YES";
			else
				outFile << " VTXCLR_NO";

			for (unsigned i{}; i < GeomData.m_nVertices; ++i)
			{
				outFile << std::endl;
				SerializeVec3(outFile, GeomData.m_pPos[i].m_Pos);
				SerializeVec2(outFile, GeomData.m_pPos[i].m_UV);

				if (GeomData.m_bVtxClrPresent) {
					SerializeVec3(outFile, GeomData.m_pPos[i].m_Clr);
				}
			}

			return true;	// no errors
		}


		bool SerializeVertexExtra(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept
		{
			for (unsigned i{}; i < GeomData.m_nExtras; ++i)
			{
				outFile << std::endl;
				SerializeVec3(outFile, GeomData.m_pExtras[i].m_Normal);
				SerializeVec3(outFile, GeomData.m_pExtras[i].m_Tangent);
			}

			return true;
		}


		bool SerializeIndices(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept
		{
			outFile << std::endl;
			for (unsigned i{}; i < GeomData.m_nIndices; ++i)
			{
				SerializeUnsigned(outFile, GeomData.m_pIndices[i]);
			}
			return true;
		}


		bool SerializeQuaternion(std::ofstream& outFile, const glm::quat& value) noexcept
		{
			return false;
		}


		bool SerializeAnimation(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept
		{
			bool Err{};

			assert(GeomData.m_bHasAnimations);

			for (unsigned meshindex{}; meshindex < GeomData.m_nMeshes; ++meshindex)
			{
				// How many meshes are there? Each mesh has a bone info map
				Err |= SerializeBoneInfoMap(outFile, GeomData.m_pMesh[meshindex].m_Animation);
			}

			return Err;
		}


		bool SerializeBoneInfoMap(std::ofstream& outFile, const _GEOM::Animation animation) noexcept
		{
			outFile << " | Bone_Count: " << animation.m_BoneCounter << " | ";
			outFile << "m_Duration: " << animation.m_Duration << " | ";
			outFile << "m_TicksPerSecond: " << animation.m_TicksPerSecond << " | ";
			outFile << "Bone_Info_Map_Size: " << animation.m_BoneInfoMap.size() << "\n";

			// in this particular mesh, extract and serialize the bone info map
			for (const auto lBoneInfo : animation.m_BoneInfoMap)
			{
				// serialize the bone name and the bone id
				outFile << lBoneInfo.first << " " << lBoneInfo.second.id << " | ";
			}

			// serialize the offset mat4x4, in row major order
			for (unsigned rows{}; rows < 4; ++rows)
			{
				outFile << std::endl;
				for (const auto lBoneInfo : animation.m_BoneInfoMap)
				{
					outFile << "{" << lBoneInfo.second.offset[0][rows] << " " << lBoneInfo.second.offset[1][rows] << " " << lBoneInfo.second.offset[2][rows] << " " << lBoneInfo.second.offset[3][rows] << "} ";
				}
			}

			return true;
		}



	// ====================================================================================================
	//										NEW DESERIALIZATION CODE BLOCK
	// ====================================================================================================
	bool ReadIndices(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
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

	
	bool ReadVertexExtra(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
	{
		ReadUnsigned(inFile, GeomData.m_nExtras);
		std::unique_ptr<_GEOM::Geom::VertexExtra[]> extras = std::make_unique<_GEOM::Geom::VertexExtra[]>(GeomData.m_nExtras);

		for (unsigned i{}; i < GeomData.m_nExtras; ++i)
		{
			char ch;
			std::string VertexExtraStr;
			std::getline(inFile, VertexExtraStr);
			std::stringstream Stream(VertexExtraStr);

			Stream >> ch >> extras[i].m_Normal.x;
			Stream >> ch >> extras[i].m_Normal.y;
			Stream >> ch >> extras[i].m_Normal.z >> ch;

			Stream >> ch >> extras[i].m_Tangent.x;
			Stream >> ch >> extras[i].m_Tangent.y;
			Stream >> ch >> extras[i].m_Tangent.z;
		}

		GeomData.m_pExtras = std::move(extras);
		return true;
	}
	
	
	bool ReadVertexPos(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
	{
		std::string check;
		check = ReadUnsigned(inFile, GeomData.m_nVertices);
		if(check.find("VTXCLR_YES") != std::string::npos)
			GeomData.m_bVtxClrPresent = true;
		else
			GeomData.m_bVtxClrPresent = false;

		std::unique_ptr<_GEOM::Geom::VertexPos[]> pos = std::make_unique<_GEOM::Geom::VertexPos[]>(GeomData.m_nVertices);
		for (unsigned i{}; i < GeomData.m_nVertices; ++i)
		{
			char ch;
			std::string VertexPosStr;
			std::getline(inFile, VertexPosStr);
			std::stringstream Stream(VertexPosStr);

			Stream >> ch >> pos[i].m_Pos.x;
			Stream >> ch >> pos[i].m_Pos.y;
			Stream >> ch >> pos[i].m_Pos.z >> ch;

			Stream >> ch >> pos[i].m_UV.x;
			Stream >> ch >> pos[i].m_UV.y >> ch;

			// only serialize the vertex color if it is present
			if (GeomData.m_bVtxClrPresent)
			{
				Stream >> ch >> pos[i].m_Clr.x;
				Stream >> ch >> pos[i].m_Clr.y;
				Stream >> ch >> pos[i].m_Clr.z;
			}
		}

		GeomData.m_pPos = std::move(pos);
		return true;
	}
	
	
	bool ReadSubMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
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
		}

		GeomData.m_pSubMesh = std::move(subMesh);
		return true;
	}
	
	
	bool ReadMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
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

	
	std::string ReadUnsigned(std::ifstream& inFile, std::uint32_t& value) noexcept
	{
		std::string line;
		std::getline(inFile, line);

		size_t colonPos = line.find(':');
		assert(colonPos != std::string::npos);

		std::string unsignedStr = line.substr(colonPos + 1);
		std::istringstream ss(unsignedStr);
		ss >> value;

		if (unsignedStr.empty() && unsignedStr.substr(1).empty()) {
			std::cout << "[ERROR]>> Failed to Read unsigned value\n";
			assert(false);
		}

		return ss.str();
	}


	bool ReadSigned(std::ifstream& inFile, std::int16_t& value) noexcept
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


	bool ReadVec3WithHeader(std::ifstream& inFile, glm::vec3& value) noexcept
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


	bool ReadVec2WithHeader(std::ifstream& inFile, glm::vec2& value) noexcept
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

}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v)
{
	os << "x: " << v.x << " y: " << v.y << "\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
{
	os << "x: " << v.x << " y: " << v.y << " z: " << v.z << "\n";
	return os;
}

std::ifstream& operator>>(std::ifstream& is, glm::vec2& v)
{
	is >> v.x >> v.y;
	return is;
}

std::ifstream& operator>>(std::ifstream& is, glm::vec3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}