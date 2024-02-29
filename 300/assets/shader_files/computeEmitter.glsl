#version 450
#extension GL_ARB_gpu_shader_int64 : enable


// -- Workgroup Size --
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

// -- Uniform Variables --
uniform int uEmitterCount;
uniform vec3 uCamPos;

struct ParticleEmitter
{
    vec4 mStartColor;
	vec4 mEndColor;
	vec4 mSizeLifetimeCount;	// X: start size | Y: end size | Z: Lifetime | W: Count
	vec4 mPositionSpeed;		// XYZ: position | W: Speed
	//uint64_t mTexture;
};

struct Particle
{
	vec4 mStartColor;
	vec4 mEndColor;
	vec4 mCurrColor;
	vec4 mVelocity;
	vec4 mSizeLife;			// X: Start size | Y: End size | Z: Life Time left | W: Max Life
	vec4 mPositionSpeed;	// XYZ: position | W: Speed
	//uint64_t mTexture;
	mat4 mLtwMatrix;		// Local-to-world transformation matrix
};

layout (std430, binding = 5) buffer emittersBuffer
{
    ParticleEmitter emitters[];
};

layout (std430, binding = 6) buffer particleBuffer
{
    Particle particles[];
};

layout (std430, binding = 7) buffer indexBuffer
{
    int count;
	int indices[];
}freelist;

// -- Helper Functions --
void MakeParticle(out Particle p, ParticleEmitter e);
void InitVectors(vec3 position);

// -- Global Variables -- 
vec3 rightVector;
vec3 upVector;
vec3 forwardVector;

void main()
{
	// Each thread group will process one emitter
	int emitterIndex = int(gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.x);

	if (emitterIndex >= uEmitterCount)	// Boundary Check
		return;
	
	// Retrieve Emitter from buffer
	ParticleEmitter emitter = emitters[emitterIndex];
	int particleCount = int(emitter.mSizeLifetimeCount.w);
	InitVectors(emitter.mPositionSpeed.xyz);

	int threadIndex = int(gl_LocalInvocationID.y * gl_WorkGroupSize.x + gl_LocalInvocationID.x);
	if (threadIndex < particleCount)
	{
		// undo decrement and return if nothing in freelist
		int index = atomicAdd(freelist.count, -1) - 1;
		if (index < 0)
		{
			atomicAdd(freelist.count, 1);
			return;
		}
		int particleIndex = freelist.indices[index];
		MakeParticle(particles[particleIndex], emitter);
	}
}

void MakeParticle(out Particle p, ParticleEmitter e)
{
	// Position, Speed
	p.mPositionSpeed = e.mPositionSpeed;

	// Color
	p.mStartColor = e.mStartColor;
	p.mEndColor = e.mEndColor;
	
	// Size, Life time left
	p.mSizeLife.x = e.mSizeLifetimeCount.x;		// start size
	p.mSizeLife.y = e.mSizeLifetimeCount.y;		// end size
	p.mSizeLife.z = e.mSizeLifetimeCount.z;		// life left
	p.mSizeLife.w = e.mSizeLifetimeCount.z;		// life time

	// Random velocity with pseudorandom noise function, with thread index as seed
	p.mVelocity.xyz = noise3(int(gl_LocalInvocationID.y * gl_WorkGroupSize.x + gl_LocalInvocationID.x));
	p.mVelocity.xyz = normalize(p.mVelocity.x * rightVector + p.mVelocity.y * upVector);	// Make velocity be outwards perpendicular to camera's view							

	// Texture handle
	//p.mTexture = e.mTexture;
}

void InitVectors(vec3 position)
{
	forwardVector = uCamPos - position;
	rightVector = cross(forwardVector, vec3(0, 1, 0));
	upVector = cross(rightVector, forwardVector);
}