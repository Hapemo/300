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
struct instance_infos
{
    std::string     m_Name{};
    void* m_pData{ nullptr }; // to store data / act as an pointer to link list if not used
    uid             m_GUID;
    unsigned        m_Type;
    int             m_RefCount{ 1 };
};

struct ref {

    void*       data {nullptr};
    unsigned    data_uid {0};
    
    void* getdata(std::unordered_map<std::uint64_t, instance_infos*>& m_ResourceInstance) {

        if (data == nullptr) {
            auto it = m_ResourceInstance.find(data_uid);
            if (it == m_ResourceInstance.end())
                return nullptr;

            data = it->second->m_pData;
        }
        return data;
    };
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





/***************************************************************************/
/*!
\brief
    New Resource Manager Class
*/
/**************************************************************************/
class ResourceTy
{
public:


    std::pair<std::string, std::pair<std::string, std::string>> deserialize_Shader(std::string filename);
    bool serialize_Shader(std::string shaderProgram, std::pair < std::string, std::string> shaderPair);
    void create_Shader(std::string ShaderPrgm, std::string vertPath, std::string fragPath);
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
    const std::string shader_program_path = "../assets/ShaderProgram/";
    const std::string fbx_path = "../assets/Models/";

    std::unordered_map<std::string, GFX::Texture*> m_EditorTextures;
    std::unordered_map<std::uint64_t,std::pair<std::string, ref>> m_Shaders;
    std::unordered_map<std::uint64_t, instance_infos*>      m_ResourceInstance;

    int mResouceCnt;
private:


    instance_infos*                                         m_pInfoBufferEmptyHead{ nullptr };

    std::array<instance_infos, MAX_RESOURCE>                m_Infobuffer;
};


#endif