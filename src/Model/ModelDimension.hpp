#ifndef MODEL_DIMENSION_HPP
#define MODEL_DIMENSION_HPP

#include <glm/glm.hpp>

class ModelDimension {
   public:
    glm::vec3 min = glm::vec3 (FLT_MAX);
    glm::vec3 max = glm::vec3 (-FLT_MAX);
    glm::vec3 size;
};

#endif  // MODEL_DIMENSION_HPP