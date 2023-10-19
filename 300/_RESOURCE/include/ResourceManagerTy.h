#pragma once 
#ifndef _RESOURCEMANAGERTY_H
#define _RESOURCEMANAGERTY_H




#include <filesystem>
#include "Guid.h"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include <Constants.h>
#include <variant>  





//struct p_ref {
//
//    std::variant<void*, unsigned> data;
//
//    template <typename T>
//    bool GetVariantData(T* val) {
//        try {
//            *val = std::get<T>(data);
//            return true;
//        }
//        catch (...) {
//            std::cout << " could not get reference to data!"
//            return false;
//        }
//    }   
//};


//struct ref {
//
//    p_ref partial_ref;F
//    unsigned data_uid;
//
//};
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



struct instance_infos
{
    std::string     m_Name{};
    void* m_pData{ nullptr }; // to store data / act as an pointer to link list if not used
    uid             m_GUID;
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


    void shader_Loader();
    GFX::Shader* get_Shader(unsigned);
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
    void texture_Load(std::string texturename, unsigned uid);
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
    const std::string fbx_path = "../assets/Models/";

    std::unordered_map<std::string, GFX::Texture*> m_EditorTextures;

    int mResouceCnt;
private:


    instance_infos*                                         m_pInfoBufferEmptyHead{ nullptr };
    std::unordered_map<std::uint64_t, instance_infos*>      m_ResourceInstance;
    std::array<instance_infos, MAX_RESOURCE>                m_Infobuffer;
};


#endif