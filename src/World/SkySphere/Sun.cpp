#include "Sun.hpp"

Sun::Sun(const GraphicCore::VulkanDevice & Context, const Camera & Camera, const vk::Extent2D & WindowSize) : context(Context), camera(Camera), windowSize(WindowSize)
{
}

Sun::~Sun()
{
}

// Create an image memory barrier for changing the layout of
// an image and put it into an active command buffer
void Sun::setImageLayout(vk::CommandBuffer cmdBuffer, vk::Image image, vk::ImageAspectFlags aspectMask, vk::ImageLayout oldImageLayout, vk::ImageLayout newImageLayout, uint32_t mipLevel, uint32_t mipLevelCount) {
	// Create an image barrier object
	vk::ImageMemoryBarrier imageMemoryBarrier;
	imageMemoryBarrier.oldLayout = oldImageLayout;
	imageMemoryBarrier.newLayout = newImageLayout;
	imageMemoryBarrier.image = image;
	imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
	imageMemoryBarrier.subresourceRange.baseMipLevel = mipLevel;
	imageMemoryBarrier.subresourceRange.levelCount = mipLevelCount;
	imageMemoryBarrier.subresourceRange.layerCount = 1;

	// Only sets masks for layouts used in this example
	// For a more complete version that can be used with
	// other layouts see vkx::setImageLayout

	// Source layouts (new)

	if (oldImageLayout == vk::ImageLayout::ePreinitialized) {
		imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
	}
	else if (oldImageLayout == vk::ImageLayout::eTransferDstOptimal) {
		imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
	}
	else if (oldImageLayout == vk::ImageLayout::eTransferSrcOptimal) {
		imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
	}
	else if (oldImageLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
		imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
	}

	// Target layouts (new)

	if (newImageLayout == vk::ImageLayout::eTransferDstOptimal) {
		// New layout is transfer destination (copy, blit)
		// Make sure any reads from and writes to the image have been finished
		imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
	}
	else if (newImageLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
		// New layout is shader read (sampler, input attachment)
		// Make sure any writes to the image have been finished
		imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	}
	else if (newImageLayout == vk::ImageLayout::eTransferSrcOptimal) {
		// New layout is transfer source (copy, blit)
		// Make sure any reads from and writes to the image have been finished
		imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
	}

	// Put barrier on top
	vk::PipelineStageFlags srcStageFlags = vk::PipelineStageFlagBits::eTopOfPipe;
	vk::PipelineStageFlags destStageFlags = vk::PipelineStageFlagBits::eTopOfPipe;

	// Put barrier inside setup command buffer
	cmdBuffer.pipelineBarrier(srcStageFlags, destStageFlags, vk::DependencyFlags(), nullptr, nullptr, imageMemoryBarrier);
}

void Sun::updateDrawCommandBuffer(const vk::CommandBuffer& cmdBuffer) {
	cmdBuffer.setViewport(0, GraphicCore::util::viewport(windowSize));
	cmdBuffer.setScissor(0, GraphicCore::util::rect2D(windowSize));
	cmdBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSet, nullptr);
	cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines.solid);
	vk::DeviceSize offsets = 0;
	cmdBuffer.bindVertexBuffers(0, geometry.vertices.getBuffer(), offsets);
	cmdBuffer.bindIndexBuffer(geometry.indices.getBuffer(), 0, vk::IndexType::eUint32);

	cmdBuffer.drawIndexed(geometry.count, 1, 0, 0, 0);
}

void Sun::generateQuad() {

	struct Vertex {
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 normal;
	};

	// Setup vertices for a single uv-mapped quad
#define DIM 1.0f
#define NORMAL { 0.0f, 0.0f, 1.0f }
	std::vector<Vertex> vertexBuffer =
	{
		{ {  DIM,  DIM, 0.0f }, { 1.0f, 1.0f }, NORMAL },
		{ { -DIM,  DIM, 0.0f }, { 0.0f, 1.0f }, NORMAL },
		{ { -DIM, -DIM, 0.0f }, { 0.0f, 0.0f }, NORMAL },
		{ {  DIM, -DIM, 0.0f }, { 1.0f, 0.0f }, NORMAL }
	};
#undef DIM
#undef NORMAL
	geometry.vertices = context.stageToDeviceBuffer(vk::BufferUsageFlagBits::eVertexBuffer, vertexBuffer);

	// Setup indices
	std::vector<uint32_t> indexBuffer = { 0,1,2, 2,3,0 };
	geometry.indices = context.stageToDeviceBuffer(vk::BufferUsageFlagBits::eIndexBuffer, indexBuffer);
	geometry.count = (uint32_t)indexBuffer.size();
}

void Sun::setupDescriptorPool() {
	// Example uses one ubo and one image sampler
	std::vector<vk::DescriptorPoolSize> poolSizes =
	{
		vk::DescriptorPoolSize{ vk::DescriptorType::eUniformBuffer, 1 },
		vk::DescriptorPoolSize{ vk::DescriptorType::eCombinedImageSampler, 1 },
	};
	descriptorPool = device.createDescriptorPool({ {}, 2, (uint32_t)poolSizes.size(), poolSizes.data() });
}

void Sun::loadAssets()
{
	texture.loadFromFile(context, getAssetPath() + "textures/metalplate01_rgba.ktx", vk::Format::eR8G8B8A8Unorm);
}

void Sun::setupDescriptorSetLayout() {
	std::vector<vk::DescriptorSetLayoutBinding> setLayoutBindings{
		// Binding 0 : Vertex shader uniform buffer
		vk::DescriptorSetLayoutBinding{ 0,  vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex },
		// Binding 1 : Fragment shader image sampler
		vk::DescriptorSetLayoutBinding{ 1,  vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment },
	};

	descriptorSetLayout = device.createDescriptorSetLayout({ {}, (uint32_t)setLayoutBindings.size(), setLayoutBindings.data() });
	pipelineLayout = device.createPipelineLayout({ {}, 1, &descriptorSetLayout });
}

void Sun::setupDescriptorSets(std::vector<vk::WriteDescriptorSet> & writeDescriptorSets) {
	descriptorSet = device.allocateDescriptorSets({ descriptorPool, 1, &descriptorSetLayout })[0];

	vk::DescriptorImageInfo texDescriptor{ texture.getVkSampler(), texture.getVkImageView(), vk::ImageLayout::eGeneral };

	vk::WriteDescriptorSet a{ descriptorSet,			0, 0, 1, vk::DescriptorType::eUniformBuffer,		nullptr, &uniformDataVS.getDescriptorInfo() };
	vk::WriteDescriptorSet b{ descriptorSet,			1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &texDescriptor };
	writeDescriptorSets.push_back(a);
	writeDescriptorSets.push_back(b);
}

void Sun::preparePipelines(vk::RenderPass & renderPass) {
	GraphicCore::Pipeline pipelineBuilder{ device, pipelineLayout, renderPass };
	pipelineBuilder.getPipelineRasterization().getRasterizationState().cullMode = vk::CullModeFlagBits::eNone;

	pipelineBuilder.GetVertexInputState().getBindingDescriptions()= {
	{ 0, sizeof(Vertex), vk::VertexInputRate::eVertex }
	};

	pipelineBuilder.GetVertexInputState().getAttributeDescriptions() = {
		{ 0, 0, vk::Format::eR32G32B32Sfloat, 0 },
		{ 1, 0, vk::Format::eR32G32Sfloat, sizeof(float) * 3 },
		{ 2, 0, vk::Format::eR32G32B32Sfloat, sizeof(float) * 5 },
	};
	pipelineBuilder.loadShader(getAssetPath() + "shaders/texture/texture.vert.spv", vk::ShaderStageFlagBits::eVertex);
	pipelineBuilder.loadShader(getAssetPath() + "shaders/texture/texture.frag.spv", vk::ShaderStageFlagBits::eFragment);
	pipelines.solid = pipelineBuilder.create(context.getPipelineCache());
}

// Prepare and initialize uniform buffer containing shader uniforms
void Sun::prepareUniformBuffers() {
	// Vertex shader uniform buffer block
	uniformDataVS = context.createUniformBuffer(uboVS);
	updateUniformBuffers();
}

void Sun::updateUniformBuffers() {
	// Vertex shader
	uboVS.projection = camera.matrices.perspective;
	glm::mat4 viewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, camera.position.z));
	uboVS.model = viewMatrix * glm::translate(glm::mat4(), glm::vec3(camera.position.x, camera.position.y, 0));
	uboVS.model = uboVS.model * glm::inverse(camera.matrices.skyboxView);
	uboVS.viewPos = glm::vec4(0.0f, 0.0f, -camera.position.z, 0.0f);
	uniformDataVS.copy(uboVS);
}

void Sun::viewChanged() {
	updateUniformBuffers();
}

void Sun::changeLodBias(float delta) {
	uboVS.lodBias += delta;
	if (uboVS.lodBias < 0.0f) {
		uboVS.lodBias = 0.0f;
	}
	if (uboVS.lodBias > 8.0f) {
		uboVS.lodBias = 8.0f;
	}
	updateUniformBuffers();
}