#include "TexturedModel.hpp"

TexturedModel::TexturedModel (const Model* Model, const ModelTexture* Texture)
    : model (Model)
    , texture (Texture) { }

Model const*        TexturedModel::getModel( ) const { return model; }
ModelTexture const* TexturedModel::getModelTexture( ) const { return texture; }
