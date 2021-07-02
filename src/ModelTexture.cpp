#include "pch.h"
#include "ModelTexture.hpp"

ModelTexture::ModelTexture (VulkanGame::Ref<GraphicCore::CoreImage> Texture)
    : texture (Texture)
{
}

float ModelTexture::getShineDamper( ) const { return shineDamper; }
float ModelTexture::getReflectivity( ) const { return reflectivity; }

void ModelTexture::setShineDamper (const float& ShineDamper) { shineDamper = ShineDamper; }
void ModelTexture::setReflectivity (const float& Reflectivity) { reflectivity = Reflectivity; }

VulkanGame::Ref<GraphicCore::CoreImage> ModelTexture::getImage( ) const { return texture; }
