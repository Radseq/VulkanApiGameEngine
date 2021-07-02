
#ifndef IMODEL_LOADER_HPP
#define IMODEL_LOADER_HPP

#include "Model.hpp"
#include "ModelCreateInfo.hpp"
#include "pch.h"

class IModelLoad
{
   public:
    virtual void loadFromFile (Model* const model, const std::string_view& filename,
                               ModelCreateInfo* const createInfo) = 0;
};

#endif  // IMODEL_LOADER_HPP