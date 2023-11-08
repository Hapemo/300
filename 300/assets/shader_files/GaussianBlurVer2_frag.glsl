#version 450

// OUT
layout (location = 0) out vec4 FragColor0;
layout (location = 1) out vec4 FragColor1;

// IN  
in vec2 TexCoords;

uniform sampler2D image;
uniform float SamplingWeight;
  
vec2 iResolution = textureSize(image, 0);
vec2 uv = gl_FragCoord.xy / iResolution.xy;

float SCurve (float x) 
{
	x = x * 2.0 - 1.0;
	return -x * abs(x) * 0.5 + x + 0.5;
}

vec4 BlurH (sampler2D source, vec2 size, vec2 uv, float radius) {

	if (radius >= 1.0)
	{
		vec4 A = vec4(0.0); 
		vec4 C = vec4(0.0); 

		float width = 1.0 / size.x;
		float height = 1.0 / size.y;

		float divisor = 0.0; 
        float weight = 0.0;
        
        float radiusMultiplier = 1.0 / radius;
        
 		for (float firstpass = -radius; firstpass <= radius; firstpass++)
		{
			A = texture(source, uv + vec2(firstpass * width, firstpass * height));

			weight = SCurve(1.0 - (abs(firstpass) * radiusMultiplier)); 
            C += A * weight; 
			divisor += weight; 
		}

		for (float secondpass = -radius; secondpass <= radius; secondpass++)
		{
			A = texture(source, uv - vec2(secondpass * width, -secondpass * height));
            
			weight = SCurve(1.0 - (abs(secondpass) * radiusMultiplier)); 
            C += A * weight; 
			divisor += weight; 
		}

		return vec4(C.r / divisor, C.g / divisor, C.b / divisor, 1.0);
	}

	return texture(source, uv);
}


void main()
{             
	FragColor0 = BlurH(image, iResolution.xy, uv, SamplingWeight);
}


// BOX BLUR
/*
	vec2 iResolution = textureSize(image, 0);

	vec2 uv = gl_FragCoord.xy / iResolution.xy;
	vec3 sum = vec3(0);
    float pixelSize = 1.0 / iResolution.x; 
    
    // Horizontal Blur
    vec3 accumulation = vec3(0);
    vec3 weightsum = vec3(0);

    for (float i = -kernel; i <= kernel; i++)
	{
        accumulation += texture(image, uv + vec2(i * pixelSize, i * pixelSize)).xyz * SamplingWeight;
        weightsum += SamplingWeight;
    }
    
    sum = accumulation / weightsum;
    
    FragColor0 = vec4(sum, 1.0);
*/

// GAUSSIAN BLUR V2
/*
	float normpdf(in float x, in float sigma)
	{
		return SamplingWeight * exp(-0.5*x*x/(sigma*sigma))/sigma;
	}
	vec2 resolution = textureSize(image, 0);

	//declare stuff
	const int mSize = 11;
	const int kSize = (mSize-1)/2;
	float kernel[mSize];
	vec3 final_colour = vec3(0.0);
		
	//create the 1-D kernel
	float sigma = 7.0;
	float Z = 0.0;
	for (int j = 0; j <= kSize; ++j)
	{
		kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
	}
		
	//get the normalization factor (as the gaussian has been clamped)
	for (int j = 0; j < mSize; ++j)
	{
		Z += kernel[j];
	}
		
	//read out the texels
	for (int i=-kSize; i <= kSize; ++i)
	{
		for (int j=-kSize; j <= kSize; ++j)
		{
			final_colour += kernel[kSize+j]*kernel[kSize+i]*texture(image, (gl_FragCoord.xy + vec2(float(i),float(j))) / resolution.xy).rgb;
	
		}
	}
		
	FragColor0 = vec4(final_colour/(Z*Z), 1.0);
*/