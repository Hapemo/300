/*!*****************************************************************************
\file texture.vert
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains the vertex shader for rendering textures. 2 Shader programs
are being used to reduce the frequency of "if" statements in the fragment shader.
*******************************************************************************/
#version 450 core

//taking in vertex attributes
layout (location=0) in vec3 aVertexPosition;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aVertexTexture;
layout (location=3) in float aTexID;

//passing to fragment shader
out vec4 vColor;
out vec2 vTexCoord;
out float vTexID;

/*!*****************************************************************************
\brief
Main function of the vertex shader.
*******************************************************************************/
void main(void) 
{
	gl_Position = vec4(vec3(vec2(aVertexPosition), -aVertexPosition.z), 1.0);
	vColor = aColor;
	vTexCoord = aVertexTexture;
	vTexID = aTexID;
}
