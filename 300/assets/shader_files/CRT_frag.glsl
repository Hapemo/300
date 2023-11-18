#version 450

// in ==
in vec2 TexCoords;

// == out 
out vec4 FragColor;

uniform sampler2D Scene;
uniform float accumulationTime;

vec3 chromaticAbberation(float dist, vec2 uv, vec3 frag)
{
	frag.r = texture(Scene, vec2(uv.x + dist, uv.y)).r;
    frag.g = texture(Scene, vec2(uv.x - dist, uv.y)).g;
    frag.b = texture(Scene, vec2(uv.x, uv.y)).b;
    return frag;
}

vec3 crt(vec3 frag)
{
	// average out the brightness
    float brightness = (frag.r + frag.g + frag.b) / 3.0;
    float mul = 0.1 * brightness * brightness + 0.85;
    return frag * mul;
}

vec2 distort(vec2 uv, int t, float tex_offset)
{
    if (t == 0)
        uv.x -= tex_offset;
    else if (t == 1)
        uv.x -= tex_offset * 2;
    else if (t == 2)
        uv.x -= tex_offset * 3;
    else if (t == 3)
        uv.x -= tex_offset * 2;
    else
        uv.x -= tex_offset * 1;
    return uv;
}

void main()
{
    vec2 iResolution = textureSize(Scene, 0);
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
	float tex_offset = 0.001;
	int height = int(iResolution.y / 50);
    
    int y_coord = int(gl_FragCoord.y + accumulationTime * 40.0) % (height * 2);
    if(y_coord < height)
	{
        uv = distort(uv, y_coord, tex_offset);
    }

	FragColor.rgb = chromaticAbberation(0.004, uv, FragColor.rgb);

    if(y_coord < height) {
		FragColor.rgb *= 0.7;
        return;
    }

	FragColor.rgb = crt(FragColor.rgb);
}
