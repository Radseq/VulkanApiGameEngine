#include "ModelCreateInfo.hpp"

void ModelCreateInfo::setCenter (const glm::vec3& Center) { center = Center; }
void ModelCreateInfo::setScale (const glm::vec3& Scale) { scale = Scale; }
void ModelCreateInfo::setUvScale (const glm::vec2& UvScale) { uvscale = UvScale; }

glm::vec3 ModelCreateInfo::getCenter( ) const { return center; }
glm::vec3 ModelCreateInfo::getScale( ) const { return scale; }
glm::vec2 ModelCreateInfo::getUvScale( ) const { return uvscale; }
