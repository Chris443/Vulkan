#pragma once
#include "common.h"


namespace vkf {
	class Shader;
	class RenderPass;
	class PipelineLayout;
	class Device;
	class Pipeline;
	/**
	* \brief Class to create common Pipelines
	*/
	class PipelineManager
	{
	public:
		PipelineManager();
		/**
		* \brief Creates a Basic Pipeline, used for usual drawing operations.
		* \param device The logical Device to this Pipeline
		* \param shader The set of Shaders this pipeline should use
		* \param viewPortExtents The size of the viewport which should be used by this Pipeline
		* \param renderpass Renderpass of this Pipeline
		* \param pipelineLayout pipelineLayout of this Pipeline
		* \return A shared Pointer of the created Pipeline
		*/
		std::shared_ptr<Pipeline> createBasicPipeline(std::shared_ptr<Device> device, Shader shader,
			VkExtent2D viewPortExtents, std::shared_ptr<RenderPass> renderpass, std::shared_ptr<PipelineLayout> pipelineLayout);
		/**
		* \brief Creates a Wireframe Pipeline.
		* \param device The logical Device to this Pipeline
		* \param shader The set of Shaders this pipeline should use
		* \param viewPortExtents The size of the viewport which should be used by this Pipeline
		* \param renderpass Renderpass of this Pipeline
		* \param pipelineLayout pipelineLayout of this Pipeline
		* \return A shared Pointer of the created Pipeline
		*/
		std::shared_ptr<Pipeline> createWireframePipeline(std::shared_ptr<Device> device, Shader shader,
			VkExtent2D viewPortExtents, std::shared_ptr<RenderPass> renderpass, std::shared_ptr<PipelineLayout> pipelineLayout);
		/**
		* \brief Creates a Cubemap Pipeline.
		* \param device The logical Device to this Pipeline
		* \param shader The set of Shaders this pipeline should use
		* \param viewPortExtents The size of the viewport which should be used by this Pipeline
		* \param renderpass Renderpass of this Pipeline
		* \param pipelineLayout pipelineLayout of this Pipeline
		* \return A shared Pointer of the created Pipeline
		*/
		std::shared_ptr<Pipeline> createCubemapPipeline(std::shared_ptr<Device> device, Shader shader,
			VkExtent2D viewPortExtents, std::shared_ptr<RenderPass> renderpass, std::shared_ptr<PipelineLayout> pipelineLayout);

		std::shared_ptr<Pipeline> createTessPipeline(std::shared_ptr<Device> device, Shader shader,
			VkExtent2D viewPortExtents, std::shared_ptr<RenderPass> renderpass, std::shared_ptr<PipelineLayout> pipelineLayout,uint32_t patchPoints);
	};
}

