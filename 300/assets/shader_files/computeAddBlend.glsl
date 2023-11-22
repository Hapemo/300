#version 450

// -- Workgroup Size --
layout(local_size_x = 30, local_size_y = 30, local_size_z = 1) in;

// -- INPUT & OUTPUT --
layout(rgba32f, binding = 0) uniform image2D OutputColor;   // FBO Color Output
layout(rgba32f, binding = 1) uniform image2D Scene;         // Scene to be post-processed
layout(rgba32f, binding = 2) uniform image2D BloomBlur;     // Attachment containing BloomBlur

uniform float Exposure;

void main()
{
	const float gamma = 2.2;
	ivec2 texelCoords = ivec2(gl_GlobalInvocationID.xy);
	vec3 hdrColor = imageLoad(Scene, texelCoords).rgb;
	vec3 bloomColor = imageLoad(BloomBlur, texelCoords).rgb;

	// additive blending
	hdrColor += bloomColor;	

	// tone mapping
	vec3 result = vec3(1.0) - exp(-hdrColor * Exposure);

	// gamma correction to SRGB
	//result = pow(result, vec3(1.0 / gamma));

	// Blend the destination color and source color
	// source and destination blend factor of GL_ONE
	vec4 dstColor = imageLoad(OutputColor,texelCoords);
	result = result + dstColor.rgb;

	imageStore(OutputColor, texelCoords, vec4(result, 1.0));
}
