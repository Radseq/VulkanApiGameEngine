#include "Model.hpp"

Model::Model (const GraphicCore::VulkanDevice& Device)
    : device (Device) { }

const GraphicCore::VulkanDevice& Model::GetDevice( ) const { return device; }