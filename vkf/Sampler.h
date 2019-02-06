#pragma once
#include "common.h"


namespace vkf {
	class Device;
	/**
	* \brief The Sampler class encapsulates the VkSampler Handle and takes care of initialization and deleting. 
	* For initialization, use the Device.createSampler() function inside the Device class.
	*/
	class Sampler
	{
	public:
	/**
	* \brief Used to create a Vulkan Sampler with given Parameters.
	* \param device The device which creates the Sampler
	* \param magFilter Specifies the magnification filter to apply to lookups.
	* \param minFilter  Specifies the minification filter to apply to lookups.
	* \param mipmapMode Specifies the mipmap filter to apply to lookups.
	* \param addressModeU specifies addressing mode outside of 0 and 1 range for U coordinate
	* \param addressModeV specifies addressing mode outside of 0 and 1 range for V coordinate
	* \param addressModeW specifies addressing mode outside of 0 and 1 range for W coordinate
	* \param mipLodBias The Bias to be added to LoD calculations
	* \param anisotropyEnable Enables or Disables anisotropic filtering
	* \param maxAnisotropy The anisotropy value which should be used if anisotropyEnable is true
	* \param compareEnable Enables or Disables comparison against a reference for lookups
	* \param compareOp The value to use for comparison 
	* \param minLod minValue to clamp for LoD
	* \param maxLod maxValue to clamp for LoD
	* \param borderColor Color to use for the border
	* \param unnormalizedCoordinates Defines if Texel Coordinates should be normalized or not
	*/
		Sampler(std::shared_ptr<Device> device, VkFilter magFilter, VkFilter minFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressModeU,
			VkSamplerAddressMode addressModeV, VkSamplerAddressMode addressModeW, float mipLodBias, bool anisotropyEnable, float maxAnisotropy, bool compareEnable,
			VkCompareOp compareOp, float minLod, float maxLod, VkBorderColor borderColor, bool unnormalizedCoordinates);
		~Sampler();
		/**
		* \brief Getter Function to retrieve the Vulkan Sampler Handle
		* \return The Vulkan Sampler Handle to this Sampler
		*/
		VkSampler getSampler() { return sampler; }
	private:
		std::shared_ptr<Device> device;

		VkSampler sampler;

	};
}

