#version 450

// OUT
layout (location = 0) out vec4 FragColor0;
layout (location = 1) out vec4 FragColor1;

// IN  
in vec2 TexCoords;

uniform sampler2D image;
  
uniform bool horizontal;
uniform float TexOffset;

float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform float SamplingWeight;

void main()
{             
    // gets size of single texel
    vec2 tex_offset = TexOffset / textureSize(image, 0);

    // current fragment's contribution
    vec3 result = texture(image, TexCoords).rgb * weight[0]; 
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i] * SamplingWeight;
            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i] * SamplingWeight;
        }
        
        FragColor0 = vec4(result, 1.0);
    }

    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i] * SamplingWeight;
            result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i] * SamplingWeight;
        }
        
        FragColor1 = vec4(result, 1.0);
    }
}