#version 450

in vec2 TexCoords;
//in vec3 TangentLightPos;
in mat3 TBN;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec4 Tex_Ent_ID;     // x : Tex ID, y: Entity ID
in int hasLight;

struct PointLight
{
    vec3    position;
    vec3    color;
    float   linear;
    float   quadratic;
    float   intensity;
};

layout (std430, binding = 2) buffer pointLightBuffer
{
    PointLight pointLights[];
};

uniform sampler2D uTex[5];
uniform int uDebugDraw;
uniform bool uHasLight;
//uniform float uLightIntensity;
uniform vec3 uLightColor;
uniform int uLightCount;

layout (location = 0) out vec4 fragColor0;
layout (location = 1) out uint outEntityID;

vec3 ComputePointLight(PointLight light, vec4 diffColor)
{
    vec3 TangentLightPos = TBN * light.position;

    vec3 normal = texture(uTex[1], TexCoords).rgb;          // Normal Map
    normal = normalize(normal * 2.0 - 1.0);

    //vec3 emission = texture(uTex[2], TexCoords).rgb;        // Emission Map
    //vec3 specular = texture(uTex[3], TexCoords).rgb;        // Specular Map
    vec3 specular = vec3(0.3);

    // Ambient Color
    vec3 ambient = 0.1 * diffColor.rgb;

    // Diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * diffColor.rgb * light.color;

    // specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 lightIntensity = vec3(light.intensity);
    specular = spec * specular * lightIntensity; // Multiply with light intensity

    float distance = length(TangentLightPos - TangentFragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 finalColor = vec3(ambient + diffuse + specular /* + emission*/);

    return finalColor;
}

void main() 
{
    
    vec4 uColor = texture(uTex[0], TexCoords);              // Diffuse Color
    if (uColor.a <= 0.1) discard;

    outEntityID = uint(Tex_Ent_ID.y);
    if (!uHasLight)
    {
        fragColor0 = uColor;
        return;
    }

//    vec3 normal = texture(uTex[1], TexCoords).rgb;          // Normal Map
//    normal = normalize(normal * 2.0 - 1.0);
//
//    //vec3 emission = texture(uTex[2], TexCoords).rgb;        // Emission Map
//    //vec3 specular = texture(uTex[3], TexCoords).rgb;        // Specular Map
//    vec3 specular = vec3(0.3);
//
//    // Ambient Color
//    vec3 ambient = 0.1 * uColor.rgb;
//
//    // Diffuse
//    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
//    float diff = max(dot(lightDir, normal), 0.0);
//    vec3 diffuse = diff * uColor.rgb * uLightColor;
//
//    // specular
//    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    vec3 halfwayDir = normalize(lightDir + viewDir);  
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
//    vec3 lightIntensity = vec3(uLightIntensity);
//    specular = spec * specular * lightIntensity;
//
//    vec3 finalColor = vec3(ambient + diffuse + specular /* + emission*/);

    vec3 finalColor = vec3(0.0);

    for (int i = 0; i < uLightCount; ++i)
    {
        finalColor += ComputePointLight(pointLights[i], uColor);
    }

    // HDR
    // float exposure = 1.0;
    // finalColor = vec3(1.0) - exp(-finalColor * exposure);

    // Gamma correction
    float gamma = 2.2;
    finalColor = pow(finalColor, vec3(1.0/gamma));

    // Output
    if(uDebugDraw == 1) {
        uColor.a = 0.3f;
    }

    fragColor0 = vec4(finalColor, uColor.a);
}