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
	vec4 mCurrColor;
	vec4 mVelocity;
	vec4 mSizeLife;			// X: Start size | Y: End size | Z: Life Time left | W: Max Life
	vec4 mPositionSpeed;	// XYZ: position | W: Speed
	vec4 mRotationGravity;	// X: Current Rotation | Y: Rotation Delta | Z: Gravity
	uint64_t mTexture;
	mat4 mLtwMatrix;		// Local-to-world transformation matrix
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
void UpdateParticle(inout Particle p, int index);
void InitVectors(vec3 position);
mat4 Rotate(float degree);

// -- Global Variables -- 
vec3 rightVector;
vec3 upVector;
vec3 forwardVector;

void main()
{
	// Each thread updates 1 particle
	ivec2 globalID = ivec2(gl_GlobalInvocationID.xy);
	ivec2 globalSize = ivec2(gl_NumWorkGroups.xy) * ivec2(gl_WorkGroupSize.xy);
	int particleIndex = globalID.y * globalSize.x + globalID.x;

	// Boundary check
	if (particleIndex >= uParticleCount)
		return;
	
	UpdateParticle(particles[particleIndex], particleIndex);
}

void UpdateParticle(inout Particle p, int index)
{
	float lifeLeft = p.mSizeLife.z;
	float maxLife = p.mSizeLife.w;

	if (lifeLeft > 0)	// Life more than 0 means active, to be updated
	{
		InitVectors(p.mPositionSpeed.xyz);
		float factor = 1 - (lifeLeft / maxLife);

		p.mCurrColor = mix(p.mStartColor, p.mEndColor, factor);							// Color
		p.mVelocity.xyz = normalize(p.mVelocity.xyz + p.mRotationGravity.z * upVector);	// Update velocity with gravity
		p.mPositionSpeed.xyz += p.mVelocity.xyz * p.mPositionSpeed.w * uDeltaTime;		// Position
		float size = mix(p.mSizeLife.x, p.mSizeLife.y, factor);							// Size
		p.mSizeLife.z -= uDeltaTime;													// Life left
		p.mRotationGravity.x += p.mRotationGravity.y * uDeltaTime;						// Rotation


		mat4 scale = mat4(
			vec4(size, 0.0, 0.0, 0.0),
			vec4(0.0, size, 0.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 0.0, 1.0)
		);

		mat4 rotate = Rotate(p.mRotationGravity.x);

		mat4 ltwMatrix = mat4(
			vec4(rightVector, 0.0),
			vec4(upVector, 0.0),
			vec4(forwardVector, 0.0),
			vec4(p.mPositionSpeed.xyz, 1.0)
		);

		ltwMatrix = ltwMatrix * rotate * scale;
		p.mLtwMatrix = ltwMatrix;

		if (p.mSizeLife.z < 0)	// "dead" particles, add index into freelist
		{
			p.mCurrColor.a = 0;		// Set transparent
			freelist.indices[atomicAdd(freelist.count, 1)] = index;
		}
	}
}

void InitVectors(vec3 position)
{
	// Compute the rotation vectors
	forwardVector	= normalize(uCamPos - position);
	rightVector		= normalize(cross(forwardVector, vec3(0, 1, 0)));
	upVector		= normalize(cross(rightVector, forwardVector));
}

mat4 Rotate(float degree)
{
	float rad = radians(degree);
	float s = sin(rad);
	float c = cos(rad);

	mat4 r = mat4(
		vec4(c, s, 0, 0),
		vec4(-s, c, 0, 0),
		vec4(0, 0, 1, 0),
		vec4(0, 0, 0, 1)
	);
	return r;
}