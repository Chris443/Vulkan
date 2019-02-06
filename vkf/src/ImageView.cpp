#include "../ImageView.h"
#include "../Device.h"

namespace vkf {
	ImageView::ImageView(std::shared_ptr<Device> const& device, VkImage const& image, VkImageViewType viewType, 
		VkFormat format, VkComponentMapping components,
		VkImageSubresourceRange subresourceRange)
		:device(device)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.image = image;
		createInfo.viewType = viewType;
		createInfo.format = format;
		createInfo.components = components;
		createInfo.subresourceRange = subresourceRange;

		VkResult res = vkCreateImageView(device->getDevice(), &createInfo, nullptr, &imageView);
		assert(!res);
	}

	ImageView::~ImageView()
	{
		vkDestroyImageView(device->getDevice(), imageView, nullptr);
	}

}