#include "../Swapchain.h"
#include "../Device.h"

namespace vkf {

	Swapchain::Swapchain(std::shared_ptr<Device> const& device, VkSurfaceKHR const& surface, uint32_t minImageCount, VkFormat imageFormat, VkColorSpaceKHR colorSpace, VkExtent2D imageExtent,
		uint32_t imageArrayLayers, VkImageUsageFlags imageUsage, VkSharingMode imageSharingMode, std::vector<uint32_t> queueFamilyIndices,
		VkSurfaceTransformFlagBitsKHR preTransform, VkCompositeAlphaFlagBitsKHR compositeAlpha, VkPresentModeKHR presentMode, VkBool32 clipped,
		std::shared_ptr<Swapchain> oldSwapchain)
		:device(device)
	{
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.pNext = NULL;
		createInfo.flags = 0;
		createInfo.surface = surface;
		createInfo.minImageCount = minImageCount; 
		createInfo.imageFormat = imageFormat; 
		createInfo.imageColorSpace = colorSpace;
		createInfo.imageExtent = imageExtent;
		createInfo.imageArrayLayers = imageArrayLayers; //stereoskope 3D bilder, z.B. VR
		createInfo.imageUsage = imageUsage; //wofür wird das bild verwendet: color attachement = malen
		createInfo.imageSharingMode = imageSharingMode; 
		createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		createInfo.preTransform = preTransform;
		createInfo.presentMode = presentMode;
		createInfo.clipped = clipped;
		createInfo.compositeAlpha = compositeAlpha;
		createInfo.oldSwapchain = oldSwapchain ? oldSwapchain->getSwapchain() : VK_NULL_HANDLE; 

		VkResult result;
		result = vkCreateSwapchainKHR(device->getDevice() , &createInfo, NULL, &swapchain);
		assert(!result);

		std::vector<VkImage> swpImages;
		vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &minImageCount, nullptr);
		swpImages.resize(minImageCount);
		result = vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &minImageCount, swpImages.data());

		assert(!result);

		images.reserve(swpImages.size());
		presentCompleteSemaphore.reserve(images.size());
		for (size_t i = 0; i < swpImages.size(); i++)
		{
			images.push_back(std::make_shared<Image>(device, swpImages[i]));
			presentCompleteSemaphore.push_back(std::make_shared<Semaphore>(device));
		}
		freeSemaphore = std::make_shared<Semaphore>(device);
	}

	Swapchain::~Swapchain() {
		images.clear();
		vkDestroySwapchainKHR(device->getDevice(), swapchain, NULL);
	}


	swapChainImageData Swapchain::acquireNextImage(uint64_t timeout, std::shared_ptr<Fence> const& fence)
	{
		swapChainImageData imageData;
		imageData.imageIndex = -1;
		imageData.result = vkAcquireNextImageKHR(device->getDevice(),swapchain, timeout, freeSemaphore->getSemaphore(), (fence != nullptr) ? fence->getFence() : VK_NULL_HANDLE,&imageData.imageIndex);
		//assert(!res);



		std::swap(freeSemaphore, presentCompleteSemaphore[imageData.imageIndex]);
		return imageData;
	}

}