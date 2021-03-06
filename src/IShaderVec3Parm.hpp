#ifndef ISHADER_VEC3_PARM_HPP
#define ISHADER_VEC3_PARM_HPP

#include "pch.h"

class IShaderVec3Parm
{
   public:
    virtual ~IShaderVec3Parm( )                                                       = default;
    virtual void SetVec3 (const std::string_view& objParmName, const glm::vec3& parm) = 0;
};

#endif  // ISHADER_VEC3_PARM_HPP
