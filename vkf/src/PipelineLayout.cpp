#include "../PipelineLayout.h"
#include "../Device.h"

namespace vkf {

	PipelineLayout::PipelineLayout(std::shared_ptr<Device> device,std::vector<std::shared_ptr<DeskriptorSetLayout>> setLayouts, std::vector<VkPushConstantRange> pushConstantRanges)
		:device(device)
	{
		VkPipelineLayoutCreateInfo pipelinecreateInfo = {};
		pipelinecreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelinecreateInfo.pNext = NULL;
		pipelinecreateInfo.flags = 0;

		std::vector<VkDescriptorSetLayout> layouts;
		for (auto a : setLayouts) {
			layouts.push_back(a->getDescriptorSetLayout());
		}

		pipelinecreateInfo.setLayoutCount = layouts.size();
		pipelinecreateInfo.pSetLayouts = (layouts.size() > 0) ? layouts.data() : nullptr;
		pipelinecreateInfo.pushConstantRangeCount = pushConstantRanges.size();
		pipelinecreateInfo.pPushConstantRanges = (pushConstantRanges.size() > 0) ? pushConstantRanges.data() : nullptr;

		VkResult result;
		result = vkCreatePipelineLayout(device->getDevice(),&pipelinecreateInfo, NULL, &pipelineLayout);
		assert(!result);
	}
	PipelineLayout::~PipelineLayout()
	{
		vkDestroyPipelineLayout(device->getDevice(), pipelineLayout, NULL);
	}
}