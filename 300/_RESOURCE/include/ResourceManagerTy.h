#include "Mesh.hpp"
#include <filesystem>





template <typename RESOURCE_T>
struct type {

};

template<>
struct type<int> {
    int data;
};

//struct MeshDatas {
//    void* m_pData;
//    GFX::Mesh meshdata;
//};

//struct instance_info
//{
//    std::string     m_Name{};
//    void* m_pData{ nullptr };
//    //uid<unsigned>   m_GUID;
//    unsigned        m_Type;
//    int             m_RefCount{ 1 };
//   // type<T>          data;
//};


class ResourceTy
{
public:
    ResourceTy();
    ResourceTy(const ResourceTy&) = delete;
    ~ResourceTy() = default;


    constexpr static int  MAX_RESOURCE = 2400;

private:
    //instance_info* m_pInfoBufferEmptyHead{ nullptr };
    //std::array<instance_info, MAX_RESOURCE>             m_Infobuffer;
};