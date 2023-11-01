#version 450

// -- OUTPUTS --
layout (location = 0) in vec4 vHealthColor;
layout (location = 1) in vec4 vBackColor;
layout (location = 2) in mat4 vLtwMatrix;

// -- OUTPUTS --
layout (location = 0) out vec4 fHealthColor;
layout (location = 1) out vec4 fBackColor;
layout (location = 2) out vec2 fTexCoords;

// -- UNIFORMS --
uniform mat4 uViewProj;
uniform mat4 uFaceCamera;

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

	gl_Position = uViewProj * vLtwMatrix * vec4(vertexPos, 1.f);

	fHealthColor = vHealthColor;
	fBackColor = vBackColor;
}