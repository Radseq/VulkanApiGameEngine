#include "pch.h"
#include "Shader.hpp"

namespace GraphicCore
{
    std::vector<char> Shader::readFile (const std::string_view& fileName)
    {
        std::ifstream m_File (fileName.data( ), std::ios::ate | std::ios::binary);

        size_t            fileSize = static_cast<size_t> (m_File.tellg( ));
        std::vector<char> buffer (fileSize);

        m_File.seekg (0);
        m_File.read (buffer.data( ), fileSize);

        m_File.close( );

        return buffer;
    }

    vk::ShaderModule Shader::loadShaderModule (const vk::Device& device, const std::string_view& filePatchName)
    {
        std::vector<char> loaded = readFile (filePatchName);
        vk::ShaderModule  result;
        {
            vk::ShaderModuleCreateInfo shaderModuleCreateInfo {vk::ShaderModuleCreateFlags( ), loaded.size( ),
                                                               reinterpret_cast<const uint32_t*> (loaded.data( ))};

            result = device.createShaderModule (shaderModuleCreateInfo);
        }
        return result;
    }

    // Load a SPIR-V shader
    vk::PipelineShaderStageCreateInfo Shader::loadShader (const vk::Device&       device,
                                                          const std::string_view& filePatchName,
                                                          vk::ShaderStageFlagBits stage, const char* entryPoint)
    {
        vk::PipelineShaderStageCreateInfo shaderStage {vk::PipelineShaderStageCreateFlags( ), stage,
                                                       loadShaderModule (device, filePatchName), entryPoint};

        return shaderStage;
    }
}  // namespace GraphicCore
