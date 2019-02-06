#pragma once
#include "common.h"
#include "CommandBuffer.h"

namespace vkf {
	class Device;
	class CommandPool
		/**
		* \brief The CommandPool class encapsulates a Vulkan CommandPool Handle and takes care of initialization and deleting. 
		* The CommandPool should be created by the device.createCommandPool() function inside the device class instead of manually creating one.
		* The CommandPool class should be used to create/allocate CommandBuffers, which are then bound to this specific CommandPool. The created CommandBuffers are stored internally as a vector of Shared pointers.
		*/
	{
	public:
		/**
		* \brief Constructor to create a CommandPool. Use the Device.createCommandPool() function instead.
		* \param device The corresponding Device to this CommandPool
		* \param queueFamilyIndex The Queue Family which should be used for this CommandPool
		* \param flags Flag which defines the usage of this Poll and its Buffers
		*/
		CommandPool(std::shared_ptr<Device> device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
		~CommandPool();
		/**
		* \brief Allocates a CommandBuffer and returns a shared pointer to it. The created CommandBuffers are also saved internally.
		* \param level Specifies the CommandBufferLevel of the created Buffer. Usable Flags are VK_COMMAND_BUFFER_LEVEL_PRIMARY and VK_COMMAND_BUFFER_LEVEL_SECONDARY.
		* \return Returns a Shared Pointer to a CommandBuffer.
		*/
		std::shared_ptr<CommandBuffer> allocateBuffer(VkCommandBufferLevel level);
		/**
		* \brief Resets the CommandPool and all CommandBuffers created/allocated by it.
		* \param flags Flag to specify how the CommandPool should be reset. Currently only VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT is supported.
		*/
		void reset(VkCommandPoolResetFlags flags);
		/**
		* \brief Returns the Vulkan CommandPool Handle
		* \return Corresponding CommandPool Handle to this class.
		*/
		VkCommandPool getCommandPool() { return cmdPool; }
		/**
		* \brief Returns all CommandBuffers used by this Pool
		  \return A Vector of Shared pointers of CommandBuffers created by this pool
		*/
		std::vector<std::shared_ptr<CommandBuffer>> getCommandBuffers() { return cmdBuffers; }
	private:
		/**
		* \brief Shared pointer to the corresponding Device.
		*/
		std::shared_ptr<Device> device;
		/**
		* \brief The Vulkan CommandPool Handle
		*/
		VkCommandPool cmdPool;
		/**
		* \brief All CommandBuffers created by this Pool
		*/
		std::vector<std::shared_ptr<CommandBuffer>> cmdBuffers;

	};

}

