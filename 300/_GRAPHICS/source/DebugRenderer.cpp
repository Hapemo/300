#include "DebugRenderer.hpp"

GFX::DebugRenderer::DebugRenderer()
{
	SetupShader();
	SetupPointBufferObjects();
	SetupTriangleBufferObjects();
	SetupQuadBufferObjects();
	SetupAabbBufferObjects();
}

GFX::DebugRenderer::~DebugRenderer()
{
	mPointVao.Destroy();
	mPointVbo.Destroy();
	mTriangleVao.Destroy();
	mTriangleVbo.Destroy();
	mQuadVao.Destroy();
	mQuadVbo.Destroy();
	mQuadEbo.Destroy();
	mAabbVao.Destroy();
	mAabbVbo.Destroy();
	mAabbEbo.Destroy();

	mShader.DestroyShader();
}

void GFX::DebugRenderer::ActivateShader()
{
	mShader.Activate();
}

void GFX::DebugRenderer::DeactivateShader()
{
	mShader.Deactivate();
}

void GFX::DebugRenderer::AddPoint(vec3 const& pos, vec4 const& color)
{
	if (mPointLTW.size() >= MAX_INSTANCES)
	{
		std::cout << "Max Instances of Objects Reached!\n";
		return;
	}

	mat4 world = {
		vec4(1.f, 0.f, 0.f, 0.f),
		vec4(0.f, 1.f, 0.f, 0.f),
		vec4(0.f, 0.f, 1.f, 0.f),
		vec4(pos, 1.f)
	};

	mPointLTW.push_back(world);
	mPointColors.push_back(color);
}

void GFX::DebugRenderer::AddTriangle(vec3 const& p0, vec3 const& p1, vec3 const& p2, vec4 const& color)
{
	if (mTriangleLTW.size() >= MAX_INSTANCES)
	{
		std::cout << "Max Instances of Objects Reached!\n";
		return;
	}

	mat4 world = {
		vec4(p0, 0.f),
		vec4(p1, 0.f),
		vec4(p2, 0.f),
		vec4(0.f, 0.f, 0.f, 1.f)
	};

	mTriangleLTW.push_back(world);
	mTriangleColors.push_back(color);
}

void GFX::DebugRenderer::AddQuad(vec3 const& center, float width, float height, vec4 const& color)
{
	mat4 world = {
	vec4(width, 0.f, 0.f, 0.f),
	vec4(0.f, height, 0.f, 0.f),
	vec4(0.f, 0.f, 1.f, 0.f),
	vec4(center, 1.f)
	};

	mQuadLTW.push_back(world);
	mQuadColors.push_back(color);
}

void GFX::DebugRenderer::AddAabb(vec3 const& center, vec3 const& size, vec4 const& color)
{
	mat4 world = {
		vec4(size.x, 0.f, 0.f, 0.f),
		vec4(0.f, size.y, 0.f, 0.f),
		vec4(0.f, 0.f, size.z, 0.f),
		vec4(center, 1.f)
	};

	mAabbLTW.push_back(world);
	mAabbColors.push_back(color);
}

void GFX::DebugRenderer::RenderAll(mat4 viewProj)
{
	RenderAllPoints(viewProj);
	RenderAllTriangles(viewProj);
	RenderAllQuads(viewProj);
	RenderAllAabb(viewProj);
}

void GFX::DebugRenderer::ClearInstances()
{
	mPointColors.clear();
	mPointLTW.clear();

	mTriangleColors.clear();
	mTriangleLTW.clear();

	mQuadColors.clear();
	mQuadLTW.clear();

	mAabbColors.clear();
	mAabbLTW.clear();
}

void GFX::DebugRenderer::SetupShader()
{
	mShader.CreateShader(vertexShaderCode, fragmentShaderCode);
}

void GFX::DebugRenderer::SetupPointBufferObjects()
{
	// Create VAO
	mPointVao.Create();

	// Create VBO for Point object
	std::vector<GLfloat> pointMesh	// Point mesh
	{
		0.f, 0.f, 0.f
	};
	mPointVbo.Create(pointMesh.size() * sizeof(GLfloat));
	mPointVbo.AttachData(0, pointMesh.size() * sizeof(GLfloat), pointMesh.data());	// Attach mesh data to VBO
	mPointVao.AttachVerterBuffer(mPointVbo.GetID(), 0, 0, sizeof(GLfloat) * pointMesh.size());

	// Attach point VBO to VAO
	mPointVao.AddAttribute(0, 0, 3, GL_FLOAT);							// location 0, binding vao index 0
	
	// Create VBO for Color data
	mPointColorVbo.Create(sizeof(vec4) * MAX_INSTANCES);

	// Attach Color VBO and divisor to VAO
	mPointVao.AddAttribute(1, 1, 4, GL_FLOAT);							// location 1, binding vao index 1
	mPointVao.AddAttributeDivisor(1, 1);									// divisor at vao index 1
	mPointVao.AttachVerterBuffer(mPointColorVbo.GetID(), 1, 0, sizeof(vec4));	// Attach to index 1

	// Create local-to-world VBO
	mPointLTWVbo.Create(sizeof(mat4) * MAX_INSTANCES);
	for (int i = 0; i < 4; ++i)		// Add attributes and divisor as vec4
	{
		mPointVao.AddAttribute(2 + i, 2, 4, GL_FLOAT, sizeof(vec4) * i);		// location 2, binding vao index 2
		mPointVao.AddAttributeDivisor(2, 1);									// divisor at vao index 2
	}
	// Attach LTW VBO to VAO
	mPointVao.AttachVerterBuffer(mPointLTWVbo.GetID(), 2, 0, sizeof(vec4) * 4);

	mPointVao.Unbind();
}

void GFX::DebugRenderer::SetupTriangleBufferObjects()
{
	// Create VAO
	mTriangleVao.Create();

	// Create VBO for Point object
	std::vector<vec3> triangleMesh	// Point mesh
	{
		vec3(1.f, 0.f, 0.f),
		vec3(0.f, 1.f, 0.f),
		vec3(0.f, 0.f, 1.f)
	};
	mTriangleVbo.Create(triangleMesh.size() * sizeof(vec3));
	mTriangleVbo.AttachData(0, triangleMesh.size() * sizeof(vec3), triangleMesh.data());	// Attach mesh data to VBO
	mTriangleVao.AttachVerterBuffer(mTriangleVbo.GetID(), 0, 0, sizeof(vec3));

	// Attach point VBO to VAO
	mTriangleVao.AddAttribute(0, 0, 3, GL_FLOAT);							// location 0, binding vao index 0

	// Create VBO for Color data
	mTriangleColorVbo.Create(sizeof(vec4) * MAX_INSTANCES);

	// Attach Color VBO and divisor to VAO
	mTriangleVao.AddAttribute(1, 1, 4, GL_FLOAT);							// location 1, binding vao index 1
	mTriangleVao.AddAttributeDivisor(1, 1);									// divisor at vao index 1
	mTriangleVao.AttachVerterBuffer(mTriangleColorVbo.GetID(), 1, 0, sizeof(vec4));	// Attach to index 1

	// Create local-to-world VBO
	mTriangleLTWVbo.Create(sizeof(mat4) * MAX_INSTANCES);
	for (int i = 0; i < 4; ++i)		// Add attributes and divisor as vec4
	{
		mTriangleVao.AddAttribute(2 + i, 2, 4, GL_FLOAT, sizeof(vec4) * i);		// location 2, binding vao index 2
		mTriangleVao.AddAttributeDivisor(2, 1);									// divisor at vao index 2
	}
	// Attach LTW VBO to VAO
	mTriangleVao.AttachVerterBuffer(mTriangleLTWVbo.GetID(), 2, 0, sizeof(vec4) * 4);

	mTriangleVao.Unbind();
}

void GFX::DebugRenderer::SetupQuadBufferObjects()
{
	// Create VAO
	mQuadVao.Create();

	// Create VBO for Quad object
	std::vector<vec3> quadMesh	// Point mesh
	{
		vec3(-0.5f,  0.5f, 0.f),		// Top left
		vec3(-0.5f, -0.5f, 0.f),		// Bottom left
		vec3( 0.5f, -0.5f, 0.f),		// Bottom right
		vec3( 0.5f,  0.5f, 0.f)			// Top right
	};
	mQuadVbo.Create(quadMesh.size() * sizeof(vec3));
	mQuadVbo.AttachData(0, quadMesh.size() * sizeof(vec3), quadMesh.data());	// Attach mesh data to VBO
	mQuadVao.AttachVerterBuffer(mQuadVbo.GetID(), 0, 0, sizeof(vec3));

	// Attach point VBO to VAO
	mQuadVao.AddAttribute(0, 0, 3, GL_FLOAT);							// location 0, binding vao index 0 - Position

	// Create VBO for Color data
	mQuadColorVbo.Create(sizeof(vec4) * MAX_INSTANCES);

	// Attach Color VBO and divisor to VAO
	mQuadVao.AddAttribute(1, 1, 4, GL_FLOAT);								// location 1, binding vao index 1
	mQuadVao.AddAttributeDivisor(1, 1);										// divisor at vao index 1
	mQuadVao.AttachVerterBuffer(mQuadColorVbo.GetID(), 1, 0, sizeof(vec4));	// Attach to index 1

	// Create local-to-world VBO
	mQuadLTWVbo.Create(sizeof(mat4) * MAX_INSTANCES);
	for (int i = 0; i < 4; ++i)		// Add attributes and divisor as vec4
	{
		mQuadVao.AddAttribute(2 + i, 2, 4, GL_FLOAT, sizeof(vec4) * i);		// location 2 + i, binding vao index 2
		mQuadVao.AddAttributeDivisor(2, 1);									// divisor at vao index 2
	}
	// Attach LTW VBO to VAO
	mQuadVao.AttachVerterBuffer(mQuadLTWVbo.GetID(), 2, 0, sizeof(vec4) * 4); // binding vao index 2

	std::array<GLushort, 6> idx = {
		0, 1, 2,
		2, 3, 0
	};

	// Element Buffer Object
	mQuadEbo.Create(idx.size() * sizeof(GLushort));
	mQuadEbo.AttachData(0, idx.size() * sizeof(GLushort), idx.data());
	glVertexArrayElementBuffer(mQuadVao.GetID(), mQuadEbo.GetID());

	mQuadVao.Unbind();
}

void GFX::DebugRenderer::SetupAabbBufferObjects()
{
	// Create VAO
	mAabbVao.Create();

	// Create VBO for Aabb object
	std::vector<vec3> aabbMesh
	{
		vec3(-0.5f,  0.5f,  0.5f),		// Top left near
		vec3(-0.5f, -0.5f,  0.5f),		// Bottom left near
		vec3( 0.5f, -0.5f,  0.5f),		// Bottom right near
		vec3( 0.5f,  0.5f,  0.5f),		// Top right near
		vec3(-0.5f,  0.5f, -0.5f),		// Top left far
		vec3(-0.5f, -0.5f, -0.5f),		// Bottom left far
		vec3( 0.5f, -0.5f, -0.5f),		// Bottom right far
		vec3( 0.5f,  0.5f, -0.5f)		// Top right far
	};
	mAabbVbo.Create(aabbMesh.size() * sizeof(vec3));
	mAabbVbo.AttachData(0, aabbMesh.size() * sizeof(vec3), aabbMesh.data());	// Attach mesh data to VBO
	mAabbVao.AttachVerterBuffer(mAabbVbo.GetID(), 0, 0, sizeof(vec3));

	// Attach point VBO to VAO
	mAabbVao.AddAttribute(0, 0, 3, GL_FLOAT);							// location 0, binding vao index 0 - Position

	// Create VBO for Color data
	mAabbColorVbo.Create(sizeof(vec4) * MAX_INSTANCES);

	// Attach Color VBO and divisor to VAO
	mAabbVao.AddAttribute(1, 1, 4, GL_FLOAT);								// location 1, binding vao index 1
	mAabbVao.AddAttributeDivisor(1, 1);										// divisor at vao index 1
	mAabbVao.AttachVerterBuffer(mAabbColorVbo.GetID(), 1, 0, sizeof(vec4));	// Attach to index 1

	// Create local-to-world VBO
	mAabbLTWVbo.Create(sizeof(mat4) * MAX_INSTANCES);
	for (int i = 0; i < 4; ++i)		// Add attributes and divisor as vec4
	{
		mAabbVao.AddAttribute(2 + i, 2, 4, GL_FLOAT, sizeof(vec4) * i);		// location 2 + i, binding vao index 2
		mAabbVao.AddAttributeDivisor(2, 1);									// divisor at vao index 2
	}
	// Attach LTW VBO to VAO
	mAabbVao.AttachVerterBuffer(mAabbLTWVbo.GetID(), 2, 0, sizeof(vec4) * 4); // binding vao index 2

	std::array<GLushort, 16> idx = {
		0, 1, 2, 3,
		0, 4, 5, 1,
		2, 6, 7, 4,
		5, 6, 7, 3

	};

	// Element Buffer Object
	mAabbEbo.Create(idx.size() * sizeof(GLushort));
	mAabbEbo.AttachData(0, idx.size() * sizeof(GLushort), idx.data());
	glVertexArrayElementBuffer(mAabbVao.GetID(), mAabbEbo.GetID());

	mAabbVao.Unbind();
}

void GFX::DebugRenderer::RenderAllPoints(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mPointVao.Bind();

	// Attach data to vbo
	mPointColorVbo.AttachData(0, mPointColors.size() * sizeof(vec4), mPointColors.data());
	mPointLTWVbo.AttachData(0, mPointLTW.size() * sizeof(mat4), mPointLTW.data());

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawArraysInstanced(GL_POINTS, 0, mPointLTW.size(), mPointLTW.size());

	mShader.Deactivate();
	mPointVao.Unbind();
}

void GFX::DebugRenderer::RenderAllTriangles(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mTriangleVao.Bind();

	// Attach data to vbo
	mTriangleColorVbo.AttachData(0, mTriangleColors.size() * sizeof(vec4), mTriangleColors.data());
	mTriangleLTWVbo.AttachData(0, mTriangleLTW.size() * sizeof(mat4), mTriangleLTW.data());

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawArraysInstanced(GL_TRIANGLES, 0, mTriangleLTW.size() * 3, mTriangleLTW.size());

	mShader.Deactivate();
	mTriangleVao.Unbind();
}

void GFX::DebugRenderer::RenderAllQuads(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mQuadVao.Bind();

	// Attach data to vbo
	mQuadColorVbo.AttachData(0, mQuadColors.size() * sizeof(vec4), mQuadColors.data());
	mQuadLTWVbo.AttachData(0, mQuadLTW.size() * sizeof(mat4), mQuadLTW.data());

	// Use quad EBO
	glVertexArrayElementBuffer(mQuadVao.GetID(), mQuadEbo.GetID());

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, mQuadLTW.size());

	mShader.Deactivate();
	mQuadVao.Unbind();
}

void GFX::DebugRenderer::RenderAllAabb(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mAabbVao.Bind();

	// Attach data to vbo
	mAabbColorVbo.AttachData(0, mAabbColors.size() * sizeof(vec4), mAabbColors.data());
	mAabbLTWVbo.AttachData(0, mAabbLTW.size() * sizeof(mat4), mAabbLTW.data());

	// Use quad EBO
	glVertexArrayElementBuffer(mAabbVao.GetID(), mAabbEbo.GetID());

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawElementsInstanced(GL_LINE_LOOP, 16, GL_UNSIGNED_SHORT, nullptr, mAabbLTW.size());

	mShader.Deactivate();
	mAabbVao.Unbind();
}
