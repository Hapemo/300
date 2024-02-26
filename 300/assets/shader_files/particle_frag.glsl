#version 450
#extension GL_ARB_gpu_shader_int64 : enable

// -- INPUTS --
layout (location = 0) in vec4 fColor;
layout (location = 1) in vec2 fTexCoords;
layout (location = 2) flat in uint64_t fTexture;

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor;

struct Particle
{
	vec4 mColor;
	vec4 mVelocity;
	vec4 mSizeLifeSpeed;	// X: Current size | Y: Life Time left | Z: Speed
	vec4 mPosition;			// XYZ: position | W: active flag (< 0 inactive, else active)
	uint64_t mTexture;
	mat4 mLtwMatrix;		// Local-to-world transformation matrix
};

layout (std430, binding = 6) buffer particleBuffer
{
    Particle particles[];
};

void main()
{
	if (textureSize(sampler2D(fTexture), 0).x > 0)		// Valid texture exists
	{
		vec4 textureColor = texture(sampler2D(fTexture), fTexCoords);
		fragColor = textureColor * fColor;
	}
	else	// No texture, use color instead
	{
		fragColor = fColor;
	}
}
