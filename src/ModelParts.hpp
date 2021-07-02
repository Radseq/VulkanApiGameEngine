#ifndef MODEL_PARTS_HPP
#define MODEL_PARTS_HPP

#include "pch.h"

/** @brief Stores vertex and index base and counts for each part of a model */
class ModelPart
{
   public:
    std::string name;
    uint32_t    vertexBase;
    uint32_t    vertexCount;
    uint32_t    indexBase;
    uint32_t    indexCount;
};

#endif  // MODEL_PARTS_HPP