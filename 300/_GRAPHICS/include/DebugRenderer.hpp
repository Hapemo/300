/**-------------------------------------------------------------------------------------/
 * @file
 *  DebugRenderer.hpp
 * @author
 *  Lee Fu Sheng Roy, 2101440, f.lee@digipen.edu
 * @date
 *  2023/09/06
 * @brief
 *  Class definition of DebugRenderer. 
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#ifndef DEBUGRENDERER_HPP
#define DEBUGRENDERER_HPP

#include "Common.hpp"
#include "Shader.hpp"
#include "Vao.hpp"
#include "Vbo.hpp"
#include "Mesh.hpp"

namespace GFX
{
	class DebugRenderer
	{
	public:
		DebugRenderer();
		DebugRenderer(DebugRenderer const&) = delete;
		DebugRenderer& operator=(DebugRenderer const&) = delete;
		~DebugRenderer();

		void ActivateShader();
		void DeactivateShader();

		// -- Draw Functions -- 
		void AddPoint(vec3 const& pos, vec4 const& color);
		void AddTriangle(vec3 const& p0, vec3 const& p1, vec3 const& p2, vec4 const& color);
		void AddQuad(vec3 const& center, float width, float height, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });
		void AddAabb(vec3 const& center, vec3 const& size, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });
		void AddCube(vec3 const& center, vec3 const& size, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });
		void AddSphere(vec3 const& camPos, vec3 const& center, float radius, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });

		void RenderAll(mat4 viewProj);

		void ClearInstances();

		// -- TEMP --
		void BindQuadMesh() { mQuadMesh.BindVao(); }

	private:
		Shader mShader;

		// -- Mesh Models --
		Mesh mPointMesh;
		Mesh mTriangleMesh;
		Mesh mQuadMesh;
		Mesh mAabbMesh;
		Mesh mCubeMesh;
		Mesh mSphereMesh;

		// Shader Set up
		void SetupShader();

		void SetupPointMesh();
		void SetupTriangleMesh();
		void SetupQuadMesh();
		void SetupAabbMesh();
		void SetupCubeMesh();
		void SetupSphereMesh();

		// -- Render Shapes --
		void RenderAllPoints(mat4 const& viewProj);
		void RenderAllTriangles(mat4 const& viewProj);
		void RenderAllQuads(mat4 const& viewProj);
		void RenderAllAabb(mat4 const& viewProj);
		void RenderAllCube(mat4 const& viewProj);
		void RenderAllSphere(mat4 const& viewProj);

		// -- Shader Code --
		const char* vertexShaderCode = R"(
		#version 450 core
		uniform mat4 uMatrixVP;

		layout(location = 0) in vec3 aPos;
		layout(location = 1) in vec4 aVertexColor;
		layout(location = 3) in mat4 aLTW;

		layout(location = 0) out vec4 outColor;
		layout(location = 1) out vec2 outUV;
		void main() {
			gl_Position = uMatrixVP * aLTW * vec4(aPos, 1.0);
			outColor = aVertexColor;
		}
		)";

		const char* fragmentShaderCode = R"(
		#version 450 core
		layout(location = 0) in vec4 color;
		layout(location = 0) out vec4 fragColor0;
		layout(location = 1) out vec4 fragColor1;
		void main() {
			fragColor0 = color;
			fragColor1 = color;
		}
		)";
	};
}

#endif // DEBUGRENDERER_HPP
