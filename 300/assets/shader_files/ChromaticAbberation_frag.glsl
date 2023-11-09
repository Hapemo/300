#version 450

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D Scene;
uniform float ChromaticAbberationStrength;

void main()
{
	const float gamma = 2.2;

	// Chromatic abberation
	float red		= texture(Scene, TexCoords - ChromaticAbberationStrength).r;
	float green		= texture(Scene, TexCoords).g;
	float blue		= texture(Scene, TexCoords + ChromaticAbberationStrength).b; 
	
	
	red		= pow(red, 1.0/gamma);
	blue	= pow(blue, 1.0/gamma);
	green	= pow(green, 1.0/gamma);


	FragColor		= vec4( red, green, blue, 0.5 );
							
	//FragColor		= vec4(red, green, blue, 1.0);
	//vec4 color		= vec4(red, green, blue, 1.0);
	
	// gamma correction to SRGB
	//FragColor = pow(color, vec4(1.0 / gamma));
}
