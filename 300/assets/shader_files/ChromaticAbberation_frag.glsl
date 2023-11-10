#version 450

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D Scene;
uniform float ChromaticAbberationOffset;
uniform float ChromaticAbberationStrength;

void main()
{
	const float gamma = 2.2;

	// Chromatic abberation
	float red		= texture(Scene, TexCoords - ChromaticAbberationOffset).r * ChromaticAbberationStrength;
	float green		= texture(Scene, TexCoords).g * ChromaticAbberationStrength;
	float blue		= texture(Scene, TexCoords + ChromaticAbberationOffset).b * ChromaticAbberationStrength; 


	FragColor		= vec4( red, green, blue, 0.5 );
	//FragColor		= vec4( 0.0, 0.0, 0.0, 1.0 );
}
