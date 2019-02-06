#include "../Sampler.h"
#include "../Device.h"

namespace vkf {

	Sampler::Sampler(std::shared_ptr<Device> device, VkFilter magFilter, VkFilter minFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressModeU,
		VkSamplerAddressMode addressModeV, VkSamplerAddressMode addressModeW, float mipLodBias, bool anisotropyEnable, float maxAnisotropy, bool compareEnable,
		VkCompareOp compareOp, float minLod, float maxLod, VkBorderColor borderColor, bool unnormalizedCoordinates)
		:device(device)
	{
		VkSamplerCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.magFilter = magFilter;
		info.minFilter = minFilter;
		info.mipmapMode = mipmapMode;
		info.addressModeU = addressModeU;
		info.addressModeV = addressModeV;
		info.addressModeW = addressModeW;
		info.mipLodBias = mipLodBias;
		info.anisotropyEnable = anisotropyEnable;
		info.maxAnisotropy = maxAnisotropy;
		info.compareEnable = compareEnable;
		info.compareOp = compareOp;
		info.minLod = minLod;
		info.maxLod = maxLod;
		info.borderColor = borderColor;
		info.unnormalizedCoordinates = unnormalizedCoordinates;

		VkResult res = 	vkCreateSampler(device->getDevice(), &info, nullptr, &sampler);
		assert(!res);
	}


	Sampler::~Sampler()
	{
		vkDestroySampler(device->getDevice(), sampler, nullptr);
	}
}