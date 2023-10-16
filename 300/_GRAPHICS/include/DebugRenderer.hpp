/*!*****************************************************************************
\file DebugRenderer.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
DebugRender class implementation. Mostly provides the functionality of debug
drawing of different shapes.
*******************************************************************************/

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

		
/*!*****************************************************************************
Default constructor of the DebugRenderer class. Sets up the mesh of the debug 
shapes
*******************************************************************************/
		DebugRenderer();
		
/*!*****************************************************************************
Copy constructor deleted
*******************************************************************************/
		DebugRenderer(DebugRenderer const&) = delete;
		
/*!*****************************************************************************
Copy assignment operator deleted
*******************************************************************************/
		DebugRenderer& operator=(DebugRenderer const&) = delete;
		
/*!*****************************************************************************
Destructor of the DebugRenderer class. Destroys the meshes of the debug shapes
*******************************************************************************/
		~DebugRenderer();

/*!*****************************************************************************
Activates the debug draw shader
*******************************************************************************/
		void ActivateShader();
		
/*!*****************************************************************************
Deactivates the debug draw shader
*******************************************************************************/
		void DeactivateShader();

/*!*****************************************************************************
Adds an instance of a point to be drawn
*******************************************************************************/
		void AddPoint(vec3 const& pos, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });
		
/*!*****************************************************************************
Adds an instance of a line to be drawn
*******************************************************************************/
		void AddLine(vec3 const& pos1, vec3 const& pos2, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });
		
/*!*****************************************************************************
Adds an instance of a triangle to be drawn
*******************************************************************************/
		void AddTriangle(vec3 const& p0, vec3 const& p1, vec3 const& p2, vec4 const& color);
		
/*!*****************************************************************************
Adds an instance of a quad to be drawn
*******************************************************************************/
		void AddQuad(vec3 const& center, float width, float height, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });
		
/*!*****************************************************************************
Adds an instance of a Aabb to be drawn
*******************************************************************************/
		void AddAabb(vec3 const& center, vec3 const& size, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });
		
/*!*****************************************************************************
Adds an instance of a cube to be drawn
*******************************************************************************/
		void AddCube(vec3 const& center, vec3 const& size, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });
		
/*!*****************************************************************************
Adds an instance of a sphere to be drawn
*******************************************************************************/
		void AddSphere(vec3 const& camPos, vec3 const& center, float radius, vec4 const& color = vec4{ 1.f, 1.f, 1.f, 1.f });

/*!*****************************************************************************
Renders all instances of all debug shapes
*******************************************************************************/
		void RenderAll(mat4 viewProj);

/*!*****************************************************************************
Clears all instances of all debug shapes
*******************************************************************************/
		void ClearInstances();

	private:
		Shader mShader;

		// -- Mesh Models --
		Mesh mPointMesh;
		Mesh mLineMesh;
		Mesh mTriangleMesh;
		Mesh mQuadMesh;
		Mesh mAabbMesh;
		Mesh mCubeMesh;
		Mesh mSphereMesh;

		// Shader Set up
		void SetupShader();

		void SetupPointMesh();
		void SetupLineMesh();
		void SetupTriangleMesh();
		void SetupQuadMesh();
		void SetupAabbMesh();
		void SetupCubeMesh();
		void SetupSphereMesh();

		// -- Render Shapes --
		void RenderAllPoints(mat4 const& viewProj);
		void RenderAllLines(mat4 const& viewProj);
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
		void main() {
			fragColor0 = color;
		}
		)";
	};
}

#endif // DEBUGRENDERER_HPP
