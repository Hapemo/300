#version 430 core

// Workgroup Size
layout(local_size_x = 30, local_size_y = 30) in;

// Output image
layout(rgba32f, binding = 0) uniform image2D imgOutput;

// -- UNIFORM --
uniform float uGamma;

void main() {


    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = imageLoad(imgOutput, texelCoord);

    color.rgb = pow(color.rgb, vec3(1 / uGamma));

    imageStore(imgOutput, texelCoord, color);
}