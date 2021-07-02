#include "Pipeline.hpp"

namespace GraphicCore
{
    void Pipeline::init( )
    {
        pipelineCreateInfo.pRasterizationState = &pipelineRasterization.getRasterizationState( );
        pipelineCreateInfo.pInputAssemblyState = &pipelineInputAssembly.getAssembly( );
        pipelineCreateInfo.pColorBlendState    = &pipelineColorBlend;
        pipelineCreateInfo.pMultisampleState   = &multisampleState;
        pipelineCreateInfo.pViewportState      = &pipelineViewport.getViewPort( );
        pipelineCreateInfo.pDepthStencilState  = &pipelineDepthStencil.getDepthStencil( );
        pipelineCreateInfo.pDynamicState       = &pipelineDynamic.getDynamicState( );
        pipelineCreateInfo.pVertexInputState   = &vertexInputState.getVertexInput( );
    }

    Pipeline::Pipeline (const vk::Device& device, const vk::PipelineLayout layout, const vk::RenderPass& renderPass)
        : device (device)
    {
        pipelineCreateInfo.layout     = layout;
        pipelineCreateInfo.renderPass = renderPass;
        _renderPass                   = renderPass;
        _layout                       = layout;

        init( );
    }

    Pipeline::Pipeline (const Pipeline& other)
        : Pipeline (other.device, other._layout, other._renderPass)
    {
    }

    Pipeline::~Pipeline( ) { destroyShaderModules( ); }

    void Pipeline::destroyShaderModules( )
    {
        for (const auto shaderStage : shaderStages) { device.destroyShaderModule (shaderStage.module); }
        shaderStages.clear( );
    }

    void Pipeline::update( )
    {
        // pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
        // pipelineCreateInfo.pStages = shaderStages.data();

        util::updatePointerFromVector (shaderStages, pipelineCreateInfo.stageCount, pipelineCreateInfo.pStages);

        pipelineDynamic.update( );
        pipelineColorBlend.update( );
        vertexInputState.update( );
        pipelineViewport.update( );
    }

    void Pipeline::appendVertexLayout (const VertexLayout& vertexLayout)
    {
        vertexInputState.appendVertexLayout (vertexLayout);
    }

    void Pipeline::setSubPass (uint32_t subpass) { _subpass = subpass; }

    // Load a SPIR-V shader
    vk::PipelineShaderStageCreateInfo& Pipeline::loadShader (const std::string_view&         filePatchName,
                                                             const vk::ShaderStageFlagBits&& stage,
                                                             const char*                     entryPoint)
    {
        const vk::PipelineShaderStageCreateInfo shaderStage =
            Shader::loadShader (device, filePatchName, stage, entryPoint);
        shaderStages.push_back (shaderStage);
        return shaderStages.back( );
    }

    vk::Pipeline Pipeline::create (const vk::PipelineCache& cache)
    {
        update( );

        vk::ResultValue<vk::Pipeline> result = device.createGraphicsPipeline (cache, pipelineCreateInfo);
        if (result.result != vk::Result::eSuccess) { throw std::runtime_error ("failed to create pipeline"); }
        else
        {
            return result.value;
        }
    }

    vk::Pipeline Pipeline::create( )
    {
        pipelineCache = device.createPipelineCache (vk::PipelineCacheCreateInfo( ));
        return create (pipelineCache);
    }

    PipelineDepthStencil&                   Pipeline::getPipelineDepthStencil( ) { return pipelineDepthStencil; }
    PipelineRasterization&                  Pipeline::getPipelineRasterization( ) { return pipelineRasterization; }
    PipelineColorBlend&                     Pipeline::getPipelineColorBlend( ) { return pipelineColorBlend; }
    vk::PipelineMultisampleStateCreateInfo& Pipeline::getMultisampleState( ) { return multisampleState; }
    std::vector<vk::PipelineShaderStageCreateInfo>& Pipeline::getShaderStages( ) { return shaderStages; }
    PipelineVertex&                                 Pipeline::getVertexInputState( ) { return vertexInputState; }
    PipelineInputAssembly&          Pipeline::getPipelineInputAssembly( ) { return pipelineInputAssembly; }
    PipelineDynamic&                Pipeline::getPipelineDynamic( ) { return pipelineDynamic; }
    PipelineViewport&               Pipeline::getPipelineViewport( ) { return pipelineViewport; }
    vk::GraphicsPipelineCreateInfo& Pipeline::getPipelineCreateInfo( ) { return pipelineCreateInfo; }
    vk::PipelineLayout&             Pipeline::getLayout( ) const { return _layout; }
    vk::RenderPass&                 Pipeline::getVkRenderPass( ) const { return _renderPass; }
}  // namespace GraphicCore
