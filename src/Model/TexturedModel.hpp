#ifndef TEXTURED_MODEL_HPP
#define TEXTURED_MODEL_HPP

#include "Model.hpp"
#include "ModelTexture.hpp"

class TexturedModel {
    const Model*        model;
    const ModelTexture* texture;

   public:
    TexturedModel (const Model* Model, const ModelTexture* ModelTexture);

    Model const*        getModel( ) const;
    ModelTexture const* getModelTexture( ) const;
};
#endif /* TEXTURED_MODEL_HPP */