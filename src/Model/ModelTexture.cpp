#include "ModelTexture.hpp"

ModelTexture::ModelTexture (const GraphicCore::Image& Texture)
    : texture (Texture) { }

float ModelTexture::getShineDamper( ) const { return shineDamper; }
float ModelTexture::getReflectivity( ) const { return reflectivity; }

void ModelTexture::setShineDamper (const float& ShineDamper) { shineDamper = ShineDamper; }
void ModelTexture::setReflectivity (const float& Reflectivity) { reflectivity = Reflectivity; }

const GraphicCore::Image& ModelTexture::getImage( ) const { return texture; }
