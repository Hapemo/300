//#include <Serialization.h>
//#include <iostream>
//
//namespace Serialization
//{
//	template <typename T>
//	bool SerializeValue(std::ofstream& outFile, const T& value) noexcept
//	{
//		//outFile.write(reinterpret_cast<const char*>(&value), sizeof(T));
//		outFile << value;
//		return false;
//	}
//
//
//	template <typename T>
//	bool DeserializeValue(std::ifstream& inFile, T& value) noexcept
//	{
//		//inFile.read(reinterpret_cast<char*>(&value), sizeof(T));
//		inFile >> value;
//		return false;
//	}
//
//
//	template <typename T>
//	bool DEBUGSerializeValue(std::ofstream& outFile, const T& value) noexcept
//	{
//		std::cout << "serializing: " << value << std::endl;
//		outFile << value;
//		//outFile.write(reinterpret_cast<const char*>(&value), sizeof(T));
//		return false;
//	}
//
//
//	template <typename T>
//	bool DEBUGDeserializeValue(std::ifstream& inFile, T& value) noexcept
//	{
//		std::cout << "deserializing: " << value << std::endl;
//		inFile >> value;
//		//inFile.read(reinterpret_cast<char*>(&value), sizeof(T));
//		return false;
//	}
//
//
//	template <typename T, std::size_t N>
//	bool SerializeArray(std::ofstream& outFile, const std::array<T, N>& arr) noexcept
//	{
//		for (const auto& elem : arr)
//		{
//			if (!SerializeValue(outFile, elem))
//				return true;
//		}
//		return false;
//	}
//
//
//	bool SerializeUnsigned(std::ofstream& outFile, const std::uint32_t& value) noexcept
//	{
//		outFile << value;
//		return false;
//	}
//
//	bool SerializeVertexPos(std::ofstream& outFile, const _GEOM::Geom::VertexPos* const vertexPos, std::uint32_t length) noexcept
//	{
//		// Serialize each member of VertexPos struct
//		for (std::uint32_t i = 0; i < length; ++i)
//		{
//			if (SerializeValue(outFile, vertexPos[i].m_QPosition_X) ||
//				SerializeValue(outFile, vertexPos[i].m_QPosition_Y) ||
//				SerializeValue(outFile, vertexPos[i].m_QPosition_Z) ||
//				SerializeValue(outFile, vertexPos[i].m_QPosition_QNormalX))
//				assert(false);
//		}
//
//		return false;
//	}
//
//
//	bool SerializeVertexExtra(std::ofstream& outFile, const _GEOM::Geom::VertexExtra* const vertexExtra, std::uint32_t length) noexcept
//	{
//		// Serialize each member of VertexExtra struct
//		for (std::uint32_t i = 0; i < length; ++i)
//		{
//			if (DEBUGSerializeValue(outFile, vertexExtra[i].m_Packed) ||
//				DEBUGSerializeValue(outFile, vertexExtra[i].m_U) ||
//				DEBUGSerializeValue(outFile, vertexExtra[i].m_V))
//				assert(false);
//		}
//
//		return false;
//	}
//
//
//	bool SerializeSubMesh(std::ofstream& outFile, const _GEOM::Geom::SubMesh* const subMesh, std::uint32_t length) noexcept
//	{
//		// Serialize each member of SubMesh struct
//		for (size_t i{}; i < length; ++i)
//		{
//			if (DEBUGSerializeValue(outFile, subMesh[i].m_nFaces) ||
//				DEBUGSerializeValue(outFile, subMesh[i].m_iIndices) ||
//				DEBUGSerializeValue(outFile, subMesh[i].m_nIndices) ||
//				DEBUGSerializeValue(outFile, subMesh[i].m_iVertices) ||
//				DEBUGSerializeValue(outFile, subMesh[i].m_nVertices) ||
//				DEBUGSerializeValue(outFile, subMesh[i].m_iMaterial) ||
//				DEBUGSerializeValue(outFile, subMesh[i].m_PosCompressionOffset) ||
//				DEBUGSerializeValue(outFile, subMesh[i].m_UVCompressionOffset))
//				assert(false);
//		}
//
//		return false;
//	}
//
//
//	bool SerializeIndices(std::ofstream& outFile, const std::uint32_t* const indices, std::uint32_t length) noexcept
//	{
//		// Serialize each Index
//		for (std::uint32_t i = 0; i < length; ++i)
//		{
//			if (DEBUGSerializeValue(outFile, indices[i]))
//				assert(false);
//		}
//
//		return false;
//	}
//
//
//	bool SerializeMesh(std::ofstream& outFile, const _GEOM::Geom::Mesh& const mesh) noexcept
//	{
//		// Serialize the array and other members of Mesh struct
//		return SerializeArray(outFile, mesh.m_name);
//	}
//
//
//	template <typename T, std::size_t N>
//	bool DeserializeArray(std::ifstream& inFile, std::array<T, N>& arr) noexcept
//	{
//		for (auto& elem : arr)
//		{
//			if (DeserializeValue(inFile, elem))
//				return true;
//		}
//		return false;
//	}
//
//
//	bool DeserializeVertexPos(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
//	{
//		std::unique_ptr<_GEOM::Geom::VertexPos[]> vertexPos = std::make_unique<_GEOM::Geom::VertexPos[]>(GeomData.m_nVertices);
//
//		// Deserialize each member of VertexPos struct
//		for (std::uint32_t i{}; i < GeomData.m_nVertices; ++i)
//		{
//			if (DeserializeValue(inFile, vertexPos[i].m_QPosition_X) ||
//				DeserializeValue(inFile, vertexPos[i].m_QPosition_Y) ||
//				DeserializeValue(inFile, vertexPos[i].m_QPosition_Z) ||
//				DeserializeValue(inFile, vertexPos[i].m_QPosition_QNormalX))
//				assert(false);
//		}
//
//		GeomData.m_pPos = std::move(vertexPos);
//		return false;
//	}
//
//
//	bool DeserializeVertexExtra(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
//	{
//		std::unique_ptr<_GEOM::Geom::VertexExtra[]> vertexExtra = std::make_unique<_GEOM::Geom::VertexExtra[]>(GeomData.m_nExtras);
//
//		// Deserialize each member of VertexExtra struct
//		for (std::uint32_t i = 0; i < GeomData.m_nExtras; ++i)
//		{
//			if (DEBUGDeserializeValue(inFile, vertexExtra[i].m_Packed) ||
//				DEBUGDeserializeValue(inFile, vertexExtra[i].m_U) ||
//				DEBUGDeserializeValue(inFile, vertexExtra[i].m_V))
//				assert(false);
//		}
//		GeomData.m_pExtras = std::move(vertexExtra);
//		return false;
//	}
//
//
//	bool DeserializeIndices(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
//	{
//		std::unique_ptr<std::uint32_t[]> indices = std::make_unique<std::uint32_t[]>(GeomData.m_nIndices);
//		for (std::uint32_t i{}; i < GeomData.m_nIndices; ++i)
//		{
//			if (DEBUGDeserializeValue(inFile, indices[i]))
//				assert(false);
//		}
//		GeomData.m_pIndices = std::move(indices);
//		return false;
//	}
//
//
//	bool DeserializeSubMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
//	{
//		std::unique_ptr<_GEOM::Geom::SubMesh[]> subMesh = std::make_unique<_GEOM::Geom::SubMesh[]>(GeomData.m_nSubMeshes);
//
//		for (std::uint32_t i{}; i < GeomData.m_nSubMeshes; ++i)
//		{
//			if (DEBUGDeserializeValue(inFile, subMesh[i].m_nFaces) ||
//				DEBUGDeserializeValue(inFile, subMesh[i].m_iIndices) ||
//				DEBUGDeserializeValue(inFile, subMesh[i].m_nIndices) ||
//				DEBUGDeserializeValue(inFile, subMesh[i].m_iVertices) ||
//				DEBUGDeserializeValue(inFile, subMesh[i].m_nVertices) ||
//				DEBUGDeserializeValue(inFile, subMesh[i].m_iMaterial) ||
//				DEBUGDeserializeValue(inFile, subMesh[i].m_PosCompressionOffset) ||
//				DEBUGDeserializeValue(inFile, subMesh[i].m_UVCompressionOffset))
//				assert(false);
//		}
//		GeomData.m_pSubMesh = std::move(subMesh);
//		return false;
//	}
//
//
//	bool DeserializeMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept
//	{
//		std::unique_ptr<_GEOM::Geom::Mesh[]> uMesh = std::make_unique<_GEOM::Geom::Mesh[]>(GeomData.m_nMeshes);
//
//		for (std::uint32_t i{}; i < GeomData.m_nMeshes; ++i)
//		{
//			DeserializeArray(inFile, uMesh[i].m_name);
//		}
//		GeomData.m_pMesh = std::move(uMesh);
//		return false;
//	}
//}
//
//std::ostream& operator<<(std::ostream& os, const glm::vec2& v)
//{
//	os << "x: " << v.x << " y: " << v.y << "\n";
//	return os;
//}
//
//std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
//{
//	os << "x: " << v.x << " y: " << v.y << " z: " << v.z << "\n";
//	return os;
//}
//
//std::ifstream& operator>>(std::ifstream& is, glm::vec2& v)
//{
//	is >> v.x >> v.y;
//	return is;
//}
//
//std::ifstream& operator>>(std::ifstream& is, glm::vec3& v)
//{
//	is >> v.x >> v.y >> v.z;
//	return is;
//}