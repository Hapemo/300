#version 450
//#extension GL_ARB_separate_shader_objects  : enable
//#extension GL_ARB_shading_language_420pack : enable

in vec4 VertexColor;

out vec4 FragColor;

//------------------- MAIN FUNCTION -----------------------------
void main() 
{
	FragColor = VertexColor;
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}


