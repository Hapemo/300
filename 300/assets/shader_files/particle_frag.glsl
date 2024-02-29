#version 450
#extension GL_ARB_gpu_shader_int64 : enable

// -- INPUTS --
layout (location = 0) in vec4 fColor;
layout (location = 1) in vec2 fTexCoords;
layout (location = 2) flat in uint64_t fTexture;

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor;

void main()
{
	if (fTexture != 0)		// Valid texture exists
	{
		vec4 textureColor = texture(sampler2D(fTexture), fTexCoords);
		fragColor = textureColor * fColor;
	}
	else	// No texture, use color instead
	{
		fragColor = fColor;
	}
}
