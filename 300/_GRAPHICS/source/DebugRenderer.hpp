/**-------------------------------------------------------------------------------------/
 * @file
 *  DebugRenderer.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
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

constexpr int MAX_INSTANCES = 5000;

namespace CS380
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

		void RenderAll(mat4 viewProj);

		void ClearInstances();

	private:
		Shader mShader;

		// -- Vertex Array and Vertex Buffers --
		// Point
		VAO mPointVao;
		VBO mPointVbo;
		VBO mPointColorVbo;
		VBO mPointLTWVbo;
		// Triangle
		VAO mTriangleVao;
		VBO mTriangleVbo;
		VBO mTriangleColorVbo;
		VBO mTriangleLTWVbo;
		// Quad
		VAO mQuadVao;
		VBO mQuadVbo;
		VBO mQuadColorVbo;
		VBO mQuadEbo;
		VBO mQuadLTWVbo;

		// -- Containers to store render data --
		std::vector<mat4> mPointLTW;
		std::vector<mat4> mTriangleLTW;
		std::vector<mat4> mQuadLTW;
		std::vector<vec4> mColors;
		std::vector<vec4> mTriangleColors;
		std::vector<vec4> mQuadColors;

		// Shader Set up
		void SetupShader();

		void SetupPointBufferObjects();
		void SetupTriangleBufferObjects();
		void SetupQuadBufferObjects();

		// -- Render Shapes --
		void RenderAllPoints(mat4 const& viewProj);
		void RenderAllTriangles(mat4 const& viewProj);
		void RenderAllQuads(mat4 const& viewProj);

		// -- Shader Code --
		const char* vertexShaderCode = R"(
		#version 450 core
		uniform mat4 uMatrixVP;

		layout(location = 0) in vec3 aPos;
		layout(location = 1) in vec4 aVertexColor;
		layout(location = 2) in mat4 aLTW;

		layout(location = 0) out vec4 outColor;
		void main() {
			gl_Position = uMatrixVP * aLTW * vec4(aPos, 1.0);
			outColor = aVertexColor;
		}
		)";

		const char* fragmentShaderCode = R"(
		#version 450 core
		layout(location = 0) in vec4 color;
		layout(location = 0) out vec4 fragColor;
		void main() {
			fragColor = color;
		}
		)";
	};
}

#endif // DEBUGRENDERER_HPP
