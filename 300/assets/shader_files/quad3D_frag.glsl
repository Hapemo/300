#version 450

// -- INPUTS -- 
layout (location = 0) in vec4 fColor;
layout (location = 1) in vec4 fTexId_EntId;		// x: texture Index | y: entity ID | z: degree | w: slider value
layout (location = 2) in vec2 fTexCoords;

uniform sampler2D uTex2d[32];

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor;

void main()
{
	// Retrieve Texture index and entity ID
	int texIndex = int(fTexId_EntId.x);

	// Get fragment color
	vec4 outColor = vec4(1.0f);
	if (texIndex >= 0)
		outColor = texture(uTex2d[texIndex], fTexCoords);
	outColor = outColor * fColor;
	
	if (outColor.a < 0.1)
		discard;

	fragColor = outColor;
}