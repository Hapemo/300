#version 450
#extension GL_ARB_gpu_shader_int64 : enable

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

layout (std430, binding = 8) buffer uiTextureBuffer
{
    uint64_t uiTextures[];
};

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
		discard;

	// Discard any fragment not part of the pie
	if (GetAngle(fTexCoords - vec2(0.5)) < deg)
		discard;

	// Discard any fragment outside of circle
//	if (distance(fTexCoords, vec2(0.5)) >= 0.5)
//		return;

	// Retrieve Texture index and entity ID
	int texIndex = int(fTexId_EntId.x);
	uint entID = uint(fTexId_EntId.y);

	// Get fragment color
	vec4 outColor = vec4(1.0f);

	if (texIndex >= 0)
	{
		uint64_t handle = uiTextures[texIndex];		// get handle of UI textures
		outColor = texture(sampler2D(handle), fTexCoords);
	}
	outColor = outColor * fColor;
	
	if (outColor.a < 0.1)
		discard;

	// Gamma correction
    //float gamma = 2.2;
    //vec4 finalColor = vec4(pow(outColor.rgb, vec3(1.0/gamma)), outColor.a);
    vec4 finalColor = vec4(outColor.rgb, outColor.a);

	fragColor = finalColor * fColor;
	outEntityID = entID;
}