#version 430 core

// -- Workgroup Size --
layout(local_size_x = 30, local_size_y = 30, local_size_z = 1) in;

// I-- INPUT --
layout(rgba32f, binding = 1) uniform image2D gPosition;     // XYZ:  Fragment Position
layout(rgba32f, binding = 2) uniform image2D gNormal;       // XYZ:  Normal				| W: Roughness component
layout(rgba32f, binding = 3) uniform image2D gAlbedoSpec;   // RGB:  Albedo Color		| A: Specular intensity
layout(rgba32f, binding = 4) uniform image2D gEmission;     // RGBA: Emission

// -- OUTPUT --
layout(rgba32f, binding = 0) uniform image2D imgOutput;

// -- UNIFORM --
uniform vec3 uCamPos;
uniform int uLightCount;

struct PointLight           // 48 Bytes
{
    vec4    position;       // 0 
    vec4    color;          // 16
    float   linear;         // 32
    float   quadratic;      // 26
    float   intensity;      // 40
    float   padding;        // 44
};

layout (std430, binding = 2) buffer pointLightBuffer
{
    PointLight pointLights[];
};

vec3 ComputePointLight(PointLight light, vec3 fragPos, vec3 normal, float roughness, vec4 albedoSpec, vec4 emission)
{
    // Compute light directions and reflection             
    vec3 LightPos = light.position.xyz;

    vec3 viewDir = normalize(uCamPos - fragPos);
    vec3 lightDir = normalize(LightPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  

    // Ambient Color
    vec3 ambient = 0.1 * albedoSpec.rgb;

    // Diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * albedoSpec.rgb * light.color.rgb;

    // specular
    float spec = pow(max(dot(normal, halfwayDir), 0.0), roughness); // pow to shininess
    vec3 specular = spec * albedoSpec.a * light.color.rgb;   // Multiply with light color

    // Attenuation
    float linear = max(light.linear * 0.001, 0.0);
    float quadratic = max(light.quadratic * 0.00001, 0.0);
    float distance = length(LightPos - fragPos);
    float attenuation = 1.0 / (1.0 + linear * distance + quadratic * (distance * distance));
    attenuation *= light.intensity;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 finalColor = vec3(ambient + diffuse + specular + emission.rgb);
    //finalColor *= vec3(VertexColor) * vec3(globalTint);

    return finalColor;
}

void main() 
{
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    vec3 fragPos        = imageLoad(gPosition, texelCoord).xyz;
    vec4 normal         = imageLoad(gNormal, texelCoord);
    vec4 albedoSpec    = imageLoad(gAlbedoSpec, texelCoord);
    vec4 emission       = imageLoad(gEmission, texelCoord);
    float roughness     = normal.w;

    vec3 finalColor = vec3(0.0);

    // Compute color with all point light sources
    for (int i = 0; i < uLightCount; ++i)
    {
        finalColor += ComputePointLight(pointLights[i], fragPos, normal.xyz, roughness, albedoSpec, emission);
    }

    // Gamma correction
    float gamma = 2.2;
    finalColor = pow(finalColor.rgb, vec3(1.0/gamma));

    imageStore(imgOutput, texelCoord, vec4(finalColor, 1.0));
}