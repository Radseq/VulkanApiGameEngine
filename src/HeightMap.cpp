#include "pch.h"
#include "HeightMap.hpp"

float HeightMap::getHeight (const uint32_t& x, const uint32_t& y)
{
    constexpr uint16_t maxUshort = std::numeric_limits<uint16_t>::max( );
    glm::ivec2         rpos      = glm::ivec2 (x, y) * glm::ivec2 (m_Scale);
    rpos.x                       = std::max (0, std::min (rpos.x, static_cast<int> (m_Dim) - 1));
    rpos.y                       = std::max (0, std::min (rpos.y, static_cast<int> (m_Dim) - 1));
    rpos /= glm::ivec2 (m_Scale);

    return static_cast<float> (*(m_HeightData + (rpos.x + rpos.y * m_Dim) * m_Scale) / maxUshort);
}

HeightMap::~HeightMap( ) { delete [] m_HeightData; }

void HeightMap::loadHeightMap (const std::string& filename, const uint32_t& patchsize)
{
    KtxTextureLoader textureLoader;
    auto imgContainer = textureLoader.LoadFile (ResourcePatch::GetInstance( )->GetPatch ("DataPatch") + filename);

    m_HeightData = static_cast<uint16_t*> (imgContainer.TextureData);
    m_Dim        = imgContainer.TextureExtend.width;
    m_Scale      = m_Dim / patchsize;
}
