#include "../DeskriptorSetLayout.h"
#include "../Device.h"

namespace vkf {
	DeskriptorSetLayout::DeskriptorSetLayout(std::shared_ptr<Device> device, std::vector<VkDescriptorSetLayoutBinding> bindings)
		:m_device(device) {
		VkDescriptorSetLayoutCreateInfo info;

		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.bindingCount = bindings.size();
		info.pBindings = (bindings.size() > 0) ? bindings.data() : nullptr;

		vkCreateDescriptorSetLayout(m_device->getDevice(), &info, nullptr, &descriptorSetLayout);
	}

	DeskriptorSetLayout::~DeskriptorSetLayout() {
		vkDestroyDescriptorSetLayout(m_device->getDevice(), descriptorSetLayout, nullptr);
	}
}
