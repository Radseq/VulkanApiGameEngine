#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <vulkan/vulkan.hpp>

#include "../../../../src/FileLoader/ifstreamLoader.hpp"

namespace GraphicCore {
    class Shader {
       public:
        static vk::ShaderModule loadShaderModule (const vk::Device& device, const std::string& filePatch,
                                                  const std::string& fileName);

        // Load a SPIR-V shader
        static vk::PipelineShaderStageCreateInfo loadShader (const vk::Device& device, const std::string& filePatch,
                                                             const std::string& fileName, vk::ShaderStageFlagBits stage,
                                                             const char* entryPoint = "main");
    };
}  // namespace GraphicCore

#endif SHADER_HPP