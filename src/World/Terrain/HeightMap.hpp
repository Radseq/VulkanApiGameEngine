#ifndef HEIGHT_MAP_HPP
#define HEIGHT_MAP_HPP

#include <cstdint>
#include <limits>
#include <string>
#include <ktxvulkan.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>


class HeightMap {
    uint16_t* heightdata;
    uint32_t  dim;
    uint32_t  scale;

   public:
    void  loadHeightMap (const std::string& filename, const uint32_t& patchsize);
    float getHeight (const uint32_t& x, const uint32_t& y);

    ~HeightMap( );
};

#endif  // HEIGHT_MAP_HPP
