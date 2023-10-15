#pragma once 
#ifndef _RESOURCEMANAGERTY_H
#define _RESOURCEMANAGERTY_H

#define  _ENABLE_ANIMATIONS 1


#include <filesystem>
#include "Guid.h"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include <variant>  



//union ref {
//
//    void* data;
//    unsigned dataid;
//
//};

struct p_ref {
    std::variant<void*, uid<unsigned>> data;

    template <typename T>
    bool GetVariantData(T* val) {

        try {
            *val = std::get<T>(data);
            return true;
        }
        catch (...) {
            return false;
        }
    }
    
};



/***************************************************************************/
/*!
\brief
    Defined resource type
*/
/**************************************************************************/
enum ResourceType : unsigned {

    _MESH = 1234567,
    _TEXTURE,
    _SHADER,
    _MATERIALINSTANCE
};

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
};

/***************************************************************************/
/*!
\brief
    New Resource Manager Class
*/
/**************************************************************************/
class ResourceTy
{
public:
    /***************************************************************************/
    /*!
    \brief
        Constructor for Resource Manager
    */
    /**************************************************************************/
    ResourceTy();
    ResourceTy(const ResourceTy&) = delete;
    ~ResourceTy() = default;

    void Init();
    void Exit();

    /***************************************************************************/
    /*!
    \brief
        Mesh accessors/initializer functions
    */
    /**************************************************************************/
    void mesh_LoadFolder();
    void mesh_Load(std::string filepath, unsigned uid);
    GFX::Mesh* SetupMesh(std::string filepath, unsigned);
    GFX::Mesh* get_mesh(unsigned );

    /***************************************************************************/
    /*!
    \brief
        Texture accessors/initializer functions
    */
    /**************************************************************************/
    void MaterialInstance_Loader();
    void MaterialEditor_Loader();
    GFX::Texture* SetupMaterialInstance(std::string filepath);
    GFX::Texture* getMaterialInstance(unsigned);
    GFX::Texture* SetupEditorlInstance(std::string filepath);
    
    /***************************************************************************/
    /*!
    \brief
        Allocation functions resource manager
    */
    /**************************************************************************/
    instance_infos& AllocRscInfo(void);
    void ReleaseRscInfo(instance_infos& RscInfo);


    constexpr static int  MAX_RESOURCE = 2400;
    const std::string compiled_geom_path = "../assets/compiled_geom/";
    const std::string compressed_texture_path = "../assets/Compressed/";
    const std::string compressed_Editor_path = "../assets/Editor/Textures_Compressed/";
    const std::string shader_path = "../assets/shader_files/";

    std::unordered_map<std::string, GFX::Texture*> m_EditorTextures;

    int mResouceCnt;
private:


    instance_infos*                                         m_pInfoBufferEmptyHead{ nullptr };
    std::unordered_map<std::uint64_t, instance_infos*>      m_ResourceInstance;
    std::array<instance_infos, MAX_RESOURCE>                m_Infobuffer;
};


#endif