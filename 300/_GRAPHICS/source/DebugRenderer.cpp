#include "DebugRenderer.hpp"

GFX::DebugRenderer::DebugRenderer()
{
	SetupShader();

	// Set up basic mesh models
	SetupPointMesh();
	SetupTriangleMesh();
	SetupQuadMesh();
	SetupAabbMesh();
}

GFX::DebugRenderer::~DebugRenderer()
{
	// Destroy created mesh
	mPointMesh.Destroy();
	mTriangleMesh.Destroy();
	mQuadMesh.Destroy();
	mAabbMesh.Destroy();

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
	if (mPointMesh.mLTW.size() >= MAX_INSTANCES)
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

	mPointMesh.mLTW.push_back(world);
	mPointMesh.mColors.push_back(color);
}

void GFX::DebugRenderer::AddTriangle(vec3 const& p0, vec3 const& p1, vec3 const& p2, vec4 const& color)
{
	if (mTriangleMesh.mLTW.size() >= MAX_INSTANCES)
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

	mTriangleMesh.mLTW.push_back(world);
	mTriangleMesh.mColors.push_back(color);
}

void GFX::DebugRenderer::AddQuad(vec3 const& center, float width, float height, vec4 const& color)
{
	if (mQuadMesh.mLTW.size() >= MAX_INSTANCES)
	{
		std::cout << "Max Instances of Objects Reached!\n";
		return;
	}

	mat4 world = {
		vec4(width, 0.f, 0.f, 0.f),
		vec4(0.f, height, 0.f, 0.f),
		vec4(0.f, 0.f, 1.f, 0.f),
		vec4(center, 1.f)
	};

	mQuadMesh.mLTW.push_back(world);
	mQuadMesh.mColors.push_back(color);
}

void GFX::DebugRenderer::AddAabb(vec3 const& center, vec3 const& size, vec4 const& color)
{
	if (mAabbMesh.mLTW.size() >= MAX_INSTANCES)
	{
		std::cout << "Max Instances of Objects Reached!\n";
		return;
	}

	mat4 world = {
		vec4(size.x, 0.f, 0.f, 0.f),
		vec4(0.f, size.y, 0.f, 0.f),
		vec4(0.f, 0.f, size.z, 0.f),
		vec4(center, 1.f)
	};

	mAabbMesh.mLTW.push_back(world);
	mAabbMesh.mColors.push_back(color);
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
	mPointMesh.ClearInstances();
	mTriangleMesh.ClearInstances();
	mQuadMesh.ClearInstances();
	mAabbMesh.ClearInstances();
}

void GFX::DebugRenderer::SetupShader()
{
	mShader.CreateShader(vertexShaderCode, fragmentShaderCode);
}

void GFX::DebugRenderer::SetupPointMesh()
{
	std::vector<vec3> positions
	{
		vec3(0.f, 0.f, 0.f)
	};
	
	std::vector<GLuint> indices
	{
		0
	};

	mPointMesh.Setup(positions, indices);
}

void GFX::DebugRenderer::SetupTriangleMesh()
{
	std::vector<vec3> positions
	{
		vec3(1.f, 0.f, 0.f),
		vec3(0.f, 1.f, 0.f),
		vec3(0.f, 0.f, 1.f)
	};

	std::vector<GLuint> indices
	{
		0, 1, 2
	};

	mTriangleMesh.Setup(positions, indices);
}

void GFX::DebugRenderer::SetupQuadMesh()
{
	std::vector<vec3> positions	// Point mesh
	{
		vec3(-0.5f,  0.5f, 0.f),		// Top left
		vec3(-0.5f, -0.5f, 0.f),		// Bottom left
		vec3( 0.5f, -0.5f, 0.f),		// Bottom right
		vec3( 0.5f,  0.5f, 0.f)			// Top right
	};

	std::vector<GLuint> indices
	{
		0, 1, 2,
		2, 3, 0
	};

	mQuadMesh.Setup(positions, indices);
}

void GFX::DebugRenderer::SetupAabbMesh()
{
	std::vector<vec3> positions
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
	
	std::vector<GLuint> indices
	{
		0, 1, 2, 3,
		0, 4, 5, 1,
		2, 6, 7, 4,
		5, 6, 7, 3
	};

	mAabbMesh.Setup(positions, indices);
}

void GFX::DebugRenderer::RenderAllPoints(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mPointMesh.BindVao();

	// Attach data to vbo
	mPointMesh.PrepForDraw();

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawElementsInstanced(GL_POINTS, mPointMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, mPointMesh.mLTW.size());

	mShader.Deactivate();
	mPointMesh.UnbindVao();
}

void GFX::DebugRenderer::RenderAllTriangles(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mTriangleMesh.BindVao();

	// Attach data to vbo
	mTriangleMesh.PrepForDraw();

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawElementsInstanced(GL_TRIANGLES, mTriangleMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, mTriangleMesh.mLTW.size());

	mShader.Deactivate();
	mTriangleMesh.UnbindVao();
}

void GFX::DebugRenderer::RenderAllQuads(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mQuadMesh.BindVao();

	// Attach data to vbo
	mQuadMesh.PrepForDraw();

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawElementsInstanced(GL_TRIANGLES, mQuadMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, mQuadMesh.mLTW.size());

	mShader.Deactivate();
	mQuadMesh.UnbindVao();
}

void GFX::DebugRenderer::RenderAllAabb(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mAabbMesh.BindVao();

	// Attach data to vbo
	mAabbMesh.PrepForDraw();

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawElementsInstanced(GL_LINE_LOOP, mAabbMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, mAabbMesh.mLTW.size());

	mShader.Deactivate();
	mAabbMesh.UnbindVao();
}
