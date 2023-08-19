#ifndef MESH_HPP
#define MESH_HPP

#include "Math.hpp"
#include "Vao.hpp"
#include "Vbo.hpp"
#include <geom.h>

constexpr int MAX_INSTANCES = 1000;

// Follows the format in the shader code
/******************************************
	location = 0 --> Pos;
	location = 1 --> VertexColor;
	location = 2 --> LTW;
******************************************/

namespace GFX
{
	class Mesh
	{
	public:
		// -- Called once on startup --
		void LoadFromGeom(const _GEOM::Geom& GeomData, std::vector<vec3>& positions, std::vector<unsigned short>& indices);
		void Setup(std::vector<vec3> const& positions, std::vector<unsigned short> const& indices);

		// -- Used when drawing --
		void BindVao();
		void PrepForDraw();
		void UnbindVao();

		// -- Used after drawing current scene --
		void ClearInstances();

		int GetVertexCount() { return mVertexCount; }
		int GetIndexCount() { return mIndexCount; }

		// Called once on exit --
		void Destroy();

		// Stores the rendering data for each instance of mesh
		std::vector<mat4> mLTW;
		std::vector<vec4> mColors;

		glm::vec3 m_PosCompressionScale;
		glm::vec3 m_PosCompressionOffset;
		glm::vec2 m_UVCompressionScale;
		glm::vec2 m_UVCompressionOffset;

	private:
		// Vertex array object and buffer object for each mesh
		VAO mVao;
		VBO mVbo;
		VBO mEbo;
		VBO mColorVbo;
		VBO mLTWVbo;

		// Stats of the mesh model
		int mVertexCount;
		int mIndexCount;
	};
}


namespace Deserialization
{
	bool ReadUnsigned(std::ifstream& inFile, std::uint32_t& value) noexcept;
	bool ReadSigned(std::ifstream& inFile, std::int16_t& value) noexcept;
	bool ReadVec3WithHeader(std::ifstream& inFile, glm::vec3& value) noexcept;
	bool ReadVec2WithHeader(std::ifstream& inFile, glm::vec2& value) noexcept;
	bool ReadMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
	bool ReadSubMesh(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
	bool ReadVertexPos(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
	bool ReadVertexExtra(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;
	bool ReadIndices(std::ifstream& inFile, _GEOM::Geom& GeomData) noexcept;

	bool DeserializeGeom(const std::string Filepath, _GEOM::Geom& GeomData) noexcept;
}

#endif
