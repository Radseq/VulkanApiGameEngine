#include "PipelineColorBlendAttachment.hpp"

namespace GameCore
{
	PipelineColorBlendAttachment::PipelineColorBlendAttachment
	(
		vk::Bool32 blendEnable_,
		vk::BlendFactor srcColorBlendFactor_,
		vk::BlendFactor dstColorBlendFactor_,
		vk::BlendOp colorBlendOp_,
		vk::BlendFactor srcAlphaBlendFactor_,
		vk::BlendFactor dstAlphaBlendFactor_,
		vk::BlendOp alphaBlendOp_,
		const vk::ColorComponentFlags& colorWriteMask_
	) : Parent(blendEnable_,
	           srcColorBlendFactor_,
	           dstColorBlendFactor_,
	           colorBlendOp_,
	           srcAlphaBlendFactor_,
	           dstAlphaBlendFactor_,
	           alphaBlendOp_,
	           colorWriteMask_)
	{
	}

	/*
	vk::PipelineColorBlendAttachmentState & PipelineColorBlendAttachment::returnColorBlendAttachment()
	{
		return colorBlendAttachmentState;
	}*/
}
