#include "Shader.hpp"

namespace GraphicCore {
    vk::ShaderModule Shader::loadShaderModule (const vk::Device& device, const std::string& filePatch,
                                               const std::string& fileName) {
        vk::ShaderModule result;
        {
            ifstreamLoader::getInstance( )->LoadFileBin (filePatch, fileName);

            const auto fileData = ifstreamLoader::getInstance( )->readChars (fileName);

            vk::ShaderModuleCreateInfo shaderModuleCreateInfo;
            shaderModuleCreateInfo.setCodeSize (fileData.size( ));
            shaderModuleCreateInfo.setPCode (reinterpret_cast<const uint32_t*> (fileData.data( )));

            result = device.createShaderModule (shaderModuleCreateInfo);

            ifstreamLoader::getInstance( )->closeFileByName (fileName);
            // result = device.createShaderModule({{}, storage->size(), (const uint32_t*)storage->data()});
        }
        return result;
    }

    // Load a SPIR-V shader
    vk::PipelineShaderStageCreateInfo Shader::loadShader (const vk::Device& device, const std::string& filePatch,
                                                          const std::string& fileName, vk::ShaderStageFlagBits stage,
                                                          const char* entryPoint) {
        vk::PipelineShaderStageCreateInfo shaderStage;
        shaderStage.stage  = stage;
        shaderStage.module = loadShaderModule (device, filePatch, fileName);
        shaderStage.pName  = entryPoint;
        return shaderStage;
    }
}  // namespace GraphicCore
