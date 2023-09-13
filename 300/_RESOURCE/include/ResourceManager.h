#include "Guid.h"
#include "Mesh.hpp"
#include <filesystem>

enum ResourceTypes : unsigned {

    MESH = 1234567,
    TEXTURE
};


struct MeshData {
    void* m_pData;
    GFX::Mesh meshdata;
};

class MeshManager
{
public:
    constexpr static int  MAX_RESOURCE = 2400;

    //std::vector<Mesh> mSceneMeshes;


    MeshData& AllocRscInfo();
    void ReleaseRscInfo(MeshData& RscInfo);

    
    void SetupMesh(std::string filepath,unsigned );

public:
    static MeshManager& GetInstance()
    {
        static MeshManager instance;
        return instance;
    }

    MeshManager() = default;
    MeshManager(MeshManager const&) = delete;
    void Init();

    MeshData& get_Mesh(unsigned);


    //void Destroy();

    void operator=(MeshManager const&) = delete;

    int mResouceCnt{ 0 };

private:

    std::unordered_map<unsigned, MeshData*> mSceneMeshes;
    MeshData* m_pInfoBufferEmptyHead{ nullptr };
    std::array<MeshData, MAX_RESOURCE>         m_Meshbuffer;
};

struct instance_info {
    std::string     m_Name{};
    void* m_pData{ nullptr };
    uid<unsigned>   m_GUID;
    unsigned        m_Type;
    int             m_RefCount{ 1 };
};

class Resource {

public:


    

    Resource();
    Resource(const Resource&) = delete;
    ~Resource() = default;


    void mesh_Loader();

    void get_Texture(uid<> id);
    void get_Mesh(uid<> id);
    void get_Texture(std::string name);
    void get_Mesh(std::string name);

    instance_info& AllocRscInfo(void);
    void ReleaseRscInfo(instance_info& RscInfo);

    constexpr static int  MAX_RESOURCE = 2400;

    int mResouceCnt{ 0 };

private:
    instance_info*                                      m_pInfoBufferEmptyHead{ nullptr };
    std::unordered_map<unsigned, instance_info*>        m_Textures;
    std::unordered_map<unsigned, instance_info*>        m_Meshes;
    std::array<instance_info, MAX_RESOURCE>             m_Infobuffer;

    const std::string compiled_geom_path = "../compiled_geom/";

    MeshManager mMeshManager;
};



