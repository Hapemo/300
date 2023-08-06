#include "DebugRenderer.hpp"

CS380::DebugRenderer::DebugRenderer()
{
	SetupShader();
	SetupBufferObjects();
	SetupTriangleBufferObjects();
	SetupQuadBufferObjects();
}

CS380::DebugRenderer::~DebugRenderer()
{
	mPointVao.Destroy();
	mPointVbo.Destroy();
	mTriangleVao.Destroy();
	mTriangleVbo.Destroy();
	mQuadVao.Destroy();
	mQuadVbo.Destroy();
	mQuadEbo.Destroy();

	mShader.DestroyShader();
}

void CS380::DebugRenderer::ActivateShader()
{
	mShader.Activate();
}

void CS380::DebugRenderer::DeactivateShader()
{
	mShader.Deactivate();
}

void CS380::DebugRenderer::AddPoint(vec3 const& pos, vec4 const& color)
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
	mColors.push_back(color);
}

void CS380::DebugRenderer::AddTriangle(vec3 const& p0, vec3 const& p1, vec3 const& p2, vec4 const& color)
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

void CS380::DebugRenderer::AddQuad(vec3 const& center, float width, float height, vec4 const& color)
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

void CS380::DebugRenderer::RenderAll(mat4 viewProj)
{
	RenderAllPoints(viewProj);
	RenderAllTriangles(viewProj);
	RenderAllQuads(viewProj);
}

void CS380::DebugRenderer::ClearInstances()
{
	mColors.clear();
	mPointLTW.clear();

	mTriangleColors.clear();
	mTriangleLTW.clear();

	mQuadColors.clear();
	mQuadLTW.clear();
}

void CS380::DebugRenderer::SetupShader()
{
	mShader.CreateShader(vertexShaderCode, fragmentShaderCode);
}

void CS380::DebugRenderer::SetupBufferObjects()
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

void CS380::DebugRenderer::SetupTriangleBufferObjects()
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

void CS380::DebugRenderer::SetupQuadBufferObjects()
{
	// Create VAO
	mQuadVao.Create();

	// Create VBO for Point object
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
	mQuadVao.AddAttribute(1, 1, 4, GL_FLOAT);							// location 1, binding vao index 1
	mQuadVao.AddAttributeDivisor(1, 1);									// divisor at vao index 1
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

void CS380::DebugRenderer::RenderAllPoints(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mPointVao.Bind();

	// Attach data to vbo
	mPointColorVbo.AttachData(0, mColors.size() * sizeof(vec4), mColors.data());
	mPointLTWVbo.AttachData(0, mPointLTW.size() * sizeof(mat4), mPointLTW.data());

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawArraysInstanced(GL_POINTS, 0, mPointLTW.size(), mPointLTW.size());

	mShader.Deactivate();
	mPointVao.Unbind();
}

void CS380::DebugRenderer::RenderAllTriangles(mat4 const& viewProj)
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

void CS380::DebugRenderer::RenderAllQuads(mat4 const& viewProj)
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
