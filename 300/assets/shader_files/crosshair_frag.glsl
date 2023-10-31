#version 450

// -- IN --
layout (location = 2) in vec2 fTexCoords;

//-- OUT --
layout (location = 0) out vec4 fragColor;

uniform float uThickness;
uniform float uInner;
uniform float uOuter;

void main()
{
	vec2 dir = abs(fTexCoords - vec2(0.5));
	float epsilon = uThickness * 0.01;
	if (dir.x + epsilon < dir.y || dir.x > dir.y + epsilon)
		return;

	float innerLimit = uInner * 0.01;
	float outerLimit = uOuter * 0.01;

	// Discard any fragment inside of circle
	if (distance(fTexCoords, vec2(0.5)) >= outerLimit)
		return;
	// Discard any fragment inside of circle
	if (distance(fTexCoords, vec2(0.5)) <= innerLimit)
		return;

	fragColor = vec4(1.0);
}