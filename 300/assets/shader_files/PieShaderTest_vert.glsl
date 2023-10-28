#version 450

// -- INPUTS --

// -- OUTPUTS --
layout (location = 0) out vec2 fTexCoords;

vec2 CreateQuad(int vertexID)
{
	uint b = 1 << vertexID;
	float u = int((0x3 & b) != 0);
	float v = int((0x9 & b) != 0);
	return vec2(u, v);
}

void main()
{
	fTexCoords = CreateQuad(gl_VertexID);
	vec3 vertexPos = vec3(fTexCoords - vec2(0.5f), 0.0f);

	gl_Position = vec4(vertexPos, 1.f);
}
