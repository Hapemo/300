#version 450
#extension GL_ARB_gpu_shader_int64 : enable

// -- INPUTS -- 
layout (location = 0) in vec4 fColor;
layout (location = 1) in vec4 fTexId_EntId;		// x: texture Index | y: entity ID | z: degree | w: slider value
layout (location = 2) in vec2 fTexCoords;

uniform sampler2D uTex2d[32];

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor;

layout(std430, binding = 8) buffer uiTextureBuffer
{
	uint64_t uiTextures[];
};

void main()
{
	// Retrieve Texture index and entity ID
	int texIndex = int(fTexId_EntId.x);

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

	fragColor = outColor;
}