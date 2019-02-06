#pragma once
#include "common.h"
#include "Image.h"
#include "ImageView.h"
#include "PhysicalDevice.h"

namespace vkf {
	class Device;
	class Image
		/**
		* \brief The Image class encapsulates the VkImage Handle and takes care of initialization and deleting. For initialization, use the Device.createImage() function inside the Device class.
		*/
	{
	public:
		/**
		* \brief Used for Swapchain initialization. Don't use this constructor unless you know what you are doing.
		* \note This constructor does NOT initialize the VkImage handle. It's only purpose is for swapchain initialization inside the Framework. Use the other Constructor.
		*/
		Image(std::shared_ptr<Device> const& device, VkImage const& image);
		/**
		* \brief Creates and initializes a Vulkan Image Handle.
		* \param device The logical device that creates the Image
		* \param imageType The Type if the Image, i.e. VK_IMAGE_TYPE_1D, VK_IMAGE_TYPE_2D
		* \param format The image Format
		* \param extent The extent on each image dimension
		* \param mipLevels Describes the number of levels for mipmapping
		* \param arrayLayers number of image layers
		* \param samples Used for multisampling. Describes the number of Subdata element samples
		* \param tiling Specifies the tiling arrangement in memory
		* \param usage A bit that Describes the usages of the Image, i.e. VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
		* \param sharingMode Describes the Acces of the image for Queue Families
		* \param queueFamilyIndices The list of QueueFamily which will have access to the Image
		* \param initialLayout The initial Layout of all image subresources
		* \param physicalDevice The physicalDevice, which is used for memory Allocation
		*/
		Image(std::shared_ptr<Device> const& device, VkImageType imageType, VkFormat format, VkExtent3D extent, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples,
			VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndices, VkImageLayout initialLayout,
			PhysicalDevice physicalDevice, VkImageCreateFlags flags);
		~Image();
		/**
		* \brief Used for Swapchain initialization. Don't use this constructor unless you know what you are doing.
		* \note This constructor does NOT initialize the VkImage handle. It's only purpose is for swapchain initialization inside the Framework. Use the other Constructor.
		*/
		std::shared_ptr<ImageView> createImageView(VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D,
			VkFormat format = VK_FORMAT_B8G8R8A8_UNORM, VkComponentMapping components = { VK_COMPONENT_SWIZZLE_IDENTITY },
			VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT,0,1,0,1 });
		/**
		* \brief Returns a basic imageBarrier
		*/
		VkImageMemoryBarrier getImageMemoryBarrier(VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask);
		/**
		* \brief Returns a basic imageBarrier
		*/
		VkImageMemoryBarrier Image::getImageMemoryBarrier(VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageSubresourceRange  subresourceRange);
		/**
		* \brief Getter for the Vulkan Image Handle
		* \return The Vulkan Image Handle
		*/
		VkImage getImage() const { return image; };
	private:
		std::shared_ptr<Device> m_device;
		VkImage image;
		bool m_created;

		VkDeviceMemory memory;
		VkMemoryRequirements memRequirements;
		VkMemoryPropertyFlags memPropFlag;
		VkPhysicalDeviceMemoryProperties memProps;
		std::vector<uint32_t> m_queueFamilyIndices;
	};
}


