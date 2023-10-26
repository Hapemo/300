#version 450

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D Scene;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(Scene, TexCoords).rgb;

	// gamma correction to SRGB
	hdrColor = pow(hdrColor, vec3(1.0 / gamma));

	FragColor = vec4(hdrColor, 1.0);
}
