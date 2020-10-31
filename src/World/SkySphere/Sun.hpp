#pragma once

#include "Pipeline/Pipeline.hpp"
#include "Textures/Texture2D.hpp"
#include "../Util/utils.hpp"
#include "../Camera/Camera.hpp"
#include "Buffer/CoreBuffer.hpp"

class Sun
{
	struct Vertex {
		float pos[3];
		float uv[2];
		float normal[3];
	};

	GameCore::Texture2D texture;

	struct {
		uint32_t count;
		GameCore::CoreBuffer indices;
		GameCore::CoreBuffer vertices;
	} geometry;

	GameCore::CoreBuffer uniformDataVS;

	struct UboVS {
		glm::mat4 projection;
		glm::mat4 model;
		glm::vec4 viewPos;
		float lodBias = 0.0f;
	} uboVS;

	struct {
		vk::Pipeline solid;
	} pipelines;

	vk::PipelineLayout pipelineLayout;
	vk::DescriptorSet descriptorSet;
	vk::DescriptorSetLayout descriptorSetLayout;

	GameCore::VulkanDevice context;

	const vk::Device& device{ context.getDevice() };

	vk::DescriptorPool descriptorPool;

	const Camera & camera;

	const vk::Extent2D & windowSize;

public:
	Sun(const GameCore::VulkanDevice & Context, const Camera & Camera, const vk::Extent2D & WindowSize);
	~Sun();
	void setImageLayout(vk::CommandBuffer cmdBuffer, vk::Image image, vk::ImageAspectFlags aspectMask, vk::ImageLayout oldImageLayout, vk::ImageLayout newImageLayout, uint32_t mipLevel, uint32_t mipLevelCount);
	void updateDrawCommandBuffer(const vk::CommandBuffer& cmdBuffer);
	void generateQuad();
	void setupDescriptorPool();
	void loadAssets();
	void setupDescriptorSetLayout();
	void preparePipelines(vk::RenderPass & renderPass);
	void prepareUniformBuffers();
	void updateUniformBuffers();
	void viewChanged();
	void changeLodBias(float delta);
	void setupDescriptorSets(std::vector<vk::WriteDescriptorSet> & writeDescriptorSets);
};
