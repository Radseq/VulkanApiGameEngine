#include "LightShader.hpp"

const std::vector<std::string_view> LightShader::GetAllParms( )
{
    std::vector<std::string_view> result (avaliablesParms.begin( ), avaliablesParms.end( ));
    return result;
}

void LightShader::SetVec3 (const std::string_view& objParmName, const glm::vec3& parm)
{
    if (ParmName (quote (shader.lightColor)) == objParmName)
    {
        assert (CheckType (shader.lightColor, parm));
        shader.lightColor = parm;
    }
}

void LightShader::SetFloat (const std::string_view& objParmName, const float& parm)
{
    if (ParmName (quote (shader.shineDamper)) == objParmName)
    {
        assert (CheckType (shader.shineDamper, parm));
        shader.shineDamper = parm;
        return;  // if we are here we dont have to check other parm
    }
    if (ParmName (quote (shader.reflectivity)) == objParmName)
    {
        assert (CheckType (shader.shineDamper, parm));
        shader.reflectivity = parm;
    }
}

void LightShader::Create (const GraphicCore::VulkanDevice& context, const uint32_t& count)
{
    lightUniform.create (context, count, sizeof (lightUniformBufferObject));
}

const vk::DescriptorBufferInfo& LightShader::GetUniformDesc (const uint32_t& index)
{
    return lightUniform.getUniformBuffers( ) [index].getDescriptorInfo( );
}

void LightShader::Destroy (const GraphicCore::VulkanDevice& context) { lightUniform.destroyBuffer (context); }

void LightShader::Update (const uint32_t& currentImage) { lightUniform.updateUniformBuffer (currentImage, shader); }