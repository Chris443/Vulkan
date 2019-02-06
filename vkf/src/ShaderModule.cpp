#include "../ShaderModule.h"
#include "../Device.h"

namespace vkf {

	ShaderModule::ShaderModule(std::shared_ptr<Device> device,std::string shaderFilePath)
		:device(device)
	{
		shaderCode = readFile(shaderFilePath.c_str());

		VkShaderModuleCreateInfo shaderCreateInfo;
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderCreateInfo.pNext = NULL;
		shaderCreateInfo.flags = 0;
		shaderCreateInfo.codeSize = shaderCode.size();
		shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

		VkResult result;
		result = vkCreateShaderModule(device->getDevice(), &shaderCreateInfo, NULL, &shaderModule);
		assert(!result);
	}

	ShaderModule::~ShaderModule()
	{
		vkDestroyShaderModule(device->getDevice(), this->shaderModule, nullptr);
	}

	std::vector<char> ShaderModule::readFile(const char *shaderFilePath) {
		std::ifstream file(shaderFilePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}
}