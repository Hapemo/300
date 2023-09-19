#pragma once 
#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#define  _ENABLE_ANIMATIONS 0
#include "Guid.h"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <filesystem>


enum ResourceTypes : unsigned {

    MESH = 1234567,
    TEXTURE,
    SHADER,
    MATERIALINSTANCE
};


struct MeshData {
    void* m_pData;
    GFX::Mesh meshdata;
};


struct ShaderData
{
    void* m_pData;
    GFX::Shader shaderData;
};


struct MaterialInstanceData
{
    void* m_pData;
    GFX::Texture materialInstanceData;
};


class MaterialInstanceManager
{
public:
    constexpr static int  MAX_RESOURCE = 2400;
    static MaterialInstanceManager& GetInstance()
    {
        static MaterialInstanceManager instance;
        return instance;
    }
    MaterialInstanceManager() = default;
    MaterialInstanceManager(MaterialInstanceManager const&) = delete;
    void operator=(MaterialInstanceManager const&) = delete;

    void Init();

    MaterialInstanceData& AllocRscInfo();
    void ReleaseRscInfo(MaterialInstanceData& RscInfo);

    void SetupMaterialInstance(std::string filepath, unsigned);

    // Accessor
    MaterialInstanceData& getMaterialInstance(unsigned);

    int mResourceCnt{ 0 };

private:
    std::unordered_map<unsigned, MaterialInstanceData*>     mSceneMaterialInstances;
    MaterialInstanceData                                    *m_pInfoBufferEmptyHead{ nullptr };
    std::array<MaterialInstanceData, MAX_RESOURCE>          m_MaterialInstancebuffer;
};


class ShaderManager
{
public:
    constexpr static int  MAX_RESOURCE = 2400;
    static ShaderManager& GetInstance()
    {
        static ShaderManager instance;
        return instance;
    }
    ShaderManager() = default;
    ShaderManager(ShaderManager const&) = delete;
    void operator=(ShaderManager const&) = delete;

    void Init();

    ShaderData& AllocRscInfo();
    void ReleaseRscInfo(ShaderData& RscInfo);

    void SetupShader(std::string vertpath, std::string fragpath, unsigned);
    
    // Accessor
    ShaderData& getShader(unsigned);

    int mResourceCnt{ 0 };

private:
    std::unordered_map<unsigned, ShaderData*>   mSceneShaders;
    ShaderData                                  *m_pInfoBufferEmptyHead{ nullptr };
    std::array<ShaderData, MAX_RESOURCE>        m_Shaderbuffer;
};


class MeshManager
{
public:
    constexpr static int  MAX_RESOURCE = 2400;
    static MeshManager& GetInstance()
    {
        static MeshManager instance;
        return instance;
    }
    MeshManager() = default;
    MeshManager(MeshManager const&) = delete;
    void operator=(MeshManager const&) = delete;

    //std::vector<Mesh> mSceneMeshes;

    MeshData& AllocRscInfo();
    void ReleaseRscInfo(MeshData& RscInfo);

    void SetupMesh(std::string filepath, unsigned);

    void Init();

    MeshData& get_Mesh(unsigned);

    //void Destroy();

    int mResouceCnt{ 0 };

private:
    std::unordered_map<unsigned, MeshData*>     mSceneMeshes;
    MeshData                                    *m_pInfoBufferEmptyHead{ nullptr };
    std::array<MeshData, MAX_RESOURCE>          m_Meshbuffer;
};



//class AudioManager
//{
//public:
//    constexpr static int  MAX_RESOURCE = 2400;
//    static MeshManager& GetInstance()
//    {
//        static MeshManager instance;
//        return instance;
//    }
//    MeshManager() = default;
//    MeshManager(MeshManager const&) = delete;
//    void operator=(MeshManager const&) = delete;
//
//    //std::vector<Mesh> mSceneMeshes;
//
//    MeshData& AllocRscInfo();
//    void ReleaseRscInfo(MeshData& RscInfo);
//
//    void SetupMesh(std::string filepath, unsigned);
//
//    void Init();
//
//    MeshData& get_Mesh(unsigned);
//
//    //void Destroy();
//
//    int mResouceCnt{ 0 };
//
//private:
//    std::unordered_map<unsigned, MeshData*>     mSceneMeshes;
//    MeshData* m_pInfoBufferEmptyHead{ nullptr };
//    std::array<MeshData, MAX_RESOURCE>          m_Meshbuffer;
//};









struct instance_info 
{
    std::string     m_Name{};
    void            *m_pData{ nullptr };
    uid<unsigned>   m_GUID;
    unsigned        m_Type;
    int             m_RefCount{ 1 };
};


class Resource 
{

public:
    Resource();
    Resource(const Resource&) = delete;
    ~Resource() = default;

    void Init();
    void Update();
    void Exit();

    void mesh_Loader();
    void shader_Loader();
    void MaterialInstance_Loader();

    void get_Texture(uid<> id);
    void get_Mesh(uid<> id);
    GFX::Texture& get_MaterialInstance(std::string name);
    GFX::Mesh& get_Mesh(std::string name);
    GFX::Shader& get_Shader(std::string name);

    instance_info& AllocRscInfo(void);
    void ReleaseRscInfo(instance_info& RscInfo);

    constexpr static int  MAX_RESOURCE = 2400;

    int mResouceCnt{ 0 };

private:
    instance_info*                                      m_pInfoBufferEmptyHead{ nullptr };
    std::unordered_map<unsigned, instance_info*>        m_Textures;
    std::unordered_map<unsigned, instance_info*>        m_Meshes;
    std::unordered_map<unsigned, instance_info*>        m_Shaders;
    std::unordered_map<unsigned, instance_info*>        m_MaterialInstances;
    std::array<instance_info, MAX_RESOURCE>             m_Infobuffer;

    const std::string compiled_geom_path = "../compiled_geom/";
    const std::string compressed_texture_path = "../assets/Compressed/";

    MeshManager             mMeshManager;
    ShaderManager           mShaderManager;
    MaterialInstanceManager mMaterialInstanceManager;
};



#endif // !_RESOURCEMANAGER_H