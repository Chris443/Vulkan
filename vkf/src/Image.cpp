#include "../Image.h"
#include "../Device.h"

namespace vkf {
	//Image Constructor for Swapchain
	Image::Image(std::shared_ptr<Device> const& device, VkImage const& image)
	:m_device(device),
	image(image),
	m_created(false){}

	//Image Constructor
	Image::Image(std::shared_ptr<Device> const& device, VkImageType imageType,VkFormat format,	VkExtent3D extent,	uint32_t mipLevels,	uint32_t arrayLayers,VkSampleCountFlagBits samples,
		VkImageTiling tiling,VkImageUsageFlags usage,VkSharingMode sharingMode,	std::vector<uint32_t> queueFamilyIndices,	VkImageLayout initialLayout,
		PhysicalDevice physicalDevice, VkImageCreateFlags flags)
		:m_device(device),
		m_queueFamilyIndices(queueFamilyIndices),
		m_created(true) {

		VkImageCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = flags;
		info.imageType = imageType;
		info.format = format;
		info.extent = extent;
		info.mipLevels = mipLevels;
		info.arrayLayers = arrayLayers;
		info.samples = samples;
		info.tiling = tiling;
		info.usage = usage;
		info.sharingMode = sharingMode;
		info.queueFamilyIndexCount = queueFamilyIndices.size();
		info.pQueueFamilyIndices = queueFamilyIndices.data();
		info.initialLayout = initialLayout;

		VkResult res =	vkCreateImage(device->getDevice(), &info, nullptr, &image);
		assert(!res);

		vkGetImageMemoryRequirements(m_device->getDevice(), image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;

		uint32_t idx;
		memProps=physicalDevice.getMemoryProperties();
		for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i) {
			if ((memRequirements.memoryTypeBits & (1 << i)) && (memProps.memoryTypes[i].propertyFlags & memPropFlag)) { // & memPropFlag
				idx = i;
			}
		}

		allocInfo.memoryTypeIndex = idx;

		if (vkAllocateMemory(m_device->getDevice(), &allocInfo, nullptr, &memory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(m_device->getDevice(), image, memory, 0);
	}
	
	Image::~Image() { 
		if (m_created) {
			vkDestroyImage(m_device->getDevice(), image, nullptr);
		}
	}

	std::shared_ptr<ImageView> Image::createImageView(VkImageViewType viewType, VkFormat format, VkComponentMapping components,
		VkImageSubresourceRange subresourceRange) {
		return std::make_shared<ImageView>(m_device,image, viewType,format,components,subresourceRange);
	}

	VkImageMemoryBarrier Image::getImageMemoryBarrier(VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask) {
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.pNext = nullptr;
		barrier.srcAccessMask = srcAccessMask;
		barrier.dstAccessMask = dstAccessMask;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel=0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		return barrier;
	}

	VkImageMemoryBarrier Image::getImageMemoryBarrier(VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageSubresourceRange subresourceRange) {
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.pNext = nullptr;
		barrier.srcAccessMask = srcAccessMask;
		barrier.dstAccessMask = dstAccessMask;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange = subresourceRange;

		return barrier;
	}

}


