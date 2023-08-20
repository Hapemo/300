#version 450

//#extension GL_ARB_separate_shader_objects  : enable
//#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 inQPos;		        // INPUT_POSITION

layout (location = 1) in vec4 inVertexColor;        // input color
layout (location = 2) in mat4 inLTW;			    // local to world

uniform vec3 posDecompressionScale;
uniform vec3 posDecompressionOffset;
uniform vec2 uvDecompressionScale;
uniform vec2 uvDecompressionOffset;
uniform vec4 uMatrixVP;

out vec4 VertexColor;    

void main() 
{
    // Decompress the binormal & transform everything to world space
    mat3 Rot                    = mat3(inLTW);

	 // Decompress Position
    vec4 Pos = vec4( posDecompressionScale * inQPos.xyz + posDecompressionOffset, 1.0f);

    // Decompress UVs
    //Out.UV   = pushConsts.UVDecompression.xy * inQUV + pushConsts.UVDecompression.zw;

    // Set all the output vars
    VertexColor                 = inVertexColor;
    gl_Position                 = uMatrixVP * inLTW * inQPos;
    //gl_Position                 = uMatrixVP * inLTW * Pos;
}
