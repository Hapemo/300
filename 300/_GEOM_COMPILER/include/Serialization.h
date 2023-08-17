//#pragma once
//#ifndef _SERIALIZATION_H
//#define _SERIALIZATION_H
//
//#include <fstream>
//#include <geom.h>
//
//namespace Serialization
//{
//	bool SerializeUnsigned(std::ofstream& outFile, const std::uint32_t& value) noexcept;
//
//	template <typename T>
//	bool DEBUGSerializeValue(std::ofstream& outFile, const T& value) noexcept;
//
//	template <typename T>
//	bool DEBUGDeserializeValue(std::ifstream& inFile, T& value) noexcept;
//
//	template <typename T>
//	bool SerializeValue(std::ofstream& outFile, const T& value) noexcept;
//
//	template <typename T>
//	bool DeserializeValue(std::ifstream& inFile, T& value) noexcept;
//
//	template <typename T, std::size_t N>
//	bool SerializeArray(std::ofstream& outFile, const std::array<T, N>& arr) noexcept;
//
//	template <typename T, std::size_t N>
//	bool DeserializeArray(std::ifstream& inFile, std::array<T, N>& arr) noexcept;
//
//	bool SerializeVertexPos(std::ofstream& outFile, const _GEOM::Geom::VertexPos* const vertexPos, std::uint32_t length) noexcept;
//	bool SerializeVertexExtra(std::ofstream& outFile, const _GEOM::Geom::VertexExtra* const vertexExtra, std::uint32_t length) noexcept;
//	bool SerializeMesh(std::ofstream& outFile, const _GEOM::Geom::Mesh& const mesh) noexcept;
//	bool SerializeIndices(std::ofstream& outFile, const std::uint32_t* const indices, std::uint32_t length) noexcept;
//	bool SerializeSubMesh(std::ofstream& outFile, const _GEOM::Geom::SubMesh* const subMesh, std::uint32_t length) noexcept;
//
//	bool DeserializeVertexExtra(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
//	bool DeserializeIndices(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
//	bool DeserializeVertexPos(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
//	bool DeserializeMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
//	bool DeserializeSubMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
//}
//
//std::ostream& operator<<(std::ostream& os, const glm::vec2& v);
//std::ostream& operator<<(std::ostream& os, const glm::vec3& v);
//
//std::ifstream& operator>>(std::ifstream& is, glm::vec2& v);
//std::ifstream& operator>>(std::ifstream& is, glm::vec3& v);
//
//#endif // !_SERIALIZATION_H