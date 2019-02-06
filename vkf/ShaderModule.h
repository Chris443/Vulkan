#pragma once
#include "common.h"

namespace vkf {
	class Device;
	class ShaderModule
		/**
		* \brief The ShaderModule class encapsulates the VkShaderModule Handle and takes care of initialization and deleting.
		* For initialization, use the Device.createShaderModule() function inside the Device class.
		*/
	{
	public:
		/**
		* \brief Creates a ShaderModule 
		* \param device The corresponding logical Device
		* \param shaderFilePath The path to the Shader Code
		*/
		ShaderModule(std::shared_ptr<Device> device,std::string shaderFilePath);
		~ShaderModule();
		/**
		* \brief Getter for the Vulkan Handle
		* \return The Vulkan Shadermodule Handle
		*/
		VkShaderModule getModule() const {	return shaderModule;};

	private:
		std::shared_ptr<Device> device;

		VkShaderModule shaderModule;
		std::vector<char> shaderCode;

		std::vector<char> readFile(const char *shaderFilePath);
	};
}

