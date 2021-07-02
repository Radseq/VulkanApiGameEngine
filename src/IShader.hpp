#ifndef ISHADER_HPP
#define ISHADER_HPP

#include "IShaderFloatParm.hpp"
#include "IShaderMat4Parm.hpp"
#include "IShaderUniform.hpp"
#include "IShaderUniformUpdate.hpp"
#include "IShaderVec3Parm.hpp"
#include "pch.h"

#define quote(x) #x

class IShader
    : virtual public IShaderFloatParm
    , virtual public IShaderVec3Parm
    , virtual public IShaderUniformUpdate
    , virtual public IShaderUniform
    , virtual public IShaderMat4Parm
{
   public:
    virtual ~IShader( )                                        = default;
    virtual const std::vector<std::string_view> GetAllParms( ) = 0;

    virtual void SetVec3 (const std::string_view& objParmName, const glm::vec3& parm) = 0;
    virtual void SetMat4 (const std::string_view& objParmName, const glm::mat4& parm) = 0;
    virtual void SetFloat (const std::string_view& objParmName, const float& parm)    = 0;

    virtual void Create (const GraphicCore::VulkanDevice& context, const uint32_t& count) = 0;
    virtual void Destroy (const GraphicCore::VulkanDevice& context)                       = 0;
    virtual void Update (const uint32_t& currentImage)                                    = 0;
    virtual const vk::DescriptorBufferInfo& GetUniformDesc (const uint32_t& index)        = 0;
};

#endif  // ISHADER_HPP
