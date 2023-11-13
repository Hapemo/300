#version 450

#extension GL_ARB_gpu_shader_int64 : enable

in vec2 TexCoords;
in mat3 TBN;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec4 Tex_Ent_ID;     // x : Tex ID, y: Entity ID
in vec4 VertexColor;
in vec4 Bloom_Threshold;

struct PointLight           // 48 Bytes
{
    vec4    position;       // 0 
    vec4    color;          // 16
    float   linear;         // 32
    float   quadratic;      // 26
    float   intensity;      // 40
    float   padding;        // 44
};

struct Material
{
    uint64_t diffuseMap;
    uint64_t normalMap;
    uint64_t specularMap;
    uint64_t shininessMap;
    uint64_t emissionMap;
};

layout (std430, binding = 1) buffer materialBuffers
{
    Material materials[];
};

layout (std430, binding = 2) buffer pointLightBuffer
{
    PointLight pointLights[];
};

uniform sampler2D uTex[5];
uniform vec4 globalTint;;
uniform bool uHasLight;
uniform int uLightCount;
uniform vec4 globalBloomThreshold;

layout (location = 0) out vec4 fragColor0;
layout (location = 1) out uint outEntityID;
layout (location = 2) out vec4 BrightColor;

vec3 diffuseMap = vec3(0.f);
vec3 normalMap = vec3(0.f);
vec3 specularMap = vec3(0.f);
vec3 shininessMap = vec3(1.f);
vec3 emissionMap = vec3(0.f);

vec3 ComputePointLight(PointLight light)
{
    // Compute light directions and reflection             
    vec3 TangentLightPos = TBN * light.position.xyz;

    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normalMap);
    vec3 halfwayDir = normalize(lightDir + viewDir);  

    // Ambient Color
    vec3 ambient = 0.1 * diffuseMap;

    // Diffuse
    float diff = max(dot(lightDir, normalMap), 0.0);
    vec3 diffuse = diff * diffuseMap * light.color.rgb;

    // specular
    float spec = pow(max(dot(normalMap, halfwayDir), 0.0), shininessMap.r); // pow to shininess
    vec3 specular = spec * specularMap * light.color.rgb; // Multiply with light color

    // Attenuation
    float linear = max(light.linear * 0.001, 0.0);
    float quadratic = max(light.quadratic * 0.00001, 0.0);
    float distance = length(TangentLightPos - TangentFragPos);
    float attenuation = 1.0 / (1.0 + linear * distance + quadratic * (distance * distance));
    attenuation *= light.intensity;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 finalColor = vec3(ambient + diffuse + specular + emissionMap);
    finalColor *= vec3(VertexColor) * vec3(globalTint);

    return finalColor;
}

void main() 
{
    int ID = int(Tex_Ent_ID.x);

    vec4 uColor = texture(sampler2D(materials[ID].diffuseMap), TexCoords);              // Diffuse Color
    uColor.a = 1.0;
    if (uColor.a <= 0.1) discard;

    vec3 finalColor = vec3(0.0);

    //outEntityID = uint(Tex_Ent_ID.y);
    if (!uHasLight)
    {
        // No light
        finalColor = uColor.rgb; // * vec3(VertexColor);
    }

    else if(uHasLight)
    {

        if (materials[ID].diffuseMap != 0)
            diffuseMap      = texture(sampler2D(materials[ID].diffuseMap), TexCoords).rgb;
        if (materials[ID].normalMap != 0)
            normalMap       = texture(sampler2D(materials[ID].normalMap), TexCoords).rgb;
        if (materials[ID].specularMap != 0)
            specularMap     = texture(sampler2D(materials[ID].specularMap), TexCoords).rgb;
        if (materials[ID].shininessMap != 0)
            shininessMap    = texture(sampler2D(materials[ID].shininessMap), TexCoords).rgb;
        if (materials[ID].emissionMap != 0)
            emissionMap     = texture(sampler2D(materials[ID].emissionMap), TexCoords).rgb;

        normalMap = normalize(normalMap * 2.0 - 1.0);   // Maps normal

        // Compute color with all point light sources
        for (int i = 0; i < uLightCount; ++i)
        {
            finalColor += ComputePointLight(pointLights[i]);
        }
    }

    // HDR
    // check whether fragment output is higher than threshold, if so output as bright color
    
    float brightness = 0.f;
    if(globalBloomThreshold.a <= 0.1) {
        // the bloom is turned off. brightness = 0
    }
    else if (Bloom_Threshold.a > 0.1) {
        // the bloom for the individual object is turned on
        brightness = dot(finalColor.rgb, Bloom_Threshold.rgb);
    }
    else {
        // use the global bloom
        brightness = dot(finalColor.rgb, globalBloomThreshold.rgb);
    }


    if(brightness > 1.0)
        BrightColor = vec4(finalColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

    // Set the alpha for debug draw
    uColor.a *= (globalTint.a * VertexColor.a);
    //finalColor = globalTint.a;

    // Gamma correction
    float gamma = 2.2;
    finalColor = pow(finalColor, vec3(1.0/gamma));

    // Output
    fragColor0 = vec4(finalColor, uColor.a);
    outEntityID = uint(Tex_Ent_ID.y);
}