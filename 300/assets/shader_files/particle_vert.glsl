#version 450
#extension GL_ARB_gpu_shader_int64 : enable

// -- OUTPUTS --
layout (location = 0) out vec4 fColor;
layout (location = 1) out vec2 fTexCoords;
layout (location = 2) out uint64_t fTexture;

// -- UNIFORMS --
uniform mat4 uMatrixVP;

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

vec2 CreateQuad(int vertexID)
{
	uint b = 1 << vertexID;
	float u = int((0x3 & b) != 0);
	float v = int((0x9 & b) != 0);
	return vec2(u, v);
}

void main()
{
	fTexCoords = CreateQuad(gl_VertexID);
	vec3 vertexPos = vec3(fTexCoords - vec2(0.5f), 0.0f);

	Particle p = particles[gl_InstanceID];

	gl_Position = uMatrixVP * p.mLtwMatrix * vec4(vertexPos, 1.0);
	fColor = p.mColor;
	fTexture = p.mTexture;
}
