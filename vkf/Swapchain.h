#pragma once
#include "common.h"
#include "Image.h"
#include "Fence.h"
#include "Semaphore.h"

class Device;

namespace vkf {
	/**
	* \brief Datastructure for acquiring the next Image inside the Swapchain.
	* \see Swapchain
	*/
	struct swapChainImageData {
		/**
		* \brief The Index of the SwapchainImage
		*/
		uint32_t imageIndex;
		/**
		* \brief The result after retrieving the SwapchainImage
		*/
		VkResult result;
	};

	class Device;
	/**
	* \brief The Swapchain class encapsulates the Swapchain Handle and takes care of initialization and deleting. For initialization, use the Device.creatSwapchain() function inside the Device class.
	* To optimize image swapping, every image has one corresponding Semaphore, which is used for synchronization. Inside the acquireNextImage() function, the used Semaphore is replaced with a free Semaphore.
	*/
	class Swapchain
	{
	public:
		/**
		* \brief Creates a Swapchain
		* \param device The device which creates the Swapchain
		* \param surface The Surface which should be used by the Swapchain for presentation
		* \param minImageCount The minimum amount of Images which should be used. i.e. 2 for DoubleBuffering
		* \param imageFormat The format which should be used
		* \param colorSpace The colorSpace which should be used to interpret the images
		* \param imageExtent The size in each Dimension of the Swapchain in pixels
		* \param imageArrayLayers The image Array layers. Used for stereoscopic drawing, i.e. 3D images for VR
		* \param imageUsage Describes the usage of the Swapchain
		* \param imageSharingMode The sharing mode used for the swapchain
		* \param queueFamilyIndices Defines which QueueFamily have access to the swapchain
		* \param preTransform Describes the Transformation of the Images before the Presentation
		* \param compositeAlpha Describes the Alpha compositing mode to use
		* \param presentMode The presentation Mode which should be used
		* \param clipped Describes if Pixels outside the Presentation region should be discarded
		* \param oldSwapchain The old Swapchain. Used for Swapchainrecreation.
		*/
		Swapchain::Swapchain(std::shared_ptr<Device> const& device, VkSurfaceKHR const& surface, uint32_t minImageCount, VkFormat imageFormat, VkColorSpaceKHR colorSpace, VkExtent2D imageExtent,
			uint32_t imageArrayLayers, VkImageUsageFlags imageUsage, VkSharingMode imageSharingMode, std::vector<uint32_t> queueFamilyIndices,
			VkSurfaceTransformFlagBitsKHR preTransform, VkCompositeAlphaFlagBitsKHR compositeAlpha, VkPresentModeKHR presentMode, VkBool32 clipped,
			std::shared_ptr<Swapchain> oldSwapchain);
		~Swapchain();
		/**
		* Getter Function to retrieve the Vulkan Handle
		* \return The Vulkan Swapchain Handle
		*/
		VkSwapchainKHR getSwapchain() { return swapchain; };
		/**
		* Getter Function to retrieve the images used by the swapchain
		* \return a vector of pointer to the swapchainimages
		*/
		std::vector<std::shared_ptr<Image>> getImages() const{ return images; };
		/**
		* Getter Function to retrieve the next Vulkan Image
		* \param timeout a timeout in nanoseconds for an image to become available
		* \param fence The fence which should be used for synchronisation
		* \return The Swapchaindata which contains the ImageIndex and a VkResult
		* \see swapChainImageData Fence
		*/
		swapChainImageData Swapchain::acquireNextImage(uint64_t timeout, std::shared_ptr<Fence> const& fence = nullptr);
		/**
		* Getter Function to retrieve the Semaphores which are currently used for Presentation Synchronization
		* \return A vector of Pointers to Semaphores, which are used for presentation synchronization. For each image one Semaphore is saved. The index of the Semaphore vector is also the Index of the Images.
		*/
		std::vector<std::shared_ptr<Semaphore>> getPresentSemaphores() { return presentCompleteSemaphore; };
	private:
		std::shared_ptr<Device> device;

		VkSwapchainKHR	swapchain;

		std::vector<std::shared_ptr<Image>> images;
		std::vector<std::shared_ptr<Semaphore>> presentCompleteSemaphore;
		std::shared_ptr<Semaphore>              freeSemaphore;
	};

}
