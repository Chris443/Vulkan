#pragma once
#include "common.h"
#include "Pipeline.h"

namespace vkf {
	class Device;
	/**
	* \brief Class to create a Set of Shaders of a given Path. 
	*/
	class Shader
	{
	public:
		Shader();
		/**
		* \brief Creates a set of Shaders of the given Path. Shaders have to have the ending "vert.spv" and "frag.spv"
		*/
		void createShaders(std::shared_ptr<Device> device, std::string shaderPath);
		/**
		* \brief Returns the Shaderstages for PipelineCreation
		* \return All Shaderstages of this Shader
		*/
		std::vector<PipelineShaderStageCreateInfo> getShaderStages();
	private:
		std::vector<PipelineShaderStageCreateInfo> shaderStages;
		std::shared_ptr<vkf::ShaderModule> vertShaderModule;
		std::shared_ptr<vkf::ShaderModule> fragmentShaderModule;
		std::shared_ptr<vkf::ShaderModule> tescShaderModule;
		std::shared_ptr<vkf::ShaderModule> teseShaderModule;
		std::shared_ptr<vkf::ShaderModule> geomShaderModule;
		std::shared_ptr<vkf::PipelineShaderStageCreateInfo> vertShaderStageInfo;
		std::shared_ptr<vkf::PipelineShaderStageCreateInfo> fragShaderStageInfo;
		std::shared_ptr<vkf::PipelineShaderStageCreateInfo> tescShaderStageInfo;
		std::shared_ptr<vkf::PipelineShaderStageCreateInfo> teseShaderStageInfo;
		std::shared_ptr<vkf::PipelineShaderStageCreateInfo> geomShaderStageInfo;
	};
}
