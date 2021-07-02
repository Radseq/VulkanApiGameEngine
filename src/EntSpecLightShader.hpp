#ifndef ENT_SPEC_LIGHT_SHADER_HPP
#define ENT_SPEC_LIGHT_SHADER_HPP

#include "IShader.hpp"
#include "IShaderFloatParm.hpp"
#include "IShaderUniform.hpp"
#include "IShaderUniformUpdate.hpp"
#include "IShaderVec3Parm.hpp"
#include "UniformBuffer.hpp"
#include "pch.h"

class EntSpecLightShader : public IShader
{
    const std::array<std::string_view, 4> avaliablesParms = {"model", "view", "proj", "lightPosition"};

    struct EntShader
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        glm::vec3 lightPosition;
    };

    EntShader     shader = { };
    UniformBuffer uniform;

    std::string_view ParmName (const std::string_view& parm)
    {
        std::string_view result = parm.substr (parm.find (".") + 1);
        return result;
    }

    template <typename T> bool CheckType (const T& t, const T& t2) { return typeid (t) == typeid (t2); };

   public:
    const std::vector<std::string_view> GetAllParms( ) override;

    void SetVec3 (const std::string_view& objParmName, const glm::vec3& parm) override;
    // Ye i know, but i dont have better idea for shaders
    void SetMat4 (const std::string_view& objParmName, const glm::mat4& parm) override;
    void SetFloat (const std::string_view& objParmName, const float& parm) override { };

    void                            Create (const GraphicCore::VulkanDevice& context, const uint32_t& count) override;
    void                            Destroy (const GraphicCore::VulkanDevice& context) override;
    void                            Update (const uint32_t& currentImage) override;
    const vk::DescriptorBufferInfo& GetUniformDesc (const uint32_t& index) override;
};

#endif  // ENT_SPEC_LIGHT_SHADER_HPP