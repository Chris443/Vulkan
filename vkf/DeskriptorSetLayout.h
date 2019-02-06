#pragma once
#include "common.h"


namespace vkf {
	class Device;
/**
* \brief Simple Wrapper Class for Vulkan DescriptorSetLayouts. Use the Device.create() function inside the Device class to create a DescriptorSetLayout.
*/
	class DeskriptorSetLayout {
	public:
		/**
		* \brief
		* \param device The device that created the DescriptorSetLayout
		* \param bindings An Array of VkDescriptorSetLayoutBinding which describe the Descriptors binding.
		*/
		DeskriptorSetLayout(std::shared_ptr<Device> device,
			std::vector<VkDescriptorSetLayoutBinding> bindings);
		~DeskriptorSetLayout();
		/**
		* \brief Returns the Vulkan Handle of this Class
		* \return The Vulkan DescriptorSetLayout Handle
		*/
		VkDescriptorSetLayout getDescriptorSetLayout() {return descriptorSetLayout;	}
	private:
		/**
		* \brief The corresponding Device.
		*/
		std::shared_ptr<Device> m_device;
		/**
		* \brief The Vulkan DescriptorSetLayout Handle.
		*/
		VkDescriptorSetLayout descriptorSetLayout;
	};
}