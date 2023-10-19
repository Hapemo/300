#version 450

// -- INPUTS -- 
layout (location = 0) in vec4 fColor;
layout (location = 1) in vec4 fTexId_EntId;		// x: texture Index | y: entity ID | z: empty | w: empty
layout (location = 2) in vec2 fTexCoords;

uniform sampler2D uTex2d[32];

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor;
layout (location = 1) out uint outEntityID;

void main()
{
	// Retrieve Texture index and entity ID
	int texIndex = int(fTexId_EntId.x);
	uint entID = uint(fTexId_EntId.y);

	// Get fragment color
	vec4 outColor = texture(uTex2d[texIndex], fTexCoords);
	outColor = outColor * fColor;

	fragColor = outColor;
	outEntityID = entID;
}