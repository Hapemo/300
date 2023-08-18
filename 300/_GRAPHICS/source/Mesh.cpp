#include "Mesh.hpp"

// Follows the format in the shader code
/******************************************
	location = 0 --> Pos;
	location = 1 --> VertexColor;
	location = 2 --> LTW;
******************************************/

void GFX::Mesh::Setup(std::vector<vec3> const& positions, std::vector<unsigned short> const& indices)
{
	// Create VAO
	mVao.Create();

	// Create VBO for mesh model
	mVbo.Create(positions.size() * sizeof(vec3));
	mVbo.AttachData(0, positions.size() * sizeof(vec3), positions.data());			// Attach mesh data to VBO
	mVao.AttachVerterBuffer(mVbo.GetID(), 0, 0, sizeof(vec3));

	// Attach point VBO to VAO
	mVao.AddAttribute(0, 0, 3, GL_FLOAT);											// location 0, binding vao index 0

	// Create VBO for Color data
	mColorVbo.Create(sizeof(vec4) * MAX_INSTANCES);

	// Attach Color VBO and divisor to VAO
	mVao.AddAttribute(1, 1, 4, GL_FLOAT);											// location 1, binding vao index 1
	mVao.AddAttributeDivisor(1, 1);													// divisor at vao index 1
	mVao.AttachVerterBuffer(mColorVbo.GetID(), 1, 0, sizeof(vec4));					// Attach to index 1

	// Create local-to-world VBO
	mLTWVbo.Create(sizeof(mat4) * MAX_INSTANCES);

	// Add attributes and divisor as vec4
	for (int i = 0; i < 4; ++i)		
	{
		mVao.AddAttribute(2 + i, 2, 4, GL_FLOAT, sizeof(vec4) * i);					// location 2, binding vao index 2
		mVao.AddAttributeDivisor(2, 1);												// divisor at vao index 2
	}
	// Attach LTW VBO to VAO
	mVao.AttachVerterBuffer(mLTWVbo.GetID(), 2, 0, sizeof(vec4) * 4);

	// Element Buffer Object
	mEbo.Create(indices.size() * sizeof(GLushort));
	mEbo.AttachData(0, indices.size() * sizeof(GLushort), indices.data());
	glVertexArrayElementBuffer(mVao.GetID(), mEbo.GetID());

	mVao.Unbind();

	// Store mesh stats
	mVertexCount = static_cast<int>(positions.size());
	mIndexCount = static_cast<int>(indices.size());
}

void GFX::Mesh::ClearInstances()
{
	mLTW.clear();
	mColors.clear();
}

void GFX::Mesh::BindVao()
{
	mVao.Bind();
}

void GFX::Mesh::UnbindVao()
{
	mVao.Unbind();
}

void GFX::Mesh::PrepForDraw()
{
	// Attach data to vbo
	mColorVbo.AttachData(0, mColors.size() * sizeof(vec4), mColors.data());
	mLTWVbo.AttachData(0, mLTW.size() * sizeof(mat4), mLTW.data());
}
