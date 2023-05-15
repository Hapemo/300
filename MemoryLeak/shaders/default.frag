/*!*****************************************************************************
\file default.frag
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains the fragment shader for rendering default shapes. 2 Shader 
programs are being used to reduce the frequency of "if" statements in the 
fragment shader.
*******************************************************************************/
#version 450 core

//taking in vertex attributes from vertex shader
in vec4 vColor;

//fragment color
out vec4 fFragColor;

/*!*****************************************************************************
\brief
Main function of the fragment shader.
*******************************************************************************/
void main (void) 
{
	fFragColor = vColor;
}
