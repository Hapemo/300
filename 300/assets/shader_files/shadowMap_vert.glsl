#version 450
//#extension GL_ARB_separate_shader_objects  : enable
//#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 inPos;		        // INPUT_POSITION
layout(location = 9) in mat4 inLTW;			    // local to world

uniform mat4 uLightSpaceMatrix;

//------------------- MAIN FUNCTION -----------------------------
void main() 
{
	gl_Position = uLightSpaceMatrix * inLTW * vec4(inPos, 1.0);
}	


