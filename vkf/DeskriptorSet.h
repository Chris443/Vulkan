#pragma once
#include "common.h"

namespace vkf {
	class Device;
	class DeskriptorPool;
	class DeskriptorSetLayout;
	/**
	* \brief  The DeskriptorSet class encapsulates the VkDeskriptorSet Handle and takes care of initialization and deleting. 
	*  For creation, use Device.createDeskriptorset() inside the Device class.
	*/
	class DeskriptorSet {
	public:
		/**
		* \brief Creates a Vulkan DescriptorSet. Use Device.createDeskriptorset() inside the Device class instead.
		*\param device The corresponding Device to this DescriptorSet
		*\param pool The corresponding CommandPool to this DescriptorSet
		*\param layouts The Layout which define how the DescriptorSet should be allocated
		*/
		DeskriptorSet(std::shared_ptr<Device> const& device, std::shared_ptr<DeskriptorPool> const& pool, std::shared_ptr<DeskriptorSetLayout> layouts);
		~DeskriptorSet();
		/**
		* \brief Updates the DescriptorSet.
		* \param writeDescriptorSets An Array of VkWriteDescriptorSets which descripte how the Descriptorsets should be updated.
		*/
		void deskriptorWrite(std::vector<VkWriteDescriptorSet> const& writeDescriptorSets);
		/**
		* \brief Returns the Vulkan Descriptor Handle
		* \return The Vulkan Descriptor Handle
		*/
		VkDescriptorSet getDescriptorSet() { return m_descriptorSet; };
	private:
		/**
		* \brief The Corresponding Device to this DescriptorSet
		*/
		std::shared_ptr<Device> m_device;
		/**
		* \brief The Corresponding DescriptorPool to this DescriptorSet
		*/
		std::shared_ptr<DeskriptorPool> m_pool;
		/**
		* \brief The Vulkan DescriptorSet Handle
		*/
		VkDescriptorSet m_descriptorSet;
	};
}