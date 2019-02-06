#pragma once
#include "common.h"

namespace vkf {
	class Device;
	class Semaphore
		/** 
		* \brief Simple Wrapper Class for Vulkan Semaphores. Use the Device.createSemaphore() function inside the Device Class instead of the Constructor.
		*/
	{
	public:
		/**
		* \brief Creates a Vulkan Semaphore from given Device. Use the Device.creatSemaphore() function inside the Device Class instead of the Constructor.
		*/
		Semaphore(std::shared_ptr<Device> device);
		~Semaphore();
		/**
		* \brief Function to Return the Vulkan Semaphore Handle of this class
		* \return The Vulkan Semaphore Handle
		*/
		VkSemaphore getSemaphore() { return semaphore; };
	private:
		/**
		* \brief Corresponding Device to this Semaphore
		*/
		std::shared_ptr<Device> device;
		/**
		* \brief The Vulkan Semaphore Handle
		*/
		VkSemaphore semaphore;
	};

}
