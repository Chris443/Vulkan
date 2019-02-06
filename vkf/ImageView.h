#pragma once
#include "common.h"

namespace vkf {
	class Device;
	class ImageView

	/**
	* \brief The ImageView class encapsulates the VkImageView Handle and takes care of initialization and deleting. 
	* For creating an ImageView use the createImageView function inside the Image class.
	*/
	{
	public:
		/**
		* \brief Creates an ImageView 
		* \param device The logical device that creates the image view.
		* \param image The Image to which the View is created
		* \param viewType Specifies the Type of Image, i.e. VK_IMAGE_VIEW_TYPE_1D, VK_IMAGE_VIEW_TYPE_2D...
 		* \param format The format of the Image
		* \param components Specifies the mapping of Colors
		* \param subresourceRange Used for Mipmapping
		* \see Image
		*/
		ImageView(std::shared_ptr<Device> const& device, VkImage const& image,VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D,
			VkFormat format = VK_FORMAT_B8G8R8A8_UNORM, VkComponentMapping components = { VK_COMPONENT_SWIZZLE_IDENTITY },
			VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT,0,1,0,1 });

		~ImageView();
		/**
		* \brief Getter for the Vulkan Handle
		* \return The Vulkan ImageView Handle
		*/
		VkImageView getImageView()const { return imageView; }
	private:
		std::shared_ptr<Device> device;
		VkImageView imageView;
	};

}
