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
    float brightness = (frag.r + frag.g + frag.b) / 3.0;
    float mul = 0.1 * brightness * brightness + 0.85;
    return frag * mul;
}

vec2 distort(vec2 uv, int t)
{
    if (t == 0)
        uv.x -= 0.001;
    else if (t == 1)
        uv.x -= 0.002;
    else if (t == 2)
        uv.x -= 0.003;
    else if (t == 3)
        uv.x -= 0.002;
    else
        uv.x -= 0.001;
    return uv;
}

void main()
{
    vec2 iResolution = textureSize(Scene, 0);
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    
    int y_coord = int(gl_FragCoord.y + accumulationTime * 40.0) % 12;
    if(y_coord < 6)
	{
        uv = distort(uv, y_coord);
    }

	FragColor.rgb = chromaticAbberation(0.006, uv, FragColor.rgb);

    if(y_coord < 6) {
        return;
    }

	FragColor.rgb = crt(FragColor.rgb);
}
