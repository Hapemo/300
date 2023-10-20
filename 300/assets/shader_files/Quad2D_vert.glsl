#version 450

// OUT
out vec2 TexCoords;

// IN  
in vec2 inPosition;
in vec2 inTexCoords;

uniform sampler2D image;
  
void main()
{             
	gl_Position = vec4(inPosition, 0.0, 1.0);
	TexCoords = inTexCoords;
}