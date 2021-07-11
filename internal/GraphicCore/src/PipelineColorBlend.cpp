#include "pch.h"
#include "PipelineColorBlend.hpp"

namespace GraphicCore
{
    void PipelineColorBlend::update( )
    {
        Util::updatePointerFromVector (blendAttachmentStates, this->attachmentCount, this->pAttachments);
    }

    std::vector<vk::PipelineColorBlendAttachmentState>& PipelineColorBlend::getBlendAttachmentStates( )
    {
        return blendAttachmentStates;
    }
}  // namespace GraphicCore
