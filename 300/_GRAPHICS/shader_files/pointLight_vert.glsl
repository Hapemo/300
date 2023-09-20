#version 450

layout (location = 0) in vec3 inQPos;		        // INPUT_POSITION

layout (location = 1) in vec4 inVertexColor;        // input color
layout (location = 2) in vec2 inQUV;			    // Texture Coordinates
layout (location = 5) in mat4 inLTW;			    // local to world

uniform mat4 uMatrixVP;

out vec4 VertexColor;
out vec2 TexCoords;
out mat4 LTW;
out vec4 WorldPos;

void main() 
{
    // Decompress the binormal & transform everything to world space
    // mat3 Rot                    = mat3(inLTW);

	 // Position
    vec4 Pos = vec4(inQPos.xyz, 1.0f);

    // Set all the output vars
    VertexColor                 = inVertexColor;
    vec4 vWorldPos              = uMatrixVP * inLTW * Pos;
    gl_Position                 = vWorldPos;
    TexCoords                   = inQUV;
    LTW                         = inLTW;
    WorldPos                    = inLTW * Pos;
}