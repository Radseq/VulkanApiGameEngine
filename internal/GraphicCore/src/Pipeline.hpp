#ifndef PIPLINE_HPP
#define PIPLINE_HPP

#include <CoreUtil.hpp>

#include "PipelineColorBlend.hpp"
#include "PipelineDepthStencil.hpp"
#include "PipelineDynamic.hpp"
#include "PipelineInputAssembly.hpp"
#include "PipelineRasterization.hpp"
#include "PipelineVertex.hpp"
#include "PipelineViewport.hpp"
#include "Shader.hpp"
#include "VertexLayout.hpp"

namespace GraphicCore
{
    class Pipeline
    {
        const vk::Device&              device;
        vk::PipelineCache              pipelineCache;
        vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
        vk::RenderPass&                _renderPass {pipelineCreateInfo.renderPass};
        vk::PipelineLayout&            _layout {pipelineCreateInfo.layout};
        uint32_t                       _subpass {pipelineCreateInfo.subpass};

        vk::PipelineMultisampleStateCreateInfo multisampleState { };

        PipelineDynamic       pipelineDynamic { };
        PipelineColorBlend    pipelineColorBlend { };
        PipelineViewport      pipelineViewport { };
        PipelineVertex        vertexInputState { };
        PipelineDepthStencil  pipelineDepthStencil { };
        PipelineRasterization pipelineRasterization { };
        PipelineInputAssembly pipelineInputAssembly { };

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages { };
        void                                           init( );

       public:
        Pipeline (const vk::Device& device, vk::PipelineLayout layout, const vk::RenderPass& renderPass);
        Pipeline (const Pipeline& other);
        ~Pipeline( );

        Pipeline& operator= (const Pipeline& other) = delete;

        void                               destroyShaderModules( );
        void                               update( );
        void                               appendVertexLayout (const VertexLayout& vertexLayout);
        void                               setSubPass (uint32_t subpass);
        vk::PipelineShaderStageCreateInfo& loadShader (const std::string_view&         filePatchName,
                                                       const vk::ShaderStageFlagBits&& stage,
                                                       const char*                     entryPoint = "main");
        vk::Pipeline                       create (const vk::PipelineCache& cache);
        vk::Pipeline                       create( );

        PipelineDepthStencil&  getPipelineDepthStencil( );
        PipelineRasterization& getPipelineRasterization( );
        PipelineColorBlend&    getPipelineColorBlend( );

        vk::PipelineMultisampleStateCreateInfo& getMultisampleState( );

        std::vector<vk::PipelineShaderStageCreateInfo>& getShaderStages( );

        PipelineVertex&        getVertexInputState( );
        PipelineInputAssembly& getPipelineInputAssembly( );
        PipelineDynamic&       getPipelineDynamic( );
        PipelineViewport&      getPipelineViewport( );

        vk::GraphicsPipelineCreateInfo& getPipelineCreateInfo( );
        vk::PipelineLayout&             getLayout( ) const;
        vk::RenderPass&                 getVkRenderPass( ) const;
    };
}  // namespace GraphicCore

#endif PIPLINE_HPP
