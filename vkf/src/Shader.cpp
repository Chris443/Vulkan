#include "../Shader.h"
#include "../Device.h"

namespace vkf {

	Shader::Shader() {	}
	std::vector<PipelineShaderStageCreateInfo> Shader::getShaderStages() {
		return shaderStages;
	}

	void Shader::createShaders(std::shared_ptr<Device> device, std::string shaderPath) {
		{	
			vertShaderModule = device->createShaderModule(shaderPath + "/vert.spv");
			fragmentShaderModule = device->createShaderModule(shaderPath + "/frag.spv");
			tescShaderModule = device->createShaderModule(shaderPath + "/tesc.spv");
			teseShaderModule = device->createShaderModule(shaderPath + "/tese.spv");
			geomShaderModule = device->createShaderModule(shaderPath + "/geom.spv");
			vertShaderStageInfo = std::make_shared<PipelineShaderStageCreateInfo>(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule);
			fragShaderStageInfo = std::make_shared<PipelineShaderStageCreateInfo>(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShaderModule);
			tescShaderStageInfo = std::make_shared<PipelineShaderStageCreateInfo>(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, tescShaderModule);
			teseShaderStageInfo = std::make_shared<PipelineShaderStageCreateInfo>(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, teseShaderModule);
			geomShaderStageInfo = std::make_shared<PipelineShaderStageCreateInfo>(VK_SHADER_STAGE_GEOMETRY_BIT, geomShaderModule);
			shaderStages = { *vertShaderStageInfo,*fragShaderStageInfo, *tescShaderStageInfo,*teseShaderStageInfo, *geomShaderStageInfo };
		}

	}
}
