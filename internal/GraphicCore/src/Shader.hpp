#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>

#include "pch.h"

namespace GraphicCore
{
    class Shader
    {
        static std::vector<char> readFile (const std::string_view& fileName);
        static vk::ShaderModule  loadShaderModule (const vk::Device& device, const std::string_view& filePatchName);

       public:
        // Load a SPIR-V shader
        static vk::PipelineShaderStageCreateInfo loadShader (const vk::Device&       device,
                                                             const std::string_view& filePatchName,
                                                             vk::ShaderStageFlagBits stage,
                                                             const char*             entryPoint = "main");
    };
}  // namespace GraphicCore

#endif SHADER_HPP
