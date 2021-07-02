#ifndef MODEL_TEXTURE_HPP
#define MODEL_TEXTURE_HPP

#include <CoreImage.hpp>

#include "Model.hpp"
#include "pch.h"

/// <summary>
/// Used for set parameters to texture.
/// </summary>
class ModelTexture
{
    VulkanGame::Ref<GraphicCore::CoreImage> texture;

    /// <summary>
    /// how close camera should be to reflectivity parm to change brightness
    /// if shineDamper is close to reflectivity then brightness will be higher
    /// </summary>
    float shineDamper;

    /// <summary>
    /// how strong reflectivity should be
    /// </summary>
    float reflectivity;

   public:
    ModelTexture (VulkanGame::Ref<GraphicCore::CoreImage> Texture);

    VulkanGame::Ref<GraphicCore::CoreImage> getImage( ) const;

    float getShineDamper( ) const;
    void  setShineDamper (const float& ShineDamper);

    float getReflectivity( ) const;
    void  setReflectivity (const float& Reflectivity);
};
#endif /* MODEL_TEXTURE_HPP */