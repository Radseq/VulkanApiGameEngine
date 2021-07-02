#ifndef ISHADER_DESC_SET_HPP
#define ISHADER_DESC_SET_HPP

#include <Descriptor/DescriptorPool.hpp>
#include <glm/glm.hpp>
#include <string_view>
#include <vector>

#include "../Base/Core.hpp"
#include "../ShaderInterfaces/IShader.hpp"
#include "Descriptor/DescriptorSetLayout.hpp"
#include "Descriptor/DescriptorSets.hpp"

class IShaderDescSet
{
   public:
    virtual void CreateDescriptorSets (const uint32_t& count) = 0;

    // Can be in separate interface,
    // but desc set must be for specyfied layout and there will be
    // not same layout for diffrent set, at last for now i dont see.
    virtual void CreateDescriptorSetLayout( ) = 0;

    virtual void WriteDescSet( ) = 0;

    virtual void UpdateWriteDescriptors( ) = 0;

    virtual const GraphicCore::DescriptorSetLayoutBinding& GetDescSetLayout( ) const = 0;

    virtual const VulkanGame::Ref<GraphicCore::DescriptorSets> GetDescSets( ) const = 0;

    virtual std::vector<VulkanGame::Ref<IShader>> GetShaders( ) = 0;

    virtual void Destroy( ) = 0;
};

#endif  // ISHADER_DESC_SET_HPP
