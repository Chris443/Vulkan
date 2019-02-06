#include "../Semaphore.h"
#include "../Device.h"

namespace vkf {

	Semaphore::Semaphore(std::shared_ptr<Device> device)
		:device(device)
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo;
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.pNext = nullptr;
		semaphoreCreateInfo.flags = 0;

		vkCreateSemaphore(device->getDevice(), &semaphoreCreateInfo, nullptr, &semaphore);
	}


	Semaphore::~Semaphore()
	{
		vkDestroySemaphore(device->getDevice(), semaphore, nullptr);
	}

}