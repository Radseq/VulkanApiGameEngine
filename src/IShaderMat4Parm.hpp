#ifndef ISHADER_MAT4_PARM_HPP
#define ISHADER_MAT4_PARM_HPP

#include "pch.h"

class IShaderMat4Parm
{
   public:
    virtual ~IShaderMat4Parm( )                                                       = default;
    virtual void SetMat4 (const std::string_view& objParmName, const glm::mat4& parm) = 0;
};

#endif  // ISHADER_MAT4_PARM_HPP
