#include "Guid.h"
#include "Mesh.hpp"

enum ResourceTypes : unsigned {

    TEXTURE = 1234567,
    MESH
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


    void get_Texture(uid<> id);
    void get_Mesh(uid<> id);
    void get_Texture(std::string name);
    void get_Mesh(std::string name);

    instance_info& AllocRscInfo(void);
    void ReleaseRscInfo(instance_info& RscInfo);

    constexpr static int  MAX_RESOURCE = 2400;

private:
    instance_info* m_pInfoBufferEmptyHead{ nullptr };
    std::unordered_map<unsigned, instance_info*> m_Textures;
    std::unordered_map<unsigned, instance_info*> m_Meshes;
    std::array<instance_info, MAX_RESOURCE>         m_Infobuffer;
};


class MeshManager {


};