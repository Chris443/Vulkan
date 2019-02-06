#include "../Fence.h"
#include "../Device.h"

namespace vkf {

	Fence::Fence(std::shared_ptr<Device> device)
		:device(device)
	{
		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = 0;

		vkCreateFence(device->getDevice(), &fenceInfo, NULL, &fence);
	}


	Fence::~Fence()
	{
		vkDestroyFence(device->getDevice(), fence, NULL);
	}

}