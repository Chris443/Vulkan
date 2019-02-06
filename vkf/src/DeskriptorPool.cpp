#include "../DeskriptorPool.h"
#include "../Device.h"

namespace vkf {
	DeskriptorPool::DeskriptorPool(std::shared_ptr<Device> device,
		uint32_t sets,std::vector<VkDescriptorPoolSize> poolSizes, VkDescriptorPoolCreateFlags flag)
	:device(device){
		
		VkDescriptorPoolCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = flag;
		info.maxSets  = sets;
		info.poolSizeCount = poolSizes.size();
		info.pPoolSizes = (poolSizes.size() > 0) ? poolSizes.data() : nullptr;

		VkResult res = vkCreateDescriptorPool(device->getDevice(), &info, nullptr, &pool);
		assert(!res);
	}

	DeskriptorPool::~DeskriptorPool() {
		vkDestroyDescriptorPool(device->getDevice(), pool, nullptr);
	}
}

