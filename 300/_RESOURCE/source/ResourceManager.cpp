#include "ResourceManager.h"

Resource::Resource() {
    for (int i = 0, end = (int)m_Infobuffer.size() - 1; i != end; ++i)
    {
        m_Infobuffer[i].m_pData = &m_Infobuffer[i + 1];
    }
    m_Infobuffer[m_Infobuffer.size() - 1].m_pData = nullptr;
    m_pInfoBufferEmptyHead = m_Infobuffer.data();
}

instance_info& Resource::AllocRscInfo(void)
{
    auto pTemp = m_pInfoBufferEmptyHead;


    instance_info* pNext = reinterpret_cast<instance_info*>(m_pInfoBufferEmptyHead->m_pData);
    m_pInfoBufferEmptyHead = pNext;

    return *pTemp;
}

void Resource::ReleaseRscInfo(instance_info& RscInfo)
{
    // Add this resource info to the empty chain
    RscInfo.m_pData = m_pInfoBufferEmptyHead;
    m_pInfoBufferEmptyHead = &RscInfo;
}