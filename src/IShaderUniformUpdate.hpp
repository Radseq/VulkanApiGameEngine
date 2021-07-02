#ifndef ISHADER_UNIFORM_UPDATE_HPP
#define ISHADER_UNIFORM_UPDATE_HPP

#include "pch.h"

class IShaderUniformUpdate
{
   public:
    virtual ~IShaderUniformUpdate( )                   = default;
    virtual void Update (const uint32_t& currentImage) = 0;
};

#endif  // ISHADER_UNIFORM_UPDATE_HPP
