#include "../Device.h"
namespace vkf {
	DeviceQueueCreateInfo::DeviceQueueCreateInfo(uint32_t queueFamilyIndex, std::vector<float> queuePriorities)
		:queueFamilyIndex(queueFamilyIndex),
		queuePriorities(queuePriorities) {}

	Device::Device(VkInstance const& instance, std::vector<DeviceQueueCreateInfo> deviceQueueCreateInfos, PhysicalDevice physicalDevice,
		std::vector<std::string> deviceExtensionNames, std::vector<std::string> layerNames, VkPhysicalDeviceFeatures const& enabledFeatures) {

		for (auto d : deviceExtensionNames) {
			this->deviceExtensionNames.push_back(d);
		}
		this->layerNames = layerNames;
		this->instance = std::make_shared<VkInstance>(instance);
		this->physicalDevice = std::make_shared<PhysicalDevice>(physicalDevice);

		//initialize DeviceQueueCreateInfos
		for (uint32_t i = 0; i < deviceQueueCreateInfos.size(); ++i) {
			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.pNext = nullptr;
			queueInfo.flags = 0;
			queueInfo.queueFamilyIndex = deviceQueueCreateInfos[i].queueFamilyIndex;
			queueInfo.queueCount = deviceQueueCreateInfos[i].queuePriorities.size(); //TODO: check if QueueFamily Supports this amount of queues
			queueInfo.pQueuePriorities = deviceQueueCreateInfos[i].queuePriorities.data();
			m_deviceQueueCreateInfos.push_back(queueInfo);
		}

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pNext = NULL;
		deviceInfo.flags = 0;
		deviceInfo.queueCreateInfoCount = m_deviceQueueCreateInfos.size();
		deviceInfo.pQueueCreateInfos = m_deviceQueueCreateInfos.data();
		deviceInfo.enabledLayerCount = this->layerNames.size();

		std::vector<char const*> l;
		for (auto const&s : layerNames) {
			l.push_back(s.c_str());
		}

		deviceInfo.ppEnabledLayerNames = l.data();
		deviceInfo.enabledExtensionCount = deviceExtensionNames.size();

		std::vector<char const*> e;
		for (auto const&s : deviceExtensionNames) {
			e.push_back(s.c_str());
		}

		deviceInfo.ppEnabledExtensionNames = e.data();
		deviceInfo.pEnabledFeatures = &enabledFeatures;

		VkResult suc = vkCreateDevice(this->physicalDevice->getPhysicalDevice(), &deviceInfo, NULL, &device);
		assert(!suc);

		/*create all Queues and store them  //currently use createQueue
		for (auto const& createInfo : deviceQueueCreateInfos)
		{
			for (uint32_t queueIndex = 0; queueIndex < createInfo.queuePriorities.size(); ++queueIndex)
			{
				std::shared_ptr<Queue> queue = createQueue(createInfo.queueFamilyIndex, queueIndex);
				queues.push_back(queue);
			}

			//	check for duplicate queue family index

		}*/
	};


	std::shared_ptr<Device> Device::create(VkInstance const& instance, std::vector<DeviceQueueCreateInfo> deviceQueueCreateInfos, PhysicalDevice physicalDevice,
		std::vector<std::string> deviceExtensionNames, std::vector<std::string> layerNames, VkPhysicalDeviceFeatures const& enabledFeatures)
	{
		return std::make_shared<Device>(instance, deviceQueueCreateInfos, physicalDevice, deviceExtensionNames, layerNames, enabledFeatures);
	}

	std::shared_ptr<Queue> Device::createQueue(uint32_t familyIndex, uint32_t queueIndex) {
		VkQueue queue;
		vkGetDeviceQueue(device, familyIndex, queueIndex, &queue);
		return std::make_shared<Queue>(queue, familyIndex);
	}

	std::shared_ptr<Swapchain> Device::createSwapchain(VkSurfaceKHR const& surface, uint32_t minImageCount, VkFormat imageFormat, VkColorSpaceKHR colorSpace, VkExtent2D imageExtent,
		uint32_t imageArrayLayers, VkImageUsageFlags imageUsage, VkSharingMode imageSharingMode, std::vector<uint32_t> queueFamilyIndices,
		VkSurfaceTransformFlagBitsKHR preTransform, VkCompositeAlphaFlagBitsKHR compositeAlpha, VkPresentModeKHR presentMode, VkBool32 clipped,
		std::shared_ptr<Swapchain> oldSwapchain) {

		return std::make_shared<Swapchain>(shared_from_this(),surface, minImageCount, imageFormat, colorSpace, imageExtent,imageArrayLayers, imageUsage, imageSharingMode,
			queueFamilyIndices, preTransform, compositeAlpha, presentMode, clipped, oldSwapchain);
	}
	
	/*std::shared_ptr<Image> Device::createImage(VkImage image) {
		return std::make_shared<Image>(shared_from_this(),image);
	}*/

	std::shared_ptr<Image> Device::createImage(VkImageType imageType, VkFormat format, VkExtent3D extent, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples,
		VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndices, VkImageLayout initialLayout,
		PhysicalDevice physicalDevice,VkImageCreateFlags flags) {
		return std::make_shared<Image>(shared_from_this(), imageType, format, extent, mipLevels, arrayLayers, samples, tiling,  usage,  sharingMode,  queueFamilyIndices,  initialLayout, 
			 physicalDevice, flags);
	}

	std::shared_ptr<ShaderModule> Device::createShaderModule(std::string shaderFilePath) {
		return std::make_shared<ShaderModule>(shared_from_this(), shaderFilePath);
	}

	std::shared_ptr<PipelineLayout> Device::createPipelineLayout(std::vector<std::shared_ptr<DeskriptorSetLayout>> setLayouts, std::vector<VkPushConstantRange> pushConstantRanges) {
		return std::make_shared<PipelineLayout>(shared_from_this(),setLayouts,pushConstantRanges);
	}

	std::shared_ptr<Pipeline> Device::createPipeline(
		std::shared_ptr<PipelineLayout> pipelineLayout,
		std::shared_ptr<RenderPass> renderpass,
		uint32_t subpass,
		std::vector<PipelineShaderStageCreateInfo> shaderstages,
		PipelineVertexInputStateCreateInfo* vertexInputStateCreateInfo,
		PipelineInputAssemblyStateCreateInfo* inputAssemblyStateCreateInfo,
		PipelineRasterizationStateCreateInfo* rasterizationStateCreateInfo,
		PipelineViewportStateCreateInfo* viewportStateCreateInfo,
		PipelineMultisampleStateCreateInfo* multisampleinfo,
		PipelineColorBlendStateCreateInfo* colorBlendStateCreateInfo,
		PipelineDepthStencilStateCreateInfo* depthStencilCreateInfo,
		VkPipelineTessellationStateCreateInfo* tessellationStateCreateInfo,
		VkPipelineDynamicStateCreateInfo* dynamicStateCreateInfo,
		VkPipeline basePipelineHandle,
		uint32_t basePipelineIndex,
		VkPipelineCache pipelineCache) {
		return std::make_shared<Pipeline>(shared_from_this(), pipelineLayout, renderpass, subpass,
		shaderstages, vertexInputStateCreateInfo, inputAssemblyStateCreateInfo,	 rasterizationStateCreateInfo,viewportStateCreateInfo,	 multisampleinfo,	colorBlendStateCreateInfo,
			depthStencilCreateInfo,tessellationStateCreateInfo, dynamicStateCreateInfo, basePipelineHandle, basePipelineIndex, pipelineCache);
	}

	std::shared_ptr<RenderPass> Device::createRenderpass(std::vector<VkAttachmentDescription> attachmentDescriptions,
		std::vector<VkSubpassDescription> subPassDescription, std::vector<VkSubpassDependency> subpassDependencies) {
		return std::make_shared<RenderPass>(shared_from_this(), attachmentDescriptions, subPassDescription, subpassDependencies);
	}

	std::shared_ptr<Framebuffer> Device::createFramebuffer(std::shared_ptr<RenderPass> renderpass,
		std::vector<std::shared_ptr<ImageView>> attachments, VkExtent2D const& extents, uint32_t layers) {
		return std::make_shared<Framebuffer>(shared_from_this(), renderpass, attachments, extents, layers);
	}

	std::shared_ptr<CommandPool> Device::createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags) {
		return std::make_shared<CommandPool>(shared_from_this(), queueFamilyIndex, flags);
	}

	std::shared_ptr<Buffer> Device::createBuffer(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, VkDeviceSize size, VkBufferUsageFlags flags, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndices, VkMemoryPropertyFlags memoryPropertyflags) {
		return std::make_shared<Buffer>(shared_from_this(), physicalDeviceMemoryProperties, size, flags, sharingMode, queueFamilyIndices, memoryPropertyflags);
	}
	std::shared_ptr<DeskriptorSetLayout> Device::createDeskriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings) {
		return std::make_shared<DeskriptorSetLayout>(shared_from_this(), bindings);
	}

	std::shared_ptr<DeskriptorPool> Device::createDeskriptorPool(uint32_t sets, std::vector<VkDescriptorPoolSize> poolSizes, VkDescriptorPoolCreateFlags flag) {
		return std::make_shared<DeskriptorPool>(shared_from_this(), sets, poolSizes, flag);
	}
	std::shared_ptr<DeskriptorSet> Device::createDeskriptorSet(std::shared_ptr<DeskriptorPool> pool, std::shared_ptr<DeskriptorSetLayout> layout) {
		return std::make_shared<DeskriptorSet>(shared_from_this(), pool, layout);
	}

	std::shared_ptr<Semaphore> Device::createSemaphore() {
		return std::make_shared<Semaphore>(shared_from_this());
	}

	std::shared_ptr<Sampler> Device::createSampler(VkFilter magFilter, VkFilter minFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressModeU,
		VkSamplerAddressMode addressModeV, VkSamplerAddressMode addressModeW, float mipLodBias, bool anisotropyEnable, float maxAnisotropy, bool compareEnable,
		VkCompareOp compareOp, float minLod, float maxLod, VkBorderColor borderColor, bool unnormalizedCoordinates) {
		return std::make_shared<Sampler>(shared_from_this(), magFilter, minFilter, mipmapMode, addressModeU, addressModeV, addressModeW, mipLodBias, anisotropyEnable, maxAnisotropy, compareEnable,
			compareOp, minLod, maxLod, borderColor, unnormalizedCoordinates);
	}

	std::shared_ptr<Texture> Device::create2DTexture(std::string textureFilePath, std::shared_ptr<Queue> queue) {
		return std::make_shared<Texture>(shared_from_this(), textureFilePath, queue,TextureType::D2);
	}
	std::shared_ptr<Texture> Device::createCubemapTexture(std::string textureFilePath, std::shared_ptr<Queue> queue) {
		return std::make_shared<Texture>(shared_from_this(), textureFilePath, queue, TextureType::CUBEMAP);
	}
}