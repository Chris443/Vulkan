#include "../Framebuffer.h"
#include "../Device.h"

namespace vkf {
	
	Framebuffer::Framebuffer(std::shared_ptr<Device> device, std::shared_ptr<RenderPass> renderpass,
		std::vector<std::shared_ptr<ImageView>> attachments, VkExtent2D const& extents, uint32_t layers)
		:device(device),
		renderpass(renderpass)
	{

	VkFramebufferCreateInfo framebufferCreateInfo = {};
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.pNext = NULL;
	framebufferCreateInfo.flags = 0;
	framebufferCreateInfo.renderPass = renderpass->getRenderpass();
	framebufferCreateInfo.attachmentCount = attachments.size();//attachments.size(); //wieviele imageviews pro framebuffer, z.B. Colors,Depth,normalmaps

	std::vector<VkImageView> attViews;
	for (uint32_t i = 0; i < attachments.size(); ++i) {
		attViews.push_back(attachments[i]->getImageView());
	}

	framebufferCreateInfo.pAttachments = attViews.data();
	framebufferCreateInfo.width = extents.width;
	framebufferCreateInfo.height = extents.height;
	framebufferCreateInfo.layers = layers;

	VkResult result;
	result = vkCreateFramebuffer(device->getDevice(), &framebufferCreateInfo, NULL, &framebuffer);
	assert(!result);
	}


	Framebuffer::~Framebuffer()
	{
			vkDestroyFramebuffer(device->getDevice(), framebuffer, NULL);
		}
	}


