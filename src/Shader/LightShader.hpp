#ifndef LIGHT_SHADER_HPP
#define LIGHT_SHADER_HPP

#include <array>
#include <string_view>
#include <typeinfo>

#include "../UniformBuffers/UniformBuffer.hpp"
#include "../ShaderInterfaces/IShader.hpp"
#include "../ShaderInterfaces/IShaderFloatParm.hpp"
#include "../ShaderInterfaces/IShaderUniform.hpp"
#include "../ShaderInterfaces/IShaderUniformUpdate.hpp"
#include "../ShaderInterfaces/IShaderVec3Parm.hpp"

class LightShader : public IShader
{
    const std::array<std::string_view, 3> avaliablesParms = {"lightColor", "shineDamper", "reflectivity"};

    struct lightUniformBufferObject
    {
        glm::vec3 lightColor;
        float     shineDamper;
        float     reflectivity;
    };

    lightUniformBufferObject shader { };
    UniformBuffer            lightUniform;

    std::string_view ParmName (const std::string_view& parm)
    {
        std::string_view result = parm.substr (parm.find (".") + 1);
        return result;
    }

    template <typename T> bool CheckType (const T& t, const T& t2) { return typeid (t) == typeid (t2); };

   public:
    const std::vector<std::string_view> GetAllParms( ) override;

    void SetVec3    (const std::string_view& objParmName, const glm::vec3& parm)    override;
    // Ye i know, but i dont have better idea for shaders
    void SetMat4    (const std::string_view& objParmName, const glm::mat4& parm)    override { };
    void SetFloat   (const std::string_view& objParmName, const float& parm)        override;

    void                            Create  (const GraphicCore::VulkanDevice& context, const uint32_t& count) override;
    void                            Destroy (const GraphicCore::VulkanDevice& context)                  override;
    void                            Update  (const uint32_t& currentImage)                              override;
    const vk::DescriptorBufferInfo& GetUniformDesc (const uint32_t& index)                              override;
};

#endif  // LIGHT_SHADER_HPP
