#include "PipelineColorBlend.hpp"

namespace GameCore {
    void PipelineColorBlend::update( ) {
        // blendAttachmentStates.push_back(pipelineColorBlendAttachment.returnColorBlendAttachment());

        // this->attachmentCount = (uint32_t)blendAttachmentStates.size();
        // this->pAttachments = blendAttachmentStates.data();
        util::updatePointerFromVector (blendAttachmentStates, this->attachmentCount, this->pAttachments);
    }

    std::vector<vk::PipelineColorBlendAttachmentState>& PipelineColorBlend::getBlendAttachmentStates( ) {
        return blendAttachmentStates;
    }
}  // namespace GameCore
