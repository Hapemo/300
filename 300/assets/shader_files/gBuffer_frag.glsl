#version 450
#extension GL_ARB_gpu_shader_int64 : enable

// -- INPUTS --
in vec4 vertexColor;        // Custom Color of the vertex
in vec2 TexCoords;		    // Texture Coordinates
in vec4 Tex_Ent_ID;		    // Texture ID, Entity ID
in mat3 TBN;			    // Tangent-to-world matrix
in vec3 fragPos;		    // Fragment position
in vec4 Bloom_Threshold;    // Bloom Threshold


// -- OUTPUTS --
layout (location = 0) out vec4 outVertexColor;
layout (location = 1) out uint outEntityID;
layout (location = 2) out vec4 BrightColor;

layout (location = 3) out vec4 gFragPos;		// XYZ:  Fragment Position
layout (location = 4) out vec4 gNormal;			// XYZ:  Normal				| W: Roughness component
layout (location = 5) out vec4 gAlbedoSpec;		// RGB:  Albedo Color		| A: Specular intensity
layout (location = 6) out vec4 gEmission;		// RGBA: Emission

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

vec3 n          = vec3(0.0);
vec3 diffuse    = vec3(0.0);
vec4 emission   = vec4(0.0);
float roughness = 1.f;
float specular  = 0.0f;

void main()
{
    // Texture ID
    int ID = int(Tex_Ent_ID.x);

    // Get Normal and Roughness
    if(materials[ID].normalMap != 0)
    {
        n = texture(sampler2D(materials[ID].normalMap), TexCoords).rgb;
        n = normalize(n * 2.0 - 1.0);   // Maps normal
    }
    if(materials[ID].shininessMap != 0)
        roughness = texture(sampler2D(materials[ID].shininessMap), TexCoords).r;

    // Get Albedo and Specular
    if(materials[ID].diffuseMap != 0)
        diffuse = texture(sampler2D(materials[ID].diffuseMap), TexCoords).rgb;
    if(materials[ID].specularMap != 0)
        specular = texture(sampler2D(materials[ID].specularMap), TexCoords).r;

    // Get Emission
    if (materials[ID].emissionMap != 0)
        emission = texture(sampler2D(materials[ID].emissionMap), TexCoords);

    // Write G-Buffer output
	gFragPos    = vec4(fragPos, 1.0);
    gNormal     = vec4(TBN * n, roughness);
    gAlbedoSpec = vec4(diffuse, specular);
    gEmission   = emission;

    // Write Entity ID, bloom threshold and custom vertex color
    outEntityID = uint(Tex_Ent_ID.y);
    BrightColor = Bloom_Threshold;
    outVertexColor = vertexColor;
}