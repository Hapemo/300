#version 450

//#extension GL_ARB_separate_shader_objects  : enable
//#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3   inQPos;		        // INPUT_POSITION

layout (location = 1) in vec4   inVertexColor;      // input color
layout (location = 2) in vec2   inQUV;			    // Texture Coordinates
layout (location = 3) in vec4   boneIds;			// Bone IDs
layout (location = 4) in vec4   boneWeights;		// Bone Weights
layout (location = 5) in vec3   inTangent;			// Per vertex Tangent
layout (location = 6) in vec3   inNormal;			// Per vertex Normal
layout (location = 7) in vec4   inTex_Ent_ID;		// x: empty | y: Entity ID | z: Animation Instance ID | w: empty
layout (location = 8) in vec4   inBloom;		    // Bloom threshold and flag
layout (location = 9) in mat4   inLTW;			    // local to world

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 uMatrixVP;     // view projection

layout (std430, binding = 3) buffer boneFinal
{
  mat4 matrices[];
};

out vec4 vertexColor;
out vec2 TexCoords;
out vec4 Tex_Ent_ID;
out mat3 TBN;
out vec3 fragPos;
out vec4 Bloom_Threshold;

mat3 ComputeTBN()
{

	vec3 T = normalize(vec3(inLTW * vec4(inTangent, 0.0)));
	vec3 N = normalize(vec3(inLTW * vec4(inNormal, 0.0)));
	vec3 B = normalize(cross(N, T));

	return mat3(T, B, N);
}

void main() 
{
	// Position
    vec4 totalPosition = vec4(0.f);
    vec3 normal = vec3(0.0f);

    int animInstanceID = int(inTex_Ent_ID.z);

    for(int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        int boneId = int(boneIds[i]);
        // find an initialized bone
        if(boneId < 0) {
			continue;
        }

        // if the bone found is invalid, set a default position value
        if(boneId >= MAX_BONES) {
            totalPosition = vec4(inQPos, 1.0f);
            break;
        }

        int boneIndex = boneId + animInstanceID * MAX_BONES;

        vec4 localPosition = matrices[boneIndex] * vec4(inQPos, 1.0f);
        totalPosition += localPosition * boneWeights[i];
        normal = mat3(matrices[boneIndex]) * inNormal;
    }

    gl_Position         = uMatrixVP * inLTW * totalPosition;
    TexCoords           = inQUV;
    Tex_Ent_ID          = inTex_Ent_ID;
    vertexColor         = inVertexColor;

    TBN = ComputeTBN();

    // Set all the output vars
    fragPos  = vec3(inLTW * totalPosition);
    Bloom_Threshold = inBloom;
}
