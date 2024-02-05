#version 430 core

// -- Workgroup Size --
layout(local_size_x = 30, local_size_y = 30, local_size_z = 1) in;

// -- INPUT --
layout(rgba32f, binding = 2) uniform image2D bloomThresholds;       // Contains the bloom thresholds for Bloom
layout(rgba32f, binding = 3) uniform image2D gPosition;             // XYZ:  Fragment Position  | W: Ambient Occlusion
layout(rgba32f, binding = 4) uniform image2D gNormal;               // XYZ:  Normal				| W: Roughness component
layout(rgba32f, binding = 5) uniform image2D gAlbedoSpec;           // RGB:  Albedo Color		| A: Metallic component
layout(rgba32f, binding = 6) uniform image2D gEmission;             // RGBA: Emission

layout(binding = 0) uniform sampler2D depthBuffer;

// -- OUTPUT --
layout(rgba32f, binding = 0) uniform image2D imgOutput;             // Custom Color of the mesh, and also the output
layout(rgba32f, binding = 1) uniform image2D brightOutput;

// -- UNIFORM --
uniform vec3 uCamPos;
uniform int uLightCount;
uniform int uSpotlightCount;
uniform vec4 uGlobalTint;
uniform vec4 uGlobalBloomThreshold;
uniform mat4 uLightSpaceMatrix;
uniform vec3 uDirLightPos;

struct PointLight           // 48 Bytes
{
    vec4    position;       // 0 
    vec4    color;          // 16
    float   linear;         // 32
    float   quadratic;      // 26
    float   intensity;      // 40
    float   padding;        // 44
};

struct SpotLight            // 64 Bytes
{
	vec4    position;       // 0
	vec4    direction;      // 16
	vec4    color;          // 32
	float   cutoff;         // 48
	float   outerCutoff;    // 52
	float   intensity;      // 56
	float   pad;            // 60
};

layout (std430, binding = 2) buffer pointLightBuffer
{
    PointLight pointLights[];
};
layout(std430, binding = 4) buffer spotLightBuffer
{
    SpotLight spotlights[];
};

vec3 FresnelSchlick(float cosTheta, vec3 f0);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
float CalculateAttenuation(float linear, float quadratic, float distance);
vec3 ComputeLight(PointLight light, vec3 F0, vec3 fragPos, vec3 albedo, vec3 normal, float roughness, float metallic);
vec3 ComputeSpotLight(SpotLight light, vec3 fragPos, vec3 albedo, vec3 normal, float roughness, float metallic);
float ShadowCalculation(vec3 fragPos, vec3 normal);

const float near_plane = 0.1;
const float far_plane = 900.0;
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
    vec4 customColor    = imageLoad(imgOutput, texelCoord);
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
    for (int i = 0; i < uSpotlightCount; ++i)
    {
        lightRadiance += ComputeSpotLight(spotlights[i], fragPos.rgb, albedoSpec.rgb, normal.rgb, roughness, metallic);
    }

    vec3 ambient = vec3(0.3) * albedoSpec.rgb * ao;
    vec3 color = vec3(0.3) * albedoSpec.rgb;
    //vec3 color = ambient + lightRadiance;
    vec4 finalColor = vec4(lightRadiance + emission.rgb, 1.0);
    finalColor = finalColor * customColor;   // Set Custom Color
    float shadow = ShadowCalculation(fragPos.xyz, normal.rgb);
    finalColor = vec4(color, 1.0) + (1.0 - shadow) * finalColor;

    // HDR
    // check whether fragment output is higher than threshold, if so output as bright color
    vec4 bloomThreshold = imageLoad(bloomThresholds, texelCoord);

    float brightness = 0.f;
    if(uGlobalBloomThreshold.a <= 0.1) {
        // the bloom is turned off. brightness = 0
    }
    else if (bloomThreshold.a > 0.1) {
        // the bloom for the individual object is turned on
        brightness = dot(finalColor.rgb, bloomThreshold.rgb);
    }
    else {
        // use the global bloom
        brightness = dot(finalColor.rgb, uGlobalBloomThreshold.rgb);
    }

    if(brightness > 1.0)
        imageStore(brightOutput, texelCoord, vec4(finalColor.rgb, brightness));
    else
        imageStore(brightOutput, texelCoord, vec4(0.0, 0.0, 0.0, 1.0));

    float gamma = 2.2;
    finalColor.rgb = finalColor.rgb / (finalColor.rgb + vec3(1.0));
    finalColor.rgb = pow(finalColor.rgb, vec3(1.0/gamma));

    imageStore(imgOutput, texelCoord, finalColor * uGlobalTint);
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

vec3 ComputeSpotLight(SpotLight light, vec3 fragPos, vec3 albedo, vec3 normal, float roughness, float metallic)
{
    vec3 lightToFragDir = normalize(light.position.xyz - fragPos);
    vec3 lightDir = light.direction.xyz;

    // Diffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightToFragDir), 0.0);
    vec3 diffuse = diff * albedo;

    float theta = dot(lightToFragDir, normalize(-lightDir));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = light.intensity * clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    // placeholder, To be Changed
    diffuse *= intensity;
    metallic *= intensity;

    return light.color.rgb * (diffuse + metallic);
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

float ShadowCalculation(vec3 fragPos, vec3 normal)
{
    vec4 fragPosLightSpace = uLightSpaceMatrix * vec4(fragPos, 1.0);
    // Perform perspective divide to transform clip space to NDC
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;  // Gets light-space position in range [-1, 1]

    // Depth map range is [0, 1], convert to depth map range
    projCoords = projCoords * 0.5 + vec3(0.5);

    if (projCoords.z > 1.0)
        return 0.0;

    vec3 fragToLightDir = normalize(uDirLightPos - fragPos);
    normal = normalize(normal);

    float closestDepth = texture(depthBuffer, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // In shadow if current depth is larger than closest depth
    //float bias = max(0.05 * (1.0 - dot(normal, fragToLightDir)), 0.0005);
    float bias = 0.0005;

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthBuffer, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthBuffer, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    return shadow;
}