#pragma once
#include "common.h"
#include "DeskriptorSetLayout.h"

namespace vkf {
	class Buffer;
	class DeskriptorSet;
	class Device;
	class Texture;

	class ShaderRessourceManager
	{
	public:
		ShaderRessourceManager();


		void createUniformBuffer(VkShaderStageFlags shaderStage, uint32_t bindingLocation);
		void updateUniformBuffer(std::shared_ptr<Buffer> buffer, VkDeviceSize offset, VkDeviceSize size, uint32_t bindingLocation);

		void createSampler(VkShaderStageFlags shaderStage, uint32_t bindingLocation);
		void updateSampler(std::shared_ptr<Texture> texture, uint32_t bindingLocation);
		
		void initSet(std::shared_ptr<DeskriptorSet> set);
		void initDeskriptorSetLayout(std::shared_ptr<Device> device);
		std::shared_ptr<DeskriptorSetLayout> getDeskriptorSetLayout() { return layout; }

		void write();

	private:
		std::shared_ptr<DeskriptorSetLayout> layout;
		std::shared_ptr<DeskriptorSet> set;
		std::vector<VkDescriptorSetLayoutBinding> descriptors;
		std::vector<VkWriteDescriptorSet> writes;
	};
}

