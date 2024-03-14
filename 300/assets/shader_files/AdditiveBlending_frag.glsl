#version 450

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D Scene;
uniform sampler2D BloomBlur;
uniform float Exposure;

void main()
{
	//const float gamma = 2.2;
	vec3 hdrColor = texture(Scene, TexCoords).rgb;
	vec3 bloomColor = texture(BloomBlur, TexCoords).rgb;

	// additive blending
	hdrColor += bloomColor;	

	// tone mapping
	vec3 result = vec3(1.0) - exp(-hdrColor * Exposure);

	// gamma correction to SRGB
	//result = pow(result, vec3(1.0 / gamma));

	FragColor = vec4(result, 1.0);
}
