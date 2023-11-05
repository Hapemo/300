#version 430 core

// Workgroup Size
layout(local_size_x = 20, local_size_y = 20, local_size_z = 1) in;
//layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

// Output image
layout(rgba32f, binding = 0) uniform image2D imgOutput;

// -- UNIFORM --
uniform float t;

void main() {
//    vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
//    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
//    float speed = 100;
//    // the width of the texture
//    float width = 1920;
//
//    value.x = mod(float(texelCoord.x) + t * speed, width) / (gl_NumWorkGroups.x);
//    value.y = float(texelCoord.y) / (gl_NumWorkGroups.y);
//    imageStore(imgOutput, texelCoord, value);

    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    // Take 13 samples around current texel:
    // a - b - c
    // - j - k -
    // d - e - f
    // - l - m -
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = imageLoad(imgOutput, ivec2(texelCoord.x - 2, texelCoord.y + 2)).rgb;
    vec3 b = imageLoad(imgOutput, ivec2(texelCoord.x,     texelCoord.y + 2)).rgb;
    vec3 c = imageLoad(imgOutput, ivec2(texelCoord.x + 2, texelCoord.y + 2)).rgb;

    vec3 d = imageLoad(imgOutput, ivec2(texelCoord.x - 2, texelCoord.y)).rgb;
    vec3 e = imageLoad(imgOutput, ivec2(texelCoord.x,     texelCoord.y)).rgb;
    vec3 f = imageLoad(imgOutput, ivec2(texelCoord.x + 2, texelCoord.y)).rgb;

    vec3 g = imageLoad(imgOutput, ivec2(texelCoord.x - 2, texelCoord.y - 2)).rgb;
    vec3 h = imageLoad(imgOutput, ivec2(texelCoord.x,     texelCoord.y - 2)).rgb;
    vec3 i = imageLoad(imgOutput, ivec2(texelCoord.x + 2, texelCoord.y - 2)).rgb;

    vec3 j = imageLoad(imgOutput, ivec2(texelCoord.x - 1, texelCoord.y + 1)).rgb;
    vec3 k = imageLoad(imgOutput, ivec2(texelCoord.x + 1, texelCoord.y + 1)).rgb;
    vec3 l = imageLoad(imgOutput, ivec2(texelCoord.x - 1, texelCoord.y - 1)).rgb;
    vec3 m = imageLoad(imgOutput, ivec2(texelCoord.x + 1, texelCoord.y - 1)).rgb;

    // Apply weighted distribution:
    // 0.5 + 0.125 + 0.125 + 0.125 + 0.125 = 1
    // a,b,d,e * 0.125
    // b,c,e,f * 0.125
    // d,e,g,h * 0.125
    // e,f,h,i * 0.125
    // j,k,l,m * 0.5
    // This shows 5 square areas that are being sampled. But some of them overlap,
    // so to have an energy preserving downsample we need to make some adjustments.
    // The weights are the distributed, so that the sum of j,k,l,m (e.g.)
    // contribute 0.5 to the final color output. The code below is written
    // to effectively yield this sum. We get:
    // 0.125*5 + 0.03125*4 + 0.0625*4 = 1
    vec3 downsample = vec3(0.0);
    downsample.rgb = e*0.125;
    downsample.rgb += (a+c+g+i)*0.03125;
    downsample.rgb += (b+d+f+h)*0.0625;
    downsample.rgb += (j+k+l+m)*0.125;

    imageStore(imgOutput, texelCoord, vec4(downsample, 1.0));
}