#version 450
#extension GL_ARB_gpu_shader_int64 : enable


// -- Workgroup Size --
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

// -- Uniform Variables --
uniform int uEmitterCount;
uniform vec3 uCamPos;
uniform float uDeltaTime;

// -- INPUT & OUTPUT --
layout(rgba32f, binding = 0) uniform image2D OutputColor;   // FBO Color Output
layout(rgba32f, binding = 1) uniform image2D Scene;         // Scene to be post-processed

#define ACTIVE 10.f
#define INACTIVE -10.f

struct ParticleEmitter
{
    vec4 mStartColor;
	vec4 mEndColor;
	vec4 mSizeLifetimeSpeed;	// X: start size | Y: end size | Z: Lifetime | W: Speed
	vec4 mPosition;				// XYZ: position | W: particle count
	vec4 mParticlePoolIndex;	// X: Particle pool start index
	uint64_t mTexture;
};

struct Particle
{
	vec4 mColor;
	vec4 mVelocity;
	vec4 mSizeLifeSpeed;	// X: Current size | Y: Life Time left | Z: Speed
	vec4 mPosition;			// XYZ: position | W: active flag (< 0 inactive, else active)
	uint64_t mTexture;
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

// -- Helper Functions --
Particle InitNewParticle(ParticleEmitter e);
void InitVectors(vec3 position);

// -- Global Variables -- 
vec3 rightVector;
vec3 upVector;
vec3 forwardVector;

mat4 ltwMatrix;

void main()
{
	// Each work group will process one emitter
	int emitterIndex = int(gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.x);

	if (emitterIndex >= uEmitterCount)	// Boundary Check
		return;
	
	// Retrieve Emitter from buffer
	ParticleEmitter emitter = emitters[emitterIndex];
	int particleCount = int(emitter.mPosition.w);
	float startSize = emitter.mSizeLifetimeSpeed.x;
	float endSize = emitter.mSizeLifetimeSpeed.y;
	float lifetime = emitter.mSizeLifetimeSpeed.z;
	float speed = emitter.mSizeLifetimeSpeed.w;
	int poolStartIndex = int(emitter.mParticlePoolIndex.x);

	// Initialize vectors 
	InitVectors(emitter.mPosition.xyz);

	// Each thread process one particle
	int particleIndex = int(gl_LocalInvocationID.y * gl_WorkGroupSize.x + gl_LocalInvocationID.x);

	if (particleIndex >= particleCount)
		return;

	// Retrieve existing 
	Particle p = particles[particleIndex + poolStartIndex];

	if (p.mPosition.w < 0)
	{
		p = InitNewParticle(emitter);
		particles[particleIndex + poolStartIndex];
	}
	else
	{
		// Update the properties of particle
		float factor = 1 - (p.mSizeLifeSpeed.y / lifetime);

		p.mColor = mix(emitter.mStartColor, emitter.mEndColor, factor);									// Color
		p.mPosition.xyz += p.mVelocity.xyz * p.mSizeLifeSpeed.z;										// Position
		p.mSizeLifeSpeed.x = mix(emitter.mSizeLifetimeSpeed.x, emitter.mSizeLifetimeSpeed.y, factor);	// Size
		p.mSizeLifeSpeed.y -= uDeltaTime;																// Time Left
	}

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

Particle InitNewParticle(ParticleEmitter e)
{
	// Initialize new particle's properties with emitter's properties
	Particle p;

	// Position, Active Flag
	p.mPosition = e.mPosition;
	p.mPosition.w = ACTIVE;		// Set > 0 value as active

	// Color
	p.mColor = e.mStartColor;
	
	// Size, Life time left, Speed
	p.mSizeLifeSpeed.x = e.mSizeLifetimeSpeed.x;
	p.mSizeLifeSpeed.y = e.mSizeLifetimeSpeed.z;
	p.mSizeLifeSpeed.z = e.mSizeLifetimeSpeed.w;

	// Random velocity with pseudorandom noise function, with thread index as seed
	p.mVelocity = noise4(int(gl_LocalInvocationID.y * gl_WorkGroupSize.x + gl_LocalInvocationID.x));
	p.mVelocity.xyz = normalize(p.mVelocity.x * rightVector + p.mVelocity.y * upVector);	// Make velocity be outwards perpendicular to camera's view							

	// Texture handle
	p.mTexture = e.mTexture;

	return p;
}

void InitVectors(vec3 position)
{
	forwardVector = uCamPos - position;
	rightVector = cross(forwardVector, vec3(0, 1, 0));
	upVector = cross(rightVector, forwardVector);
}