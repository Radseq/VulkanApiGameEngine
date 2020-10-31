#include "HeightMap.hpp"

float HeightMap::getHeight (const uint32_t& x, const uint32_t& y) {
    constexpr uint16_t maxUshort = std::numeric_limits<uint16_t>::max( );
    glm::ivec2         rpos      = glm::ivec2 (x, y) * glm::ivec2 (scale);
    rpos.x                       = std::max (0, std::min (rpos.x, static_cast<int> (dim) - 1));
    rpos.y                       = std::max (0, std::min (rpos.y, static_cast<int> (dim) - 1));
    rpos /= glm::ivec2 (scale);

    return static_cast<float> (*(heightdata + (rpos.x + rpos.y * dim) * scale) / maxUshort);
}

HeightMap::~HeightMap( ) { delete [] heightdata; }

void HeightMap::loadHeightMap (const std::string& filename, const uint32_t& patchsize) {
    ktxResult   result { };
    ktxTexture* ktxTexture { };
    result = ktxTexture_CreateFromNamedFile (filename.c_str( ), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);
    assert (result == KTX_SUCCESS);
    ktx_size_t   ktxSize  = ktxTexture_GetImageSize (ktxTexture, 0);
    ktx_uint8_t* ktxImage = ktxTexture_GetData (ktxTexture);
    dim                   = ktxTexture->baseWidth;
    heightdata            = new uint16_t [dim * dim];
    memcpy (heightdata, ktxImage, ktxSize);
    this->scale = dim / patchsize;
    ktxTexture_Destroy (ktxTexture);
}
