/*!*****************************************************************************
\file font.vert
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 14-10-2022
\brief
This file contains the vertex shader for rendering fonts.
*******************************************************************************/
#version 450 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
uniform float zValue;

/*!*****************************************************************************
\brief
Main function of the vertex shader.
*******************************************************************************/
void main()
{
    vec4 temp = projection * vec4(vertex.xy, 0.0, 1.0);
    temp.z = -zValue;
    gl_Position = temp;
    TexCoords = vertex.zw;
}  