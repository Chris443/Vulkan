#pragma once
#include "common.h"

namespace vkf {
	class Device;
	class Buffer
		/**
		* \brief The Buffer class encapsulates the VkBuffer Handle and takes care of initialization and deleting. For initialization, use the Device.createBuffer() function inside the Device class.
		* The Buffer is mainly used to map data to the device. This class decides internaly the best way to store given Data by comparing it with the given MemoryProperties.
		*/
	{
	public:
		/**
		* \brief Constructor for creating a Vulkan buffer. Use the Device.createBuffer() function instead of the constructor.
		* \param device The logical device to which this Buffer belongs.
		* \param physicalDeviceMemoryProperties The Memory properties of the Device.
		* \param size The size of the Buffer in Bytes
		* \param flags Defines how this Buffer should be used, i.e. VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, possible flags are listed in the Vulkan API
		* \param sharingMode Defines how this buffer can be accessed from queues. Possible flags are VK_SHARING_MODE_EXCLUSIVE and VK_SHARING_MODE_CONCURRENT
		* \param queueFamilyIndices All Queue Families which should have access to this Buffer
		* \param memoryPropertyflags The visibility of this memorychunk
		*/
		Buffer(std::shared_ptr<Device> device, VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, VkDeviceSize size, VkBufferUsageFlags flags,
			VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndices, VkMemoryPropertyFlags memoryPropertyflags);
		~Buffer();

		/**
		* \brief Maps given Data to the DeviceMemory of given Device with specified size.
		* \param data The data which should be mapped
		*/
		void map(void* data);
		/**
		* \brief Maps given Data to the DeviceMemory of given Device with specified size.
		* \param data The data which should be mapped
		* \param offset The offset between different data sets
		*/
		void mapOffset(void*data, VkDeviceSize offset);
		/**
		* \brief Returns the Vulkan Buffer Handle
		* \return The Vulkan Buffer Handle
		*/
		VkBuffer getBuffer() { return buffer; };
		/**
		* \brief Returns the device Memory of this Buffer
		*/
		VkDeviceMemory getMemory() { return deviceMemory; };
	private:
		/**
		* \brief Shared pointer to the corresponding Device. Used for Deconstructor.
		*/
		std::shared_ptr<Device> device;
		/**
		* \brief The Buffer Handle
		*/
		VkBuffer buffer;
		/**
		* \brief The Size of the Buffer
		*/
		VkDeviceSize size;
		/**
		* \brief The DeviceMemory which is used for mapping data
		*/
		VkDeviceMemory deviceMemory;
	};

}
