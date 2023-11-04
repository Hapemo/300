#version 450

// -- INPUTS -- 
layout (location = 0) in vec2 fTexCoords;

uniform sampler2D uTex2d[32];

// -- OUTPUTS --
out vec4 fragColor;

// -- UNIFORMS --
uniform sampler2D gameScene;

void main()
{
	fragColor = texture(gameScene, fTexCoords);
}