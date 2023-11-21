#version 450

// -- INPUTS --
layout (location = 0) in vec4 vColor;
layout (location = 1) in vec4 vTexId_EntId;		// x: texture Index | y: entity ID | z: empty | w: empty
layout (location = 2) in mat4 vLtwMatrix;

// -- OUTPUTS --
layout (location = 0) out vec4 fColor;
layout (location = 1) out vec4 fTexId_EntId;
layout (location = 2) out vec2 fTexCoords;

// -- UNIFORMS --
uniform mat4 uMatrixVP;

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

	gl_Position = uMatrixVP * vLtwMatrix * vec4(vertexPos, 1.f);

	fColor = vColor;
	fTexId_EntId = vTexId_EntId;
}
