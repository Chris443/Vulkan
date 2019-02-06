#pragma once
#include "common.h"

namespace vkf {
	class Device;
	/**
	* \brief Simple Wrapper Class for Vulkan DescriptorPools. Takes care of Initializating and deleting the DescriptorPool Handle. 
	* Use the device.createdescriptorPool() function inside the Device class to create a descriptorPool.
	*/
	class DeskriptorPool {
	public:
		/**
		* \brief Constructor to create a Vulkan DescriptorPool. Use device.createDesckriptorPool() instead.
		* \param device The corresponding Device to this DescriptorPool
		* \param sets The maximum amount of DescriptorSets which can be created by this CommandPool
		* \param poolSizes An Array of the VkDescriptorPoolSize Structure, which contain a Description and the amount of Descriptors which should be allocated by that pool
		*/
		DeskriptorPool(std::shared_ptr<Device> device,
			uint32_t sets, std::vector<VkDescriptorPoolSize> poolSizes, VkDescriptorPoolCreateFlags flags = 0);
		~DeskriptorPool();
		/**
		* \brief Returns the Vulkan DescriptorPool Handle
		* \return The Vulkan DescriptorPool Handle
		*/
		VkDescriptorPool getPool() { return pool; };
	private:
		/**
		* \brief Shared Pointer to corresponding Device
		*/
		std::shared_ptr<Device> device;
		/**
		* \brief The Vulkan DescriptorPool Handle
		*/
		VkDescriptorPool pool;
	};
}