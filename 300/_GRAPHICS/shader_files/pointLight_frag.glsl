#version 450

in vec4 VertexColor;
in vec2 TexCoords;
in mat4 LTW;
in vec4 WorldPos;

uniform vec3 lightPos;
//uniform vec3 lightIntensity;

uniform sampler2D uTex[5];

layout (location = 0) out vec4 fragColor0;
layout (location = 1) out vec4 fragColor1;

void main() 
{
    vec4 uColor = texture(uTex[0], TexCoords);                      // Color texture
    vec3 normal = texture(uTex[1], TexCoords).xyz;                  // Normal texture
    normal = (WorldPos * vec4(normal, 0.f)).xyz;                    // Transform the normal to world space

    vec3 lightDir = normalize(WorldPos.xyz - lightPos.xyz);         // Light Direction
    float diffuse = max(dot(lightDir, normal), 0.0f);    // Diffuse scale

    float ambient = 0.5f;

    vec3 finalColor = uColor.xyz * vec3(1.f, 0.2f, 0.2f) * (diffuse + ambient);

    // Output
    fragColor0 = vec4(finalColor, uColor.a);
    fragColor1 = vec4(finalColor, uColor.a);
}