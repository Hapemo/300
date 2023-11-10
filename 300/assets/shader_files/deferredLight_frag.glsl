#version 450

// -- INPUTS --
layout (location = 0) in vec2 fTexCoords;

// -- UNIFORMS --
uniform vec3 uCamPos;
uniform int uLightCount;

// -- OUTPUTS --
layout (location = 0) out vec4 fragColor0;

// -- G-Buffers --
uniform sampler2D gPosition;        // XYZ:  Fragment Position
uniform sampler2D gNormal;          // XYZ:  Normal				| W: Roughness component
uniform sampler2D gAlbedoSpec;      // RGB:  Albedo Color		| A: Specular intensity
uniform sampler2D gEmission;        // RGBA: Emission

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

vec3 ComputePointLight(PointLight light, vec3 fragPos, vec4 normal, vec4 albedoSpec, vec4 emission)
{
    // Compute light directions and reflection             
    vec3 LightPos = light.position.xyz;

    vec3 viewDir = normalize(uCamPos - fragPos);
    vec3 lightDir = normalize(LightPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal.xyz);
    vec3 halfwayDir = normalize(lightDir + viewDir);  

    // Ambient Color
    vec3 ambient = 0.1 * albedoSpec.rgb;

    // Diffuse
    float diff = max(dot(lightDir, normal.xyz), 0.0);
    vec3 diffuse = diff * albedoSpec.rgb * light.color.rgb;

    // specular
    float spec = pow(max(dot(normal.xyz, halfwayDir), 0.0), normal.w); // pow to shininess
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
    vec3 fragPos    = texture(gPosition, fTexCoords).xyz;
    vec4 normal     = texture(gNormal, fTexCoords);
    vec4 albedoSpec = texture(gAlbedoSpec, fTexCoords);
    vec4 emission   = texture(gEmission, fTexCoords);

    vec4 finalColor = vec4(0.0);

    // Compute color with all point light sources
    for (int i = 0; i < uLightCount; ++i)
    {
        finalColor += ComputePointLight(pointLights[i], fragPos, normal, albedoSpec, emission);
    }

    // Gamma correction
    float gamma = 2.2;
    finalColor = vec4(pow(finalColor.rgb, vec3(1.0/gamma)), 1.0);

    // Output
    fragColor0 = finalColor;
}