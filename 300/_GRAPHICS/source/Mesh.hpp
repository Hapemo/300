#ifndef MESH_HPP
#define MESH_HPP

#include "Math.hpp"
#include "Vao.hpp"
#include "Vbo.hpp"

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
		void Setup(std::vector<vec3> const& positions, std::vector<unsigned short> const& indices);

		void ClearInstances();

	private:
		// Vertex array object and buffer object for each mesh
		VAO mVao;
		VBO mVbo;
		VBO mEbo;
		VBO mColorVbo;
		VBO mLTWVbo;

		// Stores the rendering data for each instance of mesh
		std::vector<mat4> mLTW;
		std::vector<vec4> mColors;

		// Stats of the mesh model
		int mVertexCount;
		int mIndexCount;
	};
}

#endif
