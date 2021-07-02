#ifndef MODEL_CREATE_INFO_HPP
#define MODEL_CREATE_INFO_HPP

#include "pch.h"

/** @brief Used to parametrize model loading */
class ModelCreateInfo
{
    glm::vec3 center {0};
    glm::vec3 scale {1};
    glm::vec2 uvscale {1};

   public:
    ModelCreateInfo( ) = default;

    ModelCreateInfo (const glm::vec3& scale, const glm::vec2& uvscale, const glm::vec3& center)
        : center (center)
        , scale (scale)
        , uvscale (uvscale)
    {
    }

    ModelCreateInfo (const float& scale, const float& uvscale, const float& center)
        : ModelCreateInfo (glm::vec3 (scale), glm::vec2 {uvscale}, glm::vec3 (center))
    {
    }

    void setCenter (const glm::vec3& Center);
    void setScale (const glm::vec3& Scale);
    void setUvScale (const glm::vec2& UvScale);

    glm::vec3 getCenter( ) const;
    glm::vec3 getScale( ) const;
    glm::vec2 getUvScale( ) const;
};

#endif  // MODEL_CREATE_INFO_HPP