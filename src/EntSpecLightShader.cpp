#include "pch.h"
#include "EntSpecLightShader.hpp"

const std::vector<std::string_view> EntSpecLightShader::GetAllParms( )
{
    std::vector<std::string_view> result (avaliablesParms.begin( ), avaliablesParms.end( ));
    return result;
}

void EntSpecLightShader::SetVec3 (const std::string_view& objParmName, const glm::vec3& parm)
{
    if (ParmName (quote (shader.lightPosition)) == objParmName)
    {
        assert (CheckType (shader.lightPosition, parm));
        shader.lightPosition = parm;
    }
}

void EntSpecLightShader::SetMat4 (const std::string_view& objParmName, const glm::mat4& parm)
{
    if (ParmName (quote (shader.model)) == objParmName)
    {
        assert (CheckType (shader.model, parm));
        shader.model = parm;
        return;
    }
    if (ParmName (quote (shader.view)) == objParmName)
    {
        assert (CheckType (shader.view, parm));
        shader.view = parm;
        return;
    }
    if (ParmName (quote (shader.proj)) == objParmName)
    {
        assert (CheckType (shader.proj, parm));
        shader.proj = parm;
    }
}

void EntSpecLightShader::Create (const GraphicCore::VulkanDevice& context, const uint32_t& count)
{
    uniform.create (context, count, sizeof (EntShader));
}

const vk::DescriptorBufferInfo& EntSpecLightShader::GetUniformDesc (const uint32_t& index)
{
    return uniform.getUniformBuffers( ) [index].getDescriptorInfo( );
}

void EntSpecLightShader::Destroy (const GraphicCore::VulkanDevice& context) { uniform.destroyBuffer (context); }

void EntSpecLightShader::Update (const uint32_t& currentImage) { uniform.updateUniformBuffer (currentImage, shader); }