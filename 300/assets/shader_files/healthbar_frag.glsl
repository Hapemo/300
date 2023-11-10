#version 450

// -- INPUTS --
layout (location = 0) in vec4 fHealthColor;		// r, g, b: Health color | a: Health value
layout (location = 1) in vec4 fBackColor;		// r, g, b, a: Back health color
layout (location = 2) in vec2 fTexCoords;

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 entityID;

void main()
{
	float health = fHealthColor.a * 0.01;

	if (fTexCoords.x < health)
		fragColor = vec4(fHealthColor.rgb, fBackColor.a);
	else
		fragColor = fBackColor;
}