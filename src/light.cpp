#include "pch.h"
#include "light.hpp"

Light::Light (glm::vec3 const& Position, glm::vec3 const& Color, glm::vec3 const& Attenuation)
    : position (Position)
    , color (Color)
    , attenuation (Attenuation)
{
}

glm::vec3 const& Light::getPosition( ) const { return position; }
glm::vec3 const& Light::getColor( ) const { return color; }
glm::vec3 const& Light::getAttenuation( ) const { return attenuation; }

void Light::setPosition (glm::vec3 const& Position) { position = Position; }
void Light::setColor (glm::vec3 const& Color) { color = Color; }
