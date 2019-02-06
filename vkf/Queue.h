#pragma once
#include "common.h"

namespace vkf {
	class Semaphore;
	class CommandBuffer;
	class Swapchain;
	/**
	* \brief Simple wrapper for the Vulkan SubmitInfo. Used to hide the Vulkan specific structures and to use the C++ classes.
	*/
	struct SubmitInfo {
		/**
		* \brief Simple wrapper for the Vulkan SubmitInfo. 
		* \param waitSemaphores The semaphores on which the CommandBuffers should wait
		* \param cmdBuffers The commandBuffers which should be submitted
		* \param signalSemaphores The Semaphores which should be signaled after the CommandBuffers finished
		* \param pipelineStageFlagBits The PipelineStages where the waiting should occure
		* \see Semaphore CommandBuffer
		*/
		SubmitInfo(std::vector<std::shared_ptr<Semaphore>> const& waitSemaphores,
			std::vector<std::shared_ptr<CommandBuffer>> const&  cmdBuffers,
			std::vector<std::shared_ptr<Semaphore>> const& signalSemaphores,
			std::vector<VkPipelineStageFlags> pipelineStageFlagBits);

		std::vector<std::shared_ptr<Semaphore>> m_waitSemaphores;
		std::vector<std::shared_ptr<CommandBuffer>> m_cmdBuffers;
		std::vector<std::shared_ptr<Semaphore>> m_signalSemaphores;
		std::vector<VkPipelineStageFlags> m_pipelineStageFlagBits;
	};

	/**
	* \brief The Queue class is used by the Device class to encapsulate the Handle of a Queue. Use the device.createQueue() function inside the Device class for Queuecreation.
	*/
	class Queue
	{
	public:
		/**
		* \brief Only used internally by the device class to encapsulate the Handle inside this class. This way, the Queue Handle is associated with the function calls of this class.
		* \note The Device class passes internally a Queue Handle into this class. Don't use this class to create a Queue, instead take a look the Device class.
		* \see Device
		*/
		Queue(VkQueue queue,uint32_t familyIndex);
		/**
		* \brief Submits the given SubmitInfos(i.e. The CommandBuffers) to the Queue
		* \param submitInfos The SubmitInfos which should be submitted to the Queue
		* \param fence The fence which should be used for synchronization. Default value is VK_NULL_HANDLE
		* \see SubmitInfo CommandBuffer
		*/
		void submit(std::vector<SubmitInfo> const& submitInfos, VkFence fence = VK_NULL_HANDLE);
		/**
		* \brief Function to start Presenting. Only Queues which support Presentation should call this function.
		* \param waitSemaphores The Semaphores which should be waited for before presentation
		* \param swapchains The swapchains which should be used for presentation(usually it's only one swapchain)
		* \param imageIndex The Index of the image which should be presented
		* \see Swapchain Semaphore
		*/
		VkResult present(std::vector<std::shared_ptr<Semaphore>> waitSemaphores, std::vector<std::shared_ptr<Swapchain>>
			swapchains, uint32_t imageIndex);
		/**
		* \brief Calls the Vulkan specific wait function, so the Queue waits until all operations are finished and it reached the idle status.
		*/
		void wait();
		/**
		* \brief Getter Function to retrieve the Queue familyIndex
		* \return The family Index to this Queue
		*/
		uint32_t getFamilyIndex() { return familyIndex; };
		/**
		* \brief Getter Function to retrieve the Queue Handle
		* \return The Vulkan Queue Handle
		*/
		VkQueue getQueue() { return queue; };
	private:
		VkQueue queue;
		uint32_t familyIndex;
	};
}
