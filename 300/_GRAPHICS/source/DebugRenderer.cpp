/**-------------------------------------------------------------------------------------/
 * @file
 * DebugRenderer.cpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Camera.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#include "DebugRenderer.hpp"

GFX::DebugRenderer::DebugRenderer()
{
	SetupShader();

	// Set up basic mesh models
	SetupPointMesh();
	SetupLineMesh();
	SetupTriangleMesh();
	SetupQuadMesh();
	SetupAabbMesh();
	SetupCubeMesh();
	SetupSphereMesh();
}

GFX::DebugRenderer::~DebugRenderer()
{
	// Destroy created mesh
	mPointMesh.Destroy();
	mLineMesh.Destroy();
	mTriangleMesh.Destroy();
	mQuadMesh.Destroy();
	mAabbMesh.Destroy();
	mCubeMesh.Destroy();
	mSphereMesh.Destroy();

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
		//std::cout << "Max Instances of Objects Reached!\n";
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

void GFX::DebugRenderer::AddLine(vec3 const& pos1, vec3 const& pos2, vec4 const& color)
{
	if (mLineMesh.mLTW.size() >= MAX_INSTANCES)
	{
		//std::cout << "Max Instances of Objects Reached!\n";
		return;
	}

	mat4 world = {
		vec4(pos1, 0.f),
		vec4(pos2, 0.f),
		vec4(0.f, 0.f, 1.f, 0.f),
		vec4(0.f, 0.f, 0.f, 1.f),
	};

	mLineMesh.mLTW.push_back(world);
	mLineMesh.mColors.push_back(color);
}

void GFX::DebugRenderer::AddTriangle(vec3 const& p0, vec3 const& p1, vec3 const& p2, vec4 const& color)
{
	if (mTriangleMesh.mLTW.size() >= MAX_INSTANCES)
	{
		//std::cout << "Max Instances of Objects Reached!\n";
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
		//std::cout << "Max Instances of Objects Reached!\n";
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
		//std::cout << "Max Instances of Objects Reached!\n";
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

void GFX::DebugRenderer::AddCube(vec3 const& center, vec3 const& size, vec4 const& color)
{
	if (mCubeMesh.mLTW.size() >= MAX_INSTANCES)
	{
		//std::cout << "Max Instances of Objects Reached!\n";
		return;
	}

	mat4 world = {
		vec4(size.x, 0.f, 0.f, 0.f),
		vec4(0.f, size.y, 0.f, 0.f),
		vec4(0.f, 0.f, size.z, 0.f),
		vec4(center, 1.f)
	};

	mCubeMesh.mLTW.push_back(world);
	mCubeMesh.mColors.push_back(color);
}

void GFX::DebugRenderer::AddSphere(vec3 const& camPos, vec3 const& center, float radius, vec4 const& color)
{
	// Calculate the horizon disc's position and radius
	float distanceSq = glm::dot(center - camPos, center - camPos);
	float length = glm::sqrt(glm::abs(distanceSq - radius * radius));
	float rPrime = (radius * length) / glm::sqrt(distanceSq);
	float offset = glm::sqrt(glm::abs(radius * radius - rPrime * rPrime));

	mat4 modelToWorld = {
		vec4(radius * 2, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, radius * 2, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, radius * 2, 0.0f),
		vec4(center, 1.0f)
	};

	mat4 horizonModelToWorld = {
		vec4(rPrime * 2, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, rPrime * 2, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, rPrime * 2, 0.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f)
	};

	// Add the disc along the X-Y axis
	mSphereMesh.mLTW.push_back(modelToWorld);
	mSphereMesh.mColors.push_back(color);

	// Add the disc along the Y-Z axis
	glm::mat4 modelToWorldYZ = glm::rotate(modelToWorld, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mSphereMesh.mLTW.push_back(modelToWorldYZ);

	// Add the disc along the X-Z axis
	glm::mat4 modelToWorldXZ = glm::rotate(modelToWorld, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mSphereMesh.mLTW.push_back(modelToWorldXZ);

	// Compute Transformation of the horizon disc 
	vec3 normal = camPos - center;
	vec3 up = { 0, 0, 1 };
	vec3 right = glm::cross(up, normal);
	vec3 forward = glm::cross(right, normal);
	vec3 centerPrime = center + glm::normalize(camPos - center) * offset;

	mat4 horizonRotate = {
		vec4(glm::normalize(right), 0.0f),
		vec4(glm::normalize(forward), 0.0f),
		vec4(glm::normalize(normal), 0.0f),
		vec4(centerPrime, 1.0f)
	};
	horizonModelToWorld = horizonRotate * horizonModelToWorld;
	mSphereMesh.mLTW.push_back(horizonModelToWorld);
}

void GFX::DebugRenderer::RenderAll(mat4 viewProj)
{
	if (mPointMesh.mLTW.size())
		RenderAllPoints(viewProj);
	if (mLineMesh.mLTW.size())
		RenderAllLines(viewProj);
	if (mTriangleMesh.mLTW.size())
		RenderAllTriangles(viewProj);
	if (mQuadMesh.mLTW.size())
		RenderAllQuads(viewProj);
	if (mAabbMesh.mLTW.size())
		RenderAllAabb(viewProj);
	if (mCubeMesh.mLTW.size())
		RenderAllCube(viewProj);
	if (mSphereMesh.mLTW.size())
		RenderAllSphere(viewProj);
}

void GFX::DebugRenderer::ClearInstances()
{
	mPointMesh.ClearInstances();
	mLineMesh.ClearInstances();
	mTriangleMesh.ClearInstances();
	mQuadMesh.ClearInstances();
	mAabbMesh.ClearInstances();
	mCubeMesh.ClearInstances();
	mSphereMesh.ClearInstances();
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

void GFX::DebugRenderer::SetupLineMesh()
{
	std::vector<vec3> positions
	{
		vec3(1.f, 0.f, 0.f),
		vec3(0.f, 1.f, 0.f)
	};

	std::vector<GLuint> indices
	{
		0, 1
	};

	mLineMesh.Setup(positions, indices);
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
	std::vector<vec3> positions	// Quad mesh
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

	std::vector<vec2> texCoords
	{
		vec2(0.f, 1.f),
		vec2(0.f, 0.f),
		vec2(1.f, 0.f),
		vec2(1.f, 1.f)
	};

	mQuadMesh.Setup(positions, indices, texCoords);
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

void GFX::DebugRenderer::SetupCubeMesh()
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
		0, 1, 2, 2, 3, 0,	// front
		7, 6, 5, 5, 4, 7,	// far
		4, 5, 1, 1, 0, 4,	// left
		7, 3, 2, 2, 6, 7,	// right
		4, 0, 3, 3, 7, 4,	// top
		5, 1, 2, 2, 6, 5	// bottom
	};

	mCubeMesh.Setup(positions, indices);
}

void GFX::DebugRenderer::SetupSphereMesh()
{
	constexpr int SLICES = 32;
	std::vector<vec3> positions;
	std::vector<GLuint> indices;

	positions.resize(SLICES);
	indices.resize(SLICES + 1);

	float x{};
	float y{};
	float angle = 0.0f;

	for (int i = 0; i < SLICES; ++i)
	{
		x = glm::cos(glm::radians(angle)) * 0.5f;
		y = glm::sin(glm::radians(angle)) * 0.5f;

		positions[i] = vec3{ x, y, 0.0f };
		indices[i] = i;

		angle += 360.0f / SLICES;
	}
	indices[SLICES] = 0;

	mSphereMesh.Setup(positions, indices, std::vector<vec2>{}, 4);
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
	glDrawElementsInstanced(GL_POINTS, mPointMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(mPointMesh.mLTW.size()));

	mShader.Deactivate();
	mPointMesh.UnbindVao();
}

void GFX::DebugRenderer::RenderAllLines(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mLineMesh.BindVao();

	// Attach data to vbo
	mLineMesh.PrepForDraw();

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawElementsInstanced(GL_LINES, mLineMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(mLineMesh.mLTW.size()));

	mShader.Deactivate();
	mLineMesh.UnbindVao();
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
	glDrawElementsInstanced(GL_TRIANGLES, mTriangleMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(mTriangleMesh.mLTW.size()));

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
	glDrawElementsInstanced(GL_TRIANGLES, mQuadMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(mQuadMesh.mLTW.size()));

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
	glDrawElementsInstanced(GL_LINE_LOOP, mAabbMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(mAabbMesh.mLTW.size()));

	mShader.Deactivate();
	mAabbMesh.UnbindVao();
}

void GFX::DebugRenderer::RenderAllCube(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mCubeMesh.BindVao();

	// Attach data to vbo
	mCubeMesh.PrepForDraw();

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawElementsInstanced(GL_TRIANGLES, mCubeMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(mCubeMesh.mLTW.size()));

	mShader.Deactivate();
	mCubeMesh.UnbindVao();
}

void GFX::DebugRenderer::RenderAllSphere(mat4 const& viewProj)
{
	// Attach shader to state
	mShader.Activate();

	// Bind VAO to pipeline
	mSphereMesh.BindVao();

	// Attach data to vbo
	mSphereMesh.PrepForDraw();

	// Set uniform
	glUniformMatrix4fv(mShader.GetUniformVP(), 1, GL_FALSE, &viewProj[0][0]);

	// Draw
	glDrawElementsInstanced(GL_LINE_STRIP, mSphereMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(mSphereMesh.mLTW.size()));

	mShader.Deactivate();
	mSphereMesh.UnbindVao();
}
