#version 450

layout (location = 0) in vec3 inPos;		        // INPUT_POSITION

//layout (location = 1) in vec4 inVertexColor;        // input color
layout (location = 2) in vec2 inUV;			        // Texture Coordinates
layout (location = 5) in vec3 inTangent;			// Per vertex Tangent
layout (location = 6) in vec3 inNormal;			    // Per vertex Normal
layout (location = 7) in vec4 inTex_Ent_ID;		    // Texture ID, Entity ID of object
layout (location = 8) in mat4 inLTW;			    // local to world

//uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform mat4 uMatrixVP;
uniform bool uHasLight;

out vec4 VertexColor;       // for debugging
out vec2 TexCoords;
out mat3 TBN;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec4 Tex_Ent_ID;

void main() 
{
	// Position
    VertexColor = vec4(0.f);
    vec4 Pos = vec4(inPos, 1.0f);
    gl_Position = uMatrixVP * inLTW * Pos;
    TexCoords = inUV;
    Tex_Ent_ID      = inTex_Ent_ID;

    if (!uHasLight) return;
    
    // Compute world-to-tangent space matrix
    mat3 normalMatrix = transpose(inverse(mat3(inLTW)));
    vec3 T = normalize(normalMatrix * inTangent);
    vec3 N = normalize(normalMatrix * inNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = transpose(mat3(T, B, N));

    // Transform position to tangent space
    TangentViewPos  = TBN * uViewPos;
    TangentFragPos  = TBN * vec3(inLTW * vec4(inPos, 1.0));
}