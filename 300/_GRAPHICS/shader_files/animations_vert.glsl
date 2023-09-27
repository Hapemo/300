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
layout (location = 7) in vec4   inTex_Ent_ID;		// Texture ID, Entity ID of object
layout (location = 8) in mat4   inLTW;			// local to world

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];

uniform mat4 uMatrixVP;     // view projection
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform bool uHasLight;

out vec2 TexCoords;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec4 Tex_Ent_ID;

void main() 
{
	// Position
    vec4 totalPosition = vec4(0.f);
    vec3 normal = vec3(0.0f);

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

        vec4 localPosition = finalBoneMatrices[boneId] * vec4(inQPos, 1.0f);
        totalPosition += localPosition * boneWeights[i];
        normal = mat3(finalBoneMatrices[boneId]) * inNormal;
    }

    gl_Position         = uMatrixVP * inLTW * totalPosition;
    TexCoords           = inQUV;
    Tex_Ent_ID          = inTex_Ent_ID;
    if (!uHasLight) return;

    
    // Compute world-to-tangent space matrix
    mat3 normalMatrix = transpose(inverse(mat3(inLTW)));
    vec3 T = normalize(normalMatrix * inTangent);
    vec3 N = normalize(normalMatrix * inNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));


    // Set all the output vars
    //gl_Position         = uMatrixVP * inLTW * vec4(inQPos,1.0);
    TangentLightPos     = TBN * uLightPos;
    TangentViewPos      = TBN * uViewPos;
    TangentFragPos      = TBN * vec3(inLTW * totalPosition);
}
