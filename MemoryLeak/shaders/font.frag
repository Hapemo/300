/*!*****************************************************************************
\file font.frag
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 14-10-2022
\brief
This file contains the fragment shader for rendering fonts.
*******************************************************************************/
#version 450 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

/*!*****************************************************************************
\brief
Main function of the fragment shader.
*******************************************************************************/
void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor) * sampled;
}  