#include "../DeskriptorSet.h"
#include "../DeskriptorPool.h"
#include "../DeskriptorSetLayout.h"
#include "../Device.h"

namespace vkf {
	DeskriptorSet::DeskriptorSet(std::shared_ptr<Device> const& device, std::shared_ptr<DeskriptorPool> const& pool, std::shared_ptr<DeskriptorSetLayout> layout)
	:m_device(device),
		m_pool(pool){
		VkDescriptorSetAllocateInfo info;
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.pNext = nullptr;
		info.descriptorPool = pool->getPool();
		info.descriptorSetCount = 1;
		VkDescriptorSetLayout l = layout->getDescriptorSetLayout();
		info.pSetLayouts = &l;

		vkAllocateDescriptorSets(m_device->getDevice(), &info, &m_descriptorSet);
	}

	DeskriptorSet::~DeskriptorSet() {
		vkFreeDescriptorSets(m_device->getDevice(), m_pool->getPool(), 1, &m_descriptorSet);
	}

	void DeskriptorSet::deskriptorWrite(std::vector<VkWriteDescriptorSet> const& writeDescriptorSets) {
		vkUpdateDescriptorSets(m_device->getDevice(), writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
	}

}