#version 450

// -- Workgroup Size --
layout(local_size_x = 30, local_size_y = 30, local_size_z = 1) in;

uniform float accumulationTime;

// -- INPUT & OUTPUT --
layout(rgba32f, binding = 0) uniform image2D OutputColor;   // FBO Color Output
layout(rgba32f, binding = 1) uniform image2D Scene;         // Scene to be post-processed

vec3 chromaticAbberation(int dist, ivec2 uv, vec3 frag)
{
	frag.r = imageLoad(Scene, ivec2(uv.x + dist, uv.y)).r;
    frag.g = imageLoad(Scene, ivec2(uv.x - dist, uv.y)).g;
    frag.b = imageLoad(Scene, ivec2(uv.x, uv.y)).b;
    return frag;
}

vec3 crt(vec3 frag)
{
	// average out the brightness
    float brightness = (frag.r + frag.g + frag.b) / 3.0;
    float mul = 0.1 * brightness * brightness + 0.85;
    return frag * mul;
}

ivec2 distort(ivec2 uv, int t, int tex_offset)
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
    ivec2 iResolution = imageSize(Scene);
    ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	int tex_offset = int(0.001 * iResolution.x);
	int height = int(iResolution.y / 100);
    
    ivec2 newUV = uv;
    int y_coord = int(gl_GlobalInvocationID.y + accumulationTime * 40.0) % (height * 2);
    if(y_coord < height)
	{
        newUV = distort(uv, y_coord, tex_offset);
    }

    vec4 color = vec4(1.0); 
    int dist = int(0.0025 * iResolution.x);
	color.rgb = chromaticAbberation(dist, newUV, color.rgb);

    // Performing Blending with sFactor = GL_ONE_MINUS_DST_COLOR
    // Performing Blending with dFactor = GL_ONE
        // vec3 destColor = imageLoad(Scene, uv).rgb;
        // vec3 srcFactor = vec3(1.0) - destColor;
        // color.rgb = color.rgb * srcFactor + destColor.rgb;

    if(y_coord < height) {
		color.rgb *= 0.65;
        // Write to image output
        imageStore(OutputColor, uv, color);
        return;
    }

    // Write to image output
	color.rgb = crt(color.rgb * 0.9);
    imageStore(OutputColor, uv, color);
}
