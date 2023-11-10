#version 450

// -- INPUTS --
layout (location = 0) in vec3 inPos;		        // INPUT_POSITION
layout (location = 1) in vec4 inVertexColor;        // input color
layout (location = 2) in vec2 inUV;			        // Texture Coordinates
layout (location = 5) in vec3 inTangent;			// Per vertex Tangent
layout (location = 6) in vec3 inNormal;			    // Per vertex Normal
layout (location = 7) in vec4 inTex_Ent_ID;		    // Texture ID, Entity ID of object
layout (location = 8) in mat4 inLTW;			    // local to world

// -- UNIFORMS --
uniform vec3 uViewPos;
uniform mat4 uMatrixVP;

// -- OUTPUTS --
out vec2 TexCoords;			// Texture Coordinates
out vec4 Tex_Ent_ID;		// Texture ID, Entity ID
out mat3 TBN;				// Tangent-to-world matrix
out vec3 fragPos;			// Fragment position


mat3 ComputeTBN()
{

	vec3 T = normalize(vec3(inLTW * vec4(inTangent, 0.0)));
	vec3 N = normalize(vec3(inLTW * vec4(inNormal, 0.0)));
	vec3 B = normalize(cross(T, N));

	return mat3(T, B, N);
}

void main()
{
	// Position
	vec4 worldPos = inLTW * vec4(inPos, 1.0f);
    gl_Position = uMatrixVP * worldPos;
    TexCoords	= inUV;
    Tex_Ent_ID  = inTex_Ent_ID;

	TBN = ComputeTBN();
	fragPos = worldPos.xyz;
}