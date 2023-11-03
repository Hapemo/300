#version 450

// IN
layout (location = 0) in vec2 fTexCoords;

// OUT
layout (location = 0) out vec4 fragColor;

// UNIFORM
uniform float uDegrees;

// Global
vec2 up = vec2(0, 0.5);

float GetAngle(vec2 v)
{
	float angle = dot(up, v) / (length(up) * length(v));

	if (v.x < 0)
		return radians(360.0) - acos(angle);

	return acos(angle);
}
  
void main()
{   
	// Discard any fragment not part of the pie
	if (GetAngle(fTexCoords - vec2(0.5)) < uDegrees)
		return;

	// Discard any fragment outside of circle
	if (distance(fTexCoords, vec2(0.5)) >= 0.5)
		return;

	// Render only the Perimeter of Circle
	if (distance(fTexCoords, vec2(0.5)) <= 0.48)
		return;

	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
