#ifndef ISHADER_FLOAT_PARM_HPP
#define ISHADER_FLOAT_PARM_HPP

#include "pch.h"

class IShaderFloatParm
{
   public:
    virtual ~IShaderFloatParm( )                                                   = default;
    virtual void SetFloat (const std::string_view& objParmName, const float& parm) = 0;
};

#endif  // ISHADER_FLOAT_PARM_HPP
