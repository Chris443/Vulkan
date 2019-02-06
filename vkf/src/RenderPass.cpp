#include "../RenderPass.h"
#include "../Device.h"

namespace vkf {
	
	RenderPass::RenderPass(std::shared_ptr<Device> device, std::vector<VkAttachmentDescription> attachmentDescriptions,
	 std::vector<VkSubpassDescription> subpassdescription, std::vector<VkSubpassDependency> subpassDebendencies)
	:device(device){

		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.attachmentCount = attachmentDescriptions.size();
		createInfo.pAttachments = (attachmentDescriptions.size() > 0) ? attachmentDescriptions.data() : nullptr;
		createInfo.subpassCount = subpassdescription.size();
		createInfo.pSubpasses = (subpassdescription.size() > 0) ? subpassdescription.data() : nullptr;
		createInfo.dependencyCount = subpassDebendencies.size();
		createInfo.pDependencies= (subpassDebendencies.size() > 0 ) ? subpassDebendencies.data() : nullptr;

		VkResult result = vkCreateRenderPass(device->getDevice(), &createInfo, nullptr, &renderpass);
		assert(!result);
	}


	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(device->getDevice(), renderpass, NULL);
	}
}
