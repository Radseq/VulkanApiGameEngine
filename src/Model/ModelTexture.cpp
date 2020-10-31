#include "ModelTexture.hpp"

ModelTexture::ModelTexture (const GameCore::Image& Texture)
    : texture (Texture) { }

float ModelTexture::getShineDamper( ) const { return shineDamper; }
float ModelTexture::getReflectivity( ) const { return reflectivity; }

void ModelTexture::setShineDamper (const float& ShineDamper) { shineDamper = ShineDamper; }
void ModelTexture::setReflectivity (const float& Reflectivity) { reflectivity = Reflectivity; }

const GameCore::Image& ModelTexture::getImage( ) const { return texture; }
