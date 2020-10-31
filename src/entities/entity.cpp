#include "entity.hpp"

Entity::Entity (TexturedModel const* Model, glm::vec3 const& Position, glm::vec3 const& Rotation,
                glm::vec3 const& scale)
    : model (Model)
    , position (Position)
    , rotation (Rotation)
    , scale (scale) { }

void Entity::increase_position (glm::vec3 const& Translation) { position += Translation; }
void Entity::increase_rotation (glm::vec3 const& Rotation) { rotation += Rotation; }
void Entity::increase_scale (glm::vec3 const& Scale) { scale *= Scale; }
/*
float Entity::getTextureXOffset( ) const {
    int column = textureIndex_ % model_->getModelTexture( ).getNumberOfRows( );
    return (float) column / model_->getModelTexture( ).getNumberOfRows( );
}

float Entity::getTextureYOffset( ) const {
    int row = textureIndex_ / model_->getModelTexture( ).getNumberOfRows( );
    return (float) row / model_->getModelTexture( ).getNumberOfRows( );
}*/

void Entity::setModel (TexturedModel const* Model) { model = Model; }
void Entity::setPosition (glm::vec3 const& Position) { position = Position; }
void Entity::setRotation (glm::vec3 const& Rotation) { rotation = Rotation; }
void Entity::setScale (glm::vec3 const& Scale) { scale = Scale; }

TexturedModel const* Entity::getTexturedModel( ) const { return model; }
glm::vec3 const&     Entity::getPosition( ) const { return position; }
glm::vec3 const&     Entity::getRotation( ) const { return rotation; }
glm::vec3 const&     Entity::getScale( ) const { return scale; }
