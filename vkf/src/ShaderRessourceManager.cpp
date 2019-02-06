#include "../ShaderRessourceManager.h"
#include "../DeskriptorSet.h"
#include "../Buffer.h"
#include "../Device.h"
#include "../Texture.h"

namespace vkf {
	ShaderRessourceManager::ShaderRessourceManager() {}

	void ShaderRessourceManager::initSet(std::shared_ptr<DeskriptorSet> set) {
		this->set = set;
	}

	void ShaderRessourceManager::createUniformBuffer(VkShaderStageFlags shaderStage, uint32_t bindingLocation) {

		VkDescriptorSetLayoutBinding uniformBufferBinding;
		uniformBufferBinding.binding = bindingLocation;
		uniformBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uniformBufferBinding.descriptorCount = 1;
		uniformBufferBinding.stageFlags = shaderStage;
		uniformBufferBinding.pImmutableSamplers = nullptr;

		descriptors.push_back(uniformBufferBinding);
	}

	void ShaderRessourceManager::updateUniformBuffer(std::shared_ptr<Buffer> buffer, VkDeviceSize offset, VkDeviceSize size,uint32_t bindingLocation) {
		VkDescriptorBufferInfo descriptorBufferInfo;
		descriptorBufferInfo.buffer = buffer->getBuffer();
		descriptorBufferInfo.offset = offset;
		descriptorBufferInfo.range = size;

		VkWriteDescriptorSet descriptorWrite;
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.pNext = 0;
		descriptorWrite.dstSet = set->getDescriptorSet();
		descriptorWrite.dstBinding = bindingLocation;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.pBufferInfo = &descriptorBufferInfo;

		set->deskriptorWrite({ descriptorWrite });
	}

	void ShaderRessourceManager::createSampler(VkShaderStageFlags shaderStage, uint32_t bindingLocation) {
		VkDescriptorSetLayoutBinding samplerBinding;
		samplerBinding.binding = bindingLocation;
		samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerBinding.descriptorCount = 1;
		samplerBinding.stageFlags = shaderStage;
		samplerBinding.pImmutableSamplers = nullptr;

		descriptors.push_back(samplerBinding);
	}

	void ShaderRessourceManager::updateSampler(std::shared_ptr<Texture> texture, uint32_t bindingLocation) {
		VkDescriptorImageInfo imageInfo;
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture->getImageView()->getImageView();
		imageInfo.sampler = texture->getSampler()->getSampler();

		VkWriteDescriptorSet descriptorWrite;
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.pNext = 0;
		descriptorWrite.dstSet = set->getDescriptorSet();
		descriptorWrite.dstBinding = bindingLocation;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.pImageInfo = &imageInfo;
		set->deskriptorWrite({descriptorWrite});
	}

	void ShaderRessourceManager::write() {
		set->deskriptorWrite(writes);
	}

	void ShaderRessourceManager::initDeskriptorSetLayout(std::shared_ptr<Device> device) {
		layout = device->createDeskriptorSetLayout(descriptors);
	}

}
