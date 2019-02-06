#pragma once
#include "common.h"

/**
* \brief Wrapper Class for a Vulkan Fence. 
*/
namespace vkf {
	class Device;
	class Fence
	{
	public:
		/**
		* \brief Creates and initializes a Fence by given Device.
		*/
		Fence(std::shared_ptr<Device> device);
		~Fence();
		VkFence getFence() { return fence; };
	private:
		std::shared_ptr<Device> device;
		VkFence fence;
	};
}

