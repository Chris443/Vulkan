#include "../Pipeline.h"
#include "../Device.h"
#include "../RenderPass.h"
#include "../PipelineLayout.h"

namespace vkf {

	PipelineVertexInputStateCreateInfo::PipelineVertexInputStateCreateInfo(std::vector<VkVertexInputBindingDescription> &vertexBindingDescriptions,
		std::vector<VkVertexInputAttributeDescription> &vertexAttributeDesriptions) {
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.vertexBindingDescriptionCount = vertexBindingDescriptions.size();
		createInfo.pVertexBindingDescriptions = (vertexBindingDescriptions.size() > 0) ? vertexBindingDescriptions.data() : nullptr;
		createInfo.vertexAttributeDescriptionCount = vertexAttributeDesriptions.size();
		createInfo.pVertexAttributeDescriptions = (vertexAttributeDesriptions.size() > 0) ? vertexAttributeDesriptions.data() : nullptr;
	}

	PipelineInputAssemblyStateCreateInfo::PipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable) {
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.topology = topology;
		createInfo.primitiveRestartEnable = primitiveRestartEnable;
	}

	PipelineRasterizationStateCreateInfo::PipelineRasterizationStateCreateInfo(VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable, VkPolygonMode polygonMode,
		VkCullModeFlags cullmode, VkFrontFace frontface, VkBool32 depthBiasEnable,
		float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.depthClampEnable = depthClampEnable;
		createInfo.rasterizerDiscardEnable = rasterizerDiscardEnable;
		createInfo.polygonMode = polygonMode;
		createInfo.cullMode = cullmode;
		createInfo.frontFace = frontface;
		createInfo.depthBiasEnable = depthBiasEnable;
		createInfo.depthBiasConstantFactor = depthBiasConstantFactor;
		createInfo.depthBiasClamp = depthBiasClamp;
		createInfo.depthBiasSlopeFactor = depthBiasSlopeFactor;
		createInfo.lineWidth = lineWidth;
	}


	PipelineShaderStageCreateInfo::PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage,
		std::shared_ptr<ShaderModule> shaderModule, std::string const& startName, VkSpecializationInfo* pSpec)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.stage = stage;
		createInfo.module = shaderModule->getModule();
		name.assign(startName);
		createInfo.pName = this->name.c_str();
		createInfo.pSpecializationInfo = nullptr;
	}

	PipelineViewportStateCreateInfo::PipelineViewportStateCreateInfo(std::vector<VkViewport> viewports,
		std::vector<VkRect2D> scissors)
	:m_scissors(scissors),
	m_viewports(viewports){
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.viewportCount = m_viewports.size();
		createInfo.pViewports = m_viewports.data();
		createInfo.scissorCount = m_scissors.size();
		createInfo.pScissors = m_scissors.data();
	}
	//predefined viewport/scissor setup
	PipelineViewportStateCreateInfo::PipelineViewportStateCreateInfo(VkExtent2D viewPortMaxima) {
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;

		VkViewport viewport; // as shared ptr?
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)viewPortMaxima.width;
		viewport.height = (float)viewPortMaxima.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		m_viewports.push_back(viewport);

		VkRect2D scissor;
		scissor.offset = { 0,0 };
		scissor.extent = { viewPortMaxima.width, viewPortMaxima.height};
		m_scissors.push_back(scissor);

		createInfo.viewportCount = 1;
		createInfo.pViewports = &m_viewports[0];

		createInfo.scissorCount = 1;
		createInfo.pScissors = &m_scissors[0];
	}

	PipelineMultisampleStateCreateInfo::PipelineMultisampleStateCreateInfo(VkSampleCountFlagBits rasterizationSamples, VkBool32 sampleShadingEnable,
		float minSampleShading,const VkSampleMask* sampleMask, VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnable) {
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.rasterizationSamples = rasterizationSamples;
		createInfo.sampleShadingEnable = sampleShadingEnable;
		createInfo.minSampleShading = minSampleShading;
		createInfo.pSampleMask = sampleMask ? sampleMask : nullptr;
		createInfo.alphaToCoverageEnable = alphaToCoverageEnable;
		createInfo.alphaToOneEnable = alphaToOneEnable;
	}

	PipelineColorBlendAttachementState::PipelineColorBlendAttachementState(VkBool32 blendEnable, VkBlendFactor srcColFactor, VkBlendFactor dstColFactor, VkBlendOp colBlendOp,
		VkBlendFactor stdAlphaFactor, VkBlendFactor dstAlphaFactor, VkBlendOp alphaOp, VkColorComponentFlags colorWriteMask) {

		attachement.blendEnable = blendEnable;
		attachement.srcColorBlendFactor = srcColFactor;
		attachement.dstColorBlendFactor = dstColFactor;
		attachement.colorBlendOp = colBlendOp;
		attachement.srcAlphaBlendFactor = stdAlphaFactor;
		attachement.dstAlphaBlendFactor = dstAlphaFactor;
		attachement.alphaBlendOp = alphaOp;
		attachement.colorWriteMask = colorWriteMask;	
	}

	
	PipelineColorBlendStateCreateInfo::PipelineColorBlendStateCreateInfo(std::vector<PipelineColorBlendAttachementState> attachements,VkLogicOp logicOp,
		float blendConst_1,float blendConst_2,float blendConst_3, float blendConst_4) {
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;

		if(logicOp != VK_LOGIC_OP_NO_OP)
			createInfo.logicOpEnable = VK_TRUE;
		else
			createInfo.logicOpEnable = VK_FALSE;
		createInfo.logicOp = logicOp;
		if (attachements.size() > 0) {
			for (uint32_t i = 0; i < attachements.size(); ++i) {
				(this->attachements).push_back(attachements[i].attachement);
			}
			createInfo.attachmentCount = attachements.size();
			createInfo.pAttachments = this->attachements.data();
		}
		else {
			createInfo.attachmentCount = 0;
			createInfo.pAttachments = nullptr;
		}
		
		createInfo.blendConstants[0] = blendConst_1;
		createInfo.blendConstants[1] = blendConst_2;
		createInfo.blendConstants[2] = blendConst_3;
		createInfo.blendConstants[3] = blendConst_4;
	}

	PipelineDepthStencilStateCreateInfo::PipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp, VkBool32 depthBoundsTestEnable 
			, VkBool32 stencilTestEnable, VkStencilOpState front, VkStencilOpState back, float minDepthBounds, float maxDepthBounds){

		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.depthTestEnable = depthTestEnable;
		createInfo.depthWriteEnable = depthWriteEnable;
		createInfo.depthCompareOp = depthCompareOp;
		createInfo.depthBoundsTestEnable = depthBoundsTestEnable;
		createInfo.stencilTestEnable = stencilTestEnable;
		createInfo.front = front;
		createInfo.back = back;
		createInfo.minDepthBounds = minDepthBounds;
		createInfo.maxDepthBounds = maxDepthBounds;
	};


	Pipeline::Pipeline(
		std::shared_ptr<Device> device, 
		std::shared_ptr<PipelineLayout> pipelineLayout,
		std::shared_ptr<RenderPass> renderpass,
		uint32_t subpass,

		std::vector<PipelineShaderStageCreateInfo> shaderstages,
		PipelineVertexInputStateCreateInfo* vertexInputStateCreateInfo,
		PipelineInputAssemblyStateCreateInfo* inputAssemblyStateCreateInfo,
		PipelineRasterizationStateCreateInfo* rasterizationStateCreateInfo,
		PipelineViewportStateCreateInfo* viewportStateCreateInfo,
		PipelineMultisampleStateCreateInfo* multisampleinfo,
		PipelineColorBlendStateCreateInfo* colorBlendStateCreateInfo,
		PipelineDepthStencilStateCreateInfo* depthStencilCreateInfo,
		VkPipelineTessellationStateCreateInfo* tessellationStateCreateInfo,
		VkPipelineDynamicStateCreateInfo* dynamicStateCreateInfo, 
		VkPipeline basePipelineHandle,
		uint32_t basePipelineIndex,
		VkPipelineCache pipelineCache)
		:device(device)
	{
		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.pNext = nullptr; 
		createInfo.flags = 0;
		createInfo.stageCount = shaderstages.size();

		std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos;
		for (uint32_t i = 0; i < shaderstages.size(); ++i) {
			shaderCreateInfos.push_back(shaderstages[i].createInfo);
		}


		createInfo.pStages = (shaderstages.size() >  0) ? shaderCreateInfos.data() : nullptr;
		createInfo.pVertexInputState = vertexInputStateCreateInfo ? &vertexInputStateCreateInfo->createInfo : nullptr;
		createInfo.pInputAssemblyState = inputAssemblyStateCreateInfo ? &inputAssemblyStateCreateInfo->createInfo : nullptr;
		createInfo.pTessellationState = tessellationStateCreateInfo;
		createInfo.pViewportState = viewportStateCreateInfo ? &viewportStateCreateInfo->createInfo : nullptr;
		createInfo.pRasterizationState = rasterizationStateCreateInfo ? &rasterizationStateCreateInfo->createInfo : nullptr;
		createInfo.pMultisampleState = multisampleinfo ? &multisampleinfo->createInfo : nullptr;
		createInfo.pDepthStencilState = depthStencilCreateInfo ? &depthStencilCreateInfo->createInfo : nullptr;
		createInfo.pColorBlendState = colorBlendStateCreateInfo ? &colorBlendStateCreateInfo->createInfo : nullptr;
		createInfo.pDynamicState = dynamicStateCreateInfo; 
		createInfo.layout = pipelineLayout->getPipelineLayout();
		createInfo.renderPass = renderpass->getRenderpass(); 
		createInfo.subpass = subpass; 
		createInfo.basePipelineHandle = basePipelineHandle; 
		createInfo.basePipelineIndex = basePipelineIndex; 

		VkResult res =	vkCreateGraphicsPipelines(device->getDevice(), pipelineCache, 1, &createInfo, nullptr, &pipeline);
		assert(!res);

	}

	Pipeline::~Pipeline()
	{
		vkDestroyPipeline(device->getDevice(), pipeline, nullptr);
	}
}
/*
	void Pipeline::bind(VkCommandBuffer *cmdBuffer) {
		vkCmdBindPipeline(*cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	}*/
