#version 450

// -- INPUTS -- 
layout (location = 0) in vec4 fColor;
layout (location = 1) in vec4 fTexId_EntId;		// x: texture Index | y: entity ID | z: degree | w: slider value
layout (location = 2) in vec2 fTexCoords;

uniform sampler2D uTex2d[32];

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor;
layout (location = 1) out uint outEntityID;

// -- GLOBAL --
vec2 up = vec2(0, 0.5);

float GetAngle(vec2 v)
{
	float angle = dot(up, v) / (length(up) * length(v));

	if (v.x < 0)
		return radians(360.0) - acos(angle);

	return acos(angle);
}

void main()
{
	float deg = radians(fTexId_EntId.z);

	// Discard fragment that is outside of the slider limit
	if (fTexCoords.x > fTexId_EntId.w)
		return;

	// Discard any fragment not part of the pie
	if (GetAngle(fTexCoords - vec2(0.5)) < deg)
		return;

	// Discard any fragment outside of circle
//	if (distance(fTexCoords, vec2(0.5)) >= 0.5)
//		return;

	// Retrieve Texture index and entity ID
	int texIndex = int(fTexId_EntId.x);
	uint entID = uint(fTexId_EntId.y);

	// Get fragment color
	vec4 outColor = vec4(1.0f);
	if (texIndex >= 0)
		outColor = texture(uTex2d[texIndex], fTexCoords);
	outColor = outColor * fColor;
	
	if (outColor.a < 0.1)
		discard;

	// Gamma correction
    float gamma = 2.2;
    vec4 finalColor = vec4(pow(outColor.rgb, vec3(1.0/gamma)), outColor.a);

	fragColor = finalColor * fColor;
	outEntityID = entID;
}