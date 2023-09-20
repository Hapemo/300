#version 450

//in vec4 VertexColor;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

//uniform vec3 lightIntensity;

uniform sampler2D uTex[5];

layout (location = 0) out vec4 fragColor0;
layout (location = 1) out vec4 fragColor1;

void main() 
{
    
    vec4 uColor = texture(uTex[0], TexCoords);              // Diffuse Color
    if (uColor.a <= 0.1) discard;

    vec3 normal = texture(uTex[1], TexCoords).rgb;          // Normal Map
    normal = normalize(normal * 2.0 - 1.0);

    // Ambient Color
    vec3 ambient = 0.1 * uColor.rgb;

    // Diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * uColor.rgb;

    // specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.7) * spec;

    vec3 finalColor = vec3(ambient + diffuse + specular);

    // Output
    fragColor0 = vec4(finalColor, uColor.a);
    fragColor1 = vec4(finalColor, uColor.a);
}