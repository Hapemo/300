#version 450

// -- INPUTS --
layout (location = 0) out vec4 fHealthColor;
layout (location = 1) out vec4 fBackColor;
layout (location = 2) in vec2 fTexCoords;

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor;

// -- UNIFORMS --
uniform float uHealth;

void main()
{
	if (fTexCoords.x < uHealth * 0.01)
		fragColor = vec4(0.0, 1.0, 0.0, 1.0);
	else
		fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}