#version 450

// -- INPUTS --
layout (location = 0) in vec4 fHealthColor;		// r, g, b: Health color | a: Health value
layout (location = 1) in vec4 fBackColor;		// R: texture ID | G: Contains texture if < 0 | B: Frame UI if > 0)
layout (location = 2) in vec2 fTexCoords;

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 entityID;

uniform sampler2D uTex2d[32];

void main()
{
	if (fBackColor.b > 0)	// Render frame UI
	{
		fragColor = texture(uTex2d[int(fBackColor.x)], fTexCoords);
	}
	else
	{
		float health = fHealthColor.a * 0.01;

		if (fTexCoords.x > health)
			fragColor = vec4(fHealthColor.rgb, fBackColor.a);
		else
		{
			if (fBackColor.g < 0)	// Have UI texture
				fragColor = texture(uTex2d[int(fBackColor.x)], fTexCoords);
			else
				fragColor = fBackColor;
		}
	}
}