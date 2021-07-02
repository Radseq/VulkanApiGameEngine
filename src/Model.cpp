#include "pch.h"
#include "Model.hpp"

Model::Model (const GraphicCore::VulkanDevice& Device, const GraphicCore::VertexLayout& VertexLayout)
    : device (Device)
    , m_VertexLayout (VertexLayout)
{
}

const GraphicCore::VulkanDevice& Model::GetDevice( ) const { return device; }