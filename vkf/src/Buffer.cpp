#include "../Buffer.h"
#include "../Device.h"

namespace vkf {
	Buffer::Buffer(std::shared_ptr<Device> device, VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndices,VkMemoryPropertyFlags memoryPropertyflags)
		:device(device),
		size(size)
	{
		VkBufferCreateInfo bufferCreateInfo;
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.pNext = nullptr;
		bufferCreateInfo.flags = 0;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usageFlags;
		bufferCreateInfo.sharingMode = sharingMode;
		bufferCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
		bufferCreateInfo.pQueueFamilyIndices = (queueFamilyIndices.size() > 0) ? queueFamilyIndices.data() : nullptr;

		vkCreateBuffer(device->getDevice(), &bufferCreateInfo, nullptr, &buffer);

		VkMemoryRequirements memReq;
		vkGetBufferMemoryRequirements(device->getDevice(), buffer, &memReq);
		
		VkMemoryAllocateInfo memInfo;
		memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memInfo.pNext = nullptr;
		memInfo.allocationSize = memReq.size;

		uint32_t idx;

		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; ++i) {
			if ((memReq.memoryTypeBits & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryPropertyflags)) {
				idx = i;
			}
		}
		memInfo.memoryTypeIndex = idx;

		vkAllocateMemory(device->getDevice(), &memInfo, nullptr, &deviceMemory);
		vkBindBufferMemory(device->getDevice(), buffer, deviceMemory, 0);
	}

	void Buffer::map(void*data) {
		void* rawData;
		
		vkMapMemory(device->getDevice(), deviceMemory, 0, size, 0, &rawData);

		memcpy(rawData, data, (size_t)size);

		vkUnmapMemory(device->getDevice(), deviceMemory);
	}

	void Buffer::mapOffset(void*data, VkDeviceSize offset) {
		void* rawData;

		vkMapMemory(device->getDevice(), deviceMemory, offset, size, 0,&rawData);

		memcpy(rawData, data, (size_t)size);

		vkUnmapMemory(device->getDevice(), deviceMemory);
	}


	Buffer::~Buffer()
	{
		vkDestroyBuffer(device->getDevice(), buffer, nullptr);
	}

}
