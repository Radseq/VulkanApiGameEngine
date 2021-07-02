#ifndef HEIGHT_MAP_HPP
#define HEIGHT_MAP_HPP

#include "KtxTextureLoader.hpp"
#include "ResourcePatch.hpp"
#include "pch.h"

class HeightMap
{
    uint16_t* m_HeightData;
    uint32_t  m_Dim;
    uint32_t  m_Scale;

   public:
    void  loadHeightMap (const std::string& filename, const uint32_t& patchsize);
    float getHeight (const uint32_t& x, const uint32_t& y);

    ~HeightMap( );
};

#endif  // HEIGHT_MAP_HPP
