#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/vec3.hpp>

//#include "models/textured_model.hpp"

#include "../Model/TexturedModel.hpp"

class Entity
{
   public:
    Entity( ) = default;
    Entity (VulkanGame::Ref<TexturedModel> Model, glm::vec3 const& Position, glm::vec3 const& Rotation,
            glm::vec3 const& Scale);

    void increase_position (glm::vec3 const& Translation);
    void increase_rotation (glm::vec3 const& Rotation);
    void increase_scale (glm::vec3 const& scale);

    // float getTextureXOffset( ) const;
    // float getTextureYOffset( ) const;

    VulkanGame::Ref<TexturedModel> getTexturedModel( ) const;
    void                           setModel (VulkanGame::Ref<TexturedModel> Model);
    void                           setPosition (glm::vec3 const& Position);
    void                           setRotation (glm::vec3 const& Rotation);
    void                           setScale (glm::vec3 const& Scale);

    glm::vec3 const& getRotation( ) const;
    glm::vec3 const& getPosition( ) const;
    glm::vec3 const& getScale( ) const;

   protected:
    VulkanGame::Ref<TexturedModel> model;
    glm::vec3                      position;
    glm::vec3                      rotation;
    glm::vec3                      scale;
};

#endif /* ENTITY_HPP */