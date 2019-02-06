#include "../CommandPool.h"
#include "../Device.h"
namespace vkf {

	CommandPool::CommandPool(std::shared_ptr<Device> device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
		:device(device)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.pNext = NULL;
		poolInfo.flags = flags;
		poolInfo.queueFamilyIndex = queueFamilyIndex;

		VkResult res;
		res=vkCreateCommandPool(device->getDevice(), &poolInfo, NULL, &cmdPool);
		assert(!res);
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(device->getDevice(), cmdPool, NULL);
	}

	std::shared_ptr<CommandBuffer> CommandPool::allocateBuffer(VkCommandBufferLevel level) {

		std::shared_ptr<CommandBuffer> cmdBuffer = std::make_shared<CommandBuffer>(device->getDevice(),this->cmdPool, level);
		
		cmdBuffers.push_back(cmdBuffer);
		return cmdBuffer;
	}

	void CommandPool::reset(VkCommandPoolResetFlags flags) {
		VkResult result = vkResetCommandPool(device->getDevice(),cmdPool,flags);
		assert(!result);
	}

}