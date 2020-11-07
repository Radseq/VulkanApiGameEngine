#ifndef MODEL_TEXTURE_HPP
#define MODEL_TEXTURE_HPP

#include <Image\Image.hpp>

#include "Model.hpp"

/// <summary>
/// Used for set parameters to texture.
/// </summary>
class ModelTexture {
    const GraphicCore::Image& texture;

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
    ModelTexture (const GraphicCore::Image& Texture);

    const GraphicCore::Image& getImage( ) const;

    float getShineDamper( ) const;
    void  setShineDamper (const float& ShineDamper);

    float getReflectivity( ) const;
    void  setReflectivity (const float& Reflectivity);
};
#endif /* MODEL_TEXTURE_HPP */