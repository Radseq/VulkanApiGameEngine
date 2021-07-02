#include "Shader.hpp"

namespace GraphicCore
{
    vk::ShaderModule Shader::loadShaderModule (const vk::Device& device, const std::string_view& filePatchName)
    {
        vk::ShaderModule result;
        {
            ifstreamLoader::getInstance( )->LoadFileBin (filePatchName);

            const auto fileData = ifstreamLoader::getInstance( )->readChars (filePatchName);

            vk::ShaderModuleCreateInfo shaderModuleCreateInfo;
            shaderModuleCreateInfo.setCodeSize (fileData.size( ));
            shaderModuleCreateInfo.setPCode (reinterpret_cast<const uint32_t*> (fileData.data( )));

            result = device.createShaderModule (shaderModuleCreateInfo);

            ifstreamLoader::getInstance( )->closeFileByName (filePatchName);
            // result = device.createShaderModule({{}, storage->size(), (const uint32_t*)storage->data()});
        }
        return result;
    }

    // Load a SPIR-V shader
    vk::PipelineShaderStageCreateInfo Shader::loadShader (const vk::Device& device, const std::string_view& filePatchName,
                                                          vk::ShaderStageFlagBits stage, const char* entryPoint)
    {
        vk::PipelineShaderStageCreateInfo shaderStage;
        shaderStage.stage  = stage;
        shaderStage.module = loadShaderModule (device, filePatchName);
        shaderStage.pName  = entryPoint;
        return shaderStage;
    }
}  // namespace GraphicCore
