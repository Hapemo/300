#version 450

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D Scene;
uniform float ChromaticAbberationStrength;

void main()
{
	const float gamma = 2.2;

	// Chromatic abberation
	float red		= texture(Scene, vec2(TexCoords.x - ChromaticAbberationStrength, TexCoords.y)).r;
	float green		= texture(Scene, vec2(TexCoords.x, TexCoords.y)).g;
	float blue		= texture(Scene, vec2(TexCoords.x + ChromaticAbberationStrength, TexCoords.y)).b; 
	FragColor		= vec4( texture(Scene, TexCoords - ChromaticAbberationStrength).x,
							texture(Scene, TexCoords).x,
							texture(Scene, TexCoords + ChromaticAbberationStrength).x,
							1.0
							);
							
	//FragColor		= vec4(red, green, blue, 1.0);
	//vec4 color		= vec4(red, green, blue, 1.0);
	
	// gamma correction to SRGB
	//FragColor = pow(color, vec4(1.0 / gamma));
}
