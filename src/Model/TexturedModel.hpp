#ifndef TEXTURED_MODEL_HPP
#define TEXTURED_MODEL_HPP

#include "Model.hpp"
#include "ModelTexture.hpp"

class TexturedModel {
    const Model*        model;
    VulkanGame::Ref<ModelTexture> texture;

   public:
    TexturedModel (const Model* Model, VulkanGame::Ref<ModelTexture> ModelTexture);

    Model const*        getModel( ) const;
    VulkanGame::Ref<ModelTexture> getModelTexture( ) const;
};
#endif /* TEXTURED_MODEL_HPP */