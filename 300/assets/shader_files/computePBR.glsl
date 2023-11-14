#version 430 core

// -- Workgroup Size --
layout(local_size_x = 30, local_size_y = 30, local_size_z = 1) in;

// I-- INPUT --
layout(rgba32f, binding = 1) uniform image2D gPosition;     // XYZ:  Fragment Position  | W: Ambient Occlusion
layout(rgba32f, binding = 2) uniform image2D gNormal;       // XYZ:  Normal				| W: Roughness component
layout(rgba32f, binding = 3) uniform image2D gAlbedoSpec;   // RGB:  Albedo Color		| A: Metallic component
layout(rgba32f, binding = 4) uniform image2D gEmission;     // RGBA: Emission

// -- OUTPUT --
layout(rgba32f, binding = 0) uniform image2D imgOutput;

// -- UNIFORM --
uniform vec3 uCamPos;
uniform int uLightCount;
uniform vec4 uGlobalTint;

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

vec3 FresnelSchlick(float cosTheta, vec3 f0);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
float CalculateAttenuation(float linear, float quadratic, float distance);
vec3 ComputeLight(PointLight light, vec3 F0, vec3 fragPos, vec3 albedo, vec3 normal, float roughness, float metallic);

const float PI = 3.14159265359;

void main() 
{
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    vec4 fragPos        = imageLoad(gPosition, texelCoord);
    vec4 normal         = imageLoad(gNormal, texelCoord);
    vec4 albedoSpec     = imageLoad(gAlbedoSpec, texelCoord);
    vec4 emission       = imageLoad(gEmission, texelCoord);
    float roughness     = normal.a;
    float metallic      = albedoSpec.a;
    float ao            = fragPos.a;
    //albedoSpec.rgb = pow(albedoSpec.rgb, vec3(2.2));

    // Get the F0 Parameter
    // Non-metallic surface is always 0.04
    vec3 F0 = vec3(0.04);       
    // Get F0 by linearly interpolating between F0 and albedo value with given metallic value
    F0 = mix(F0, albedoSpec.rgb, metallic);  

    vec3 lightRadiance = vec3(0.0);

    for (int i = 0; i < uLightCount; ++i)
    {
        lightRadiance += ComputeLight(pointLights[i], F0, fragPos.rgb, albedoSpec.rgb, normal.rgb, roughness, metallic);
    }

    vec3 ambient = vec3(0.03) * albedoSpec.rgb * ao;
    vec3 color = ambient + lightRadiance;
    vec3 finalColor = color + emission.rgb;

    float gamma = 2.2;
    finalColor = finalColor / (finalColor + vec3(1.0));
    finalColor = pow(finalColor, vec3(1.0/gamma));

    imageStore(imgOutput, texelCoord, vec4(finalColor, 1.0) * uGlobalTint);
}

vec3 ComputeLight(PointLight light, vec3 F0, vec3 fragPos, vec3 albedo, vec3 normal, float roughness, float metallic)
{
    vec3 lightPos = light.position.xyz;

    // Calculate radiance of light instance
    vec3 V = normalize(uCamPos - fragPos);
    vec3 L = normalize(lightPos - fragPos);
    vec3 H = normalize(V + L);
    float dist          = length(lightPos - fragPos);
    float attenuation   = CalculateAttenuation(light.linear, light.quadratic, dist);
    vec3 radiance       = light.color.rgb * attenuation * light.intensity;

    // Cook-Torrance BRDF
    float NDF   = DistributionGGX(normal, H, roughness);
    float G     = GeometrySmith(normal, V, L, roughness);
    vec3 F      = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 num        = NDF * G * F;
    float denom     = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.0001; // 0.0001 to prevent division by 0
    vec3 specular   = num / denom;

    // Return to add to outgoing radiance
    float NdotL = max(dot(normal, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

float CalculateAttenuation(float linear, float quadratic, float distance)
{
    // Attenuation
    linear = max(linear * 0.001, 0.0);
    quadratic = max(quadratic * 0.00001, 0.0);
    float attenuation = 1.0 / (1.0 + linear * distance + quadratic * (distance * distance));

    return attenuation;
}

vec3 FresnelSchlick(float cosTheta, vec3 f0)
{
    return f0 + (1.0 - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a         = roughness *  roughness;
    float a2        = a * a;
    float NdotH     = max(dot(N, H), 0.0);
    float NdotH2    = NdotH * NdotH;
    
    float denom     = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float denom = NdotV * (1.0 - k) + k;

    return NdotV / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}