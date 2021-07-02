#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "pch.h"

class Light
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 attenuation;

   public:
    Light (glm::vec3 const& Position, glm::vec3 const& Color, glm::vec3 const& Attenuation = glm::vec3 (1, 0, 0));

    glm::vec3 const& getPosition( ) const;
    glm::vec3 const& getColor( ) const;
    glm::vec3 const& getAttenuation( ) const;

    void setPosition (glm::vec3 const& Position);
    void setColor (glm::vec3 const& Color);
};
#endif /* LIGHT_HPP */