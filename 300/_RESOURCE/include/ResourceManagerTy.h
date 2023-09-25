#pragma once 
#ifndef _RESOURCEMANAGERTY_H
#define _RESOURCEMANAGERTY_H

#define  _ENABLE_ANIMATIONS 1


#include <filesystem>
#include "Guid.h"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

enum ResourceType : unsigned {

    _MESH = 1234567,
    _TEXTURE,
    _SHADER,
    _MATERIALINSTANCE
};

//template <typename RESOURCE_T>
//struct type {
//
//};
//
//template<>
//struct type<int> {
//    int data;
//};
//
//template<>
//struct type<GFX::Mesh> {
//    int data;
//};

//struct MeshDatas {
//    void* m_pData;
//    GFX::Mesh meshdata;
//};



enum MaterialType : unsigned {

    DIFFUSE,
    NORMAL,
    EMISSION,
    SPECULAR

};
struct instance_infos
{
    std::string     m_Name{};
    void* m_pData{ nullptr }; // to store data / act as an pointer to link list if not used
    uid<unsigned>   m_GUID;
    unsigned        m_Type;
    int             m_RefCount{ 1 };
   // type<T>          data;
};


class ResourceTy
{
public:
    ResourceTy();
    ResourceTy(const ResourceTy&) = delete;
    ~ResourceTy() = default;

    void Init();
    void Exit();

    void mesh_Loader();
    GFX::Mesh* SetupMesh(std::string filepath, unsigned);
    GFX::Mesh* get_mesh(unsigned );


    void MaterialInstance_Loader();
    GFX::Texture* SetupMaterialInstance(std::string filepath, unsigned);
    GFX::Texture* getMaterialInstance(unsigned);

    instance_infos& AllocRscInfo(void);
    void ReleaseRscInfo(instance_infos& RscInfo);


    constexpr static int  MAX_RESOURCE = 2400;
    const std::string compiled_geom_path = "../assets/compiled_geom/";
    const std::string compressed_texture_path = "../assets/Compressed/";

    int mResouceCnt;
private:
    instance_infos*                                         m_pInfoBufferEmptyHead{ nullptr };
    std::unordered_map<std::uint64_t, instance_infos*>      m_ResourceInstance;
    std::array<instance_infos, MAX_RESOURCE>                m_Infobuffer;

    //std::array<type<int>,MAX_RESOURCE> temp;

};


#endif