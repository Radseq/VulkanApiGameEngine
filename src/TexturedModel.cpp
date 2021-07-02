#include "pch.h"
#include "TexturedModel.hpp"

TexturedModel::TexturedModel (const Model* Model, VulkanGame::Ref<ModelTexture> Texture)
    : model (Model)
    , texture (Texture)
{
}

Model const*                  TexturedModel::getModel( ) const { return model; }
VulkanGame::Ref<ModelTexture> TexturedModel::getModelTexture( ) const { return texture; }
