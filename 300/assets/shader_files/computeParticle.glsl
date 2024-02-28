#version 450
#extension GL_ARB_gpu_shader_int64 : enable


// -- Workgroup Size --
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

// -- Uniform Variables --
uniform int uParticleCount;
uniform float uDeltaTime;
uniform vec3 uCamPos;

struct Particle
{
	vec4 mStartColor;
	vec4 mEndColor;
	vec4 mVelocity;
	vec4 mSizeLifeSpeed;	// X: Start size | Y: End size | Z: Life Time left | W: Speed
	vec4 mPosition;			// XYZ: position | W: Max Life time
	uint64_t mTexture;
	mat4 mLtwMatrix;		// Local-to-world transformation matrix
};

layout (std430, binding = 6) buffer particleBuffer
{
    Particle particles[];
};

// -- Helper Functions --
void InitVectors(vec3 position)

// -- Global Variables -- 
vec3 rightVector;
vec3 upVector;
vec3 forwardVector;

mat4 ltwMatrix;

void main()
{
	// Each thread updates 1 particle
	ivec2 globalID = ivec2(gl_GlobalInvocationID.xy);
	ivec2 localID = ivec2(gl_LocalInvocationID.xy);
	ivec2 globalSize = ivec2(gl_NumWorkGroups.xy) * ivec2(gl_WorkGroupSize.xy);
	int particleIndex = globalID.y * globalSize.x + globalID.x;

	// Boundary check
	if (particleIndex >= uParticleCount)
		return;

	// Retrieve existing particle
	Particle p = particles[particleIndex];

	float lifetime = p.mPosition.w;
	float lifeLeft = p.mSizeLifeSpeed.z;

	// Update the properties of particle
	float factor = 1 - (lifeLeft / lifetime);

	p.mColor = mix(p.mStartColor, p.mEndColor, factor);									// Color
	p.mPosition.xyz += p.mVelocity.xyz * p.mSizeLifeSpeed.z;										// Position
	p.mSizeLifeSpeed.x = mix(emitter.mSizeLifetimeSpeed.x, emitter.mSizeLifetimeSpeed.y, factor);	// Size
	p.mSizeLifeSpeed.y -= uDeltaTime;																// Time Left

	mat4 scale = mat4(
		vec4(p.mSizeLifeSpeed.x, 0.0, 0.0, 0.0),
		vec4(0.0, p.mSizeLifeSpeed.x, 0.0, 0.0),
		vec4(0.0, 0.0, 1.0, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)
	);

	ltwMatrix = mat4(
		vec4(rightVector, 0.0),
		vec4(upVector, 0.0),
		vec4(forwardVector, 0.0),
		vec4(p.mPosition.xyz, 1.0)
	);

	ltwMatrix = ltwMatrix * scale;
	p.mLtwMatrix = ltwMatrix;

	// Write back updated particles to buffer
	particles[particleIndex + poolStartIndex] = p;
}

void InitVectors(vec3 position)
{
	forwardVector = uCamPos - position;
	rightVector = cross(forwardVector, vec3(0, 1, 0));
	upVector = cross(rightVector, forwardVector);
}