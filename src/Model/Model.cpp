#include "Model.hpp"

Model::Model (const GameCore::VulkanDevice& Device)
    : device (Device) { }

const GameCore::VulkanDevice& Model::GetDevice( ) const { return device; }