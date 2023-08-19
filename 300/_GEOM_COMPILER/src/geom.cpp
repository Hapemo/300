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
				SerializeVec3(outFile, GeomData.m_pSubMesh[i].m_PosCompressionOffset);
				SerializeVec2(outFile, GeomData.m_pSubMesh[i].m_UVCompressionOffset);
			}

			return true;
		}


		bool SerializeVertexPos(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept
		{
			for (unsigned i{}; i < GeomData.m_nVertices; ++i)
			{
				outFile << std::endl;
				SerializeSigned(outFile, GeomData.m_pPos[i].m_QPosition_X);
				SerializeSigned(outFile, GeomData.m_pPos[i].m_QPosition_Y);
				SerializeSigned(outFile, GeomData.m_pPos[i].m_QPosition_Z);
				SerializeSigned(outFile, GeomData.m_pPos[i].m_QPosition_QNormalX);
			}

			return true;	// no errors
		}


		bool SerializeVertexExtra(std::ofstream& outFile, const _GEOM::Geom& GeomData) noexcept
		{
			for (unsigned i{}; i < GeomData.m_nExtras; ++i)
			{
				outFile << std::endl;
				SerializeUnsigned(outFile, GeomData.m_pExtras[i].m_Packed);
				SerializeSigned(outFile, GeomData.m_pExtras[i].m_U);
				SerializeSigned(outFile, GeomData.m_pExtras[i].m_V);
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
			std::string VertexExtraStr;
			std::getline(inFile, VertexExtraStr);
			std::stringstream Stream(VertexExtraStr);

			Stream >> extras[i].m_Packed;
			Stream >> extras[i].m_U;
			Stream >> extras[i].m_V;
		}

		GeomData.m_pExtras = std::move(extras);
		return true;
	}
	
	
	bool ReadVertexPos(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
	{
		ReadUnsigned(inFile, GeomData.m_nVertices);
		std::unique_ptr<_GEOM::Geom::VertexPos[]> pos = std::make_unique<_GEOM::Geom::VertexPos[]>(GeomData.m_nVertices);

		for (unsigned i{}; i < GeomData.m_nVertices; ++i)
		{
			std::string VertexPosStr;
			std::getline(inFile, VertexPosStr);
			std::stringstream Stream(VertexPosStr);

			Stream >> pos[i].m_QPosition_X;
			Stream >> pos[i].m_QPosition_Y;
			Stream >> pos[i].m_QPosition_Z;
			Stream >> pos[i].m_QPosition_QNormalX;
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

			Stream >> ch >> subMesh[i].m_PosCompressionOffset.x >> ch >> subMesh[i].m_PosCompressionOffset.y >> ch >> subMesh[i].m_PosCompressionOffset.z >> ch;
			Stream >> ch >> subMesh[i].m_UVCompressionOffset.x >> ch >> subMesh[i].m_UVCompressionOffset.y >> ch;
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

	
	bool ReadUnsigned(std::ifstream& inFile, std::uint32_t& value) noexcept
	{
		std::string line;
		std::getline(inFile, line);

		size_t colonPos = line.find(':');
		assert(colonPos != std::string::npos);

		std::string unsignedStr = line.substr(colonPos + 1);
		std::istringstream(unsignedStr) >> value;

		if (unsignedStr.empty() && unsignedStr.substr(1).empty()) {
			std::cout << "[ERROR]>> Failed to Read unsigned value\n";
			return false;
		}

		return true;
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