#version 450
//#extension GL_ARB_separate_shader_objects  : enable
//#extension GL_ARB_shading_language_420pack : enable

in vec4 VertexColor;
in vec2 TexCoords;

uniform sampler2D uTex;

out vec4 FragColor;

//------------------- MAIN FUNCTION -----------------------------
void main() 
{
	vec4 texturecolor = texture(uTex, TexCoords);
	FragColor = texturecolor;
}	


