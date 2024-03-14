#version 450
#extension GL_ARB_gpu_shader_int64 : enable

// -- INPUTS --
layout (location = 0) in vec2 fTexCoords;

// -- UNIFORMS --
uniform vec3 uCamPos;
uniform int uLightCount;

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor0;

// -- G-Buffers --
uniform uint64_t gPosition;        // XYZ:  Fragment Position   | W: Ambient Occlusion
uniform uint64_t gNormal;          // XYZ:  Normal				| W: Roughness component
uniform uint64_t gAlbedoSpec;      // RGB:  Albedo Color		| A: Metallic component
uniform uint64_t gEmission;        // RGBA: Emission

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
float CalculateAttenuation(float linear, float quadratic, float distance)
vec3 ComputeLight(PointLight light, vec3 F0, vec3 fragPos, vec3 albedo, vec3 normal, float roughness, float metallic);

const float PI = 3.14159265359;

void main()
{
    vec4 fragPos    = texture(sampler2D(gPosition), fTexCoords);
    vec4 normal     = texture(sampler2D(gNormal), fTexCoords);
    vec4 albedoSpec = texture(sampler2D(gAlbedoSpec), fTexCoords);
    vec4 emission   = texture(sampler2D(gEmission), fTexCoords);
    float roughness = normal.a;
    float metallic  = albedoSpec.a;
    float ao        = fragPos.a;

    // Get the F0 Parameter
    vec3 F0 = vec3(0.04);       // Non-metallic surface is always 0.04
    // Get F0 by linearly interpolating between F0 and albedo value with given metallic value
    F0 = mix(F0, albedoSpec.rgb, metallic);     

    vec3 lightRadiance = vec3(0.0);

    for (int i{}; i < uLightCount; ++i)
    {
        lightRadiance = ComputeLight(pointLights[i], F0, fragPos.rgb, albedoSpec.rgb, normal.rgb, roughness, metallic);
    }

    vec3 ambient = vec3(0.03) * albedoSpec.rgb * ao;
    vec3 color = ambient + lightRadiance;

    color = color / (color + vec3(1.0));
    //float gamma = 2.2;
    //color = pow(color, vec3(1.0/gamma));
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
    vec3 radiance       = light.color * attenuation;

    // Cook-Torrance BRDF
    float NDF   = DistributionGGX(normal, H, roughness);
    float G     = GeometrySmith(N, V, L, roughness);
    vec3 F      = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 num        = NDF * G * F;
    float denom     = 4.0 * max(dot(V, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // 0.0001 to prevent division by 0
    vec3 specular   = num / denom;

    // Return to add to outgoing radiance
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

float CalculateAttenuation(float linear, float quadratic, float distance)
{
    // Attenuation
    float linear = max(light.linear * 0.001, 0.0);
    float quadratic = max(light.quadratic * 0.00001, 0.0);
    float attenuation = 1.0 / (1.0 + linear * distance + quadratic * (distance * distance));
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

    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float denom = NdotV * (1.0 - k) + k;

    return NdotV / denom
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}