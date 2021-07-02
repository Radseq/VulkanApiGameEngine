#include "ModelTexture.hpp"

ModelTexture::ModelTexture (VulkanGame::Ref<GraphicCore::Image> Texture)
    : texture (Texture)
{
}

float ModelTexture::getShineDamper( ) const { return shineDamper; }
float ModelTexture::getReflectivity( ) const { return reflectivity; }

void ModelTexture::setShineDamper (const float& ShineDamper) { shineDamper = ShineDamper; }
void ModelTexture::setReflectivity (const float& Reflectivity) { reflectivity = Reflectivity; }

VulkanGame::Ref<GraphicCore::Image> ModelTexture::getImage( ) const { return texture; }
