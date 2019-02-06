#include "../PipelineManager.h"
#include "../Pipeline.h"
#include "../RenderPass.h"
#include "../PipelineLayout.h"
#include "../Vertex.h"
#include "../Shader.h"
#include "../Device.h"


namespace vkf {

	PipelineManager::PipelineManager(){
	}

	std::shared_ptr<Pipeline> PipelineManager::createBasicPipeline(std::shared_ptr<Device> device, Shader shader,
		VkExtent2D viewPortExtents, std::shared_ptr<RenderPass> renderpass, std::shared_ptr<PipelineLayout> pipelineLayout){

		std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions = { vkf::Vertex::getBindingDescription() };
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions = { vkf::Vertex::getAttributeDescriptions() };
		vkf::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo(vertexBindingDescriptions, vertexAttributeDescriptions);
		vkf::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
		PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
		PipelineViewportStateCreateInfo viewportStateInfo({ viewPortExtents.width, viewPortExtents.height });
		PipelineMultisampleStateCreateInfo multisampleinfo;
		std::vector<vkf::PipelineColorBlendAttachementState> colBlendAttachements = { vkf::PipelineColorBlendAttachementState() };
		PipelineColorBlendStateCreateInfo colblendInfo(colBlendAttachements);
		PipelineDepthStencilStateCreateInfo depthStencilpipelineinfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS);
		std::vector<PipelineShaderStageCreateInfo> shaderstages = shader.getShaderStages();
		std::shared_ptr<Pipeline> pipe = device->createPipeline(pipelineLayout, renderpass,0, shaderstages, &vertexInputStateCreateInfo,
			&inputAssemblyStateCreateInfo, &rasterizationStateCreateInfo, &viewportStateInfo, &multisampleinfo, &colblendInfo, &depthStencilpipelineinfo);
		return pipe;
	}

	std::shared_ptr<Pipeline> PipelineManager::createWireframePipeline(std::shared_ptr<Device> device, Shader shader,
		VkExtent2D viewPortExtents, std::shared_ptr<RenderPass> renderpass, std::shared_ptr<PipelineLayout> pipelineLayout) {

		std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions = { vkf::Vertex::getBindingDescription() };
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions = { vkf::Vertex::getAttributeDescriptions() };
		vkf::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo(vertexBindingDescriptions, vertexAttributeDescriptions);
		vkf::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
		PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(VK_FALSE, VK_FALSE,VK_POLYGON_MODE_LINE);
		PipelineViewportStateCreateInfo viewportStateInfo({ viewPortExtents.width, viewPortExtents.height });
		PipelineMultisampleStateCreateInfo multisampleinfo;
		std::vector<vkf::PipelineColorBlendAttachementState> colBlendAttachements = { vkf::PipelineColorBlendAttachementState() };
		PipelineColorBlendStateCreateInfo colblendInfo(colBlendAttachements);
		PipelineDepthStencilStateCreateInfo depthStencilpipelineinfo(VK_FALSE,VK_FALSE,VK_COMPARE_OP_NEVER);

		std::vector<PipelineShaderStageCreateInfo> shaderstages = shader.getShaderStages();

		std::shared_ptr<Pipeline> pipe = device->createPipeline(pipelineLayout, renderpass, 0, shaderstages, &vertexInputStateCreateInfo,
			&inputAssemblyStateCreateInfo, &rasterizationStateCreateInfo, &viewportStateInfo, &multisampleinfo, &colblendInfo, &depthStencilpipelineinfo);
		return pipe;
	}

	std::shared_ptr<Pipeline> PipelineManager::createCubemapPipeline(std::shared_ptr<Device> device, Shader shader,
		VkExtent2D viewPortExtents, std::shared_ptr<RenderPass> renderpass, std::shared_ptr<PipelineLayout> pipelineLayout) {

		std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions = { vkf::Vertex::getBindingDescription() };
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions = { vkf::Vertex::getAttributeDescriptions() };
		vkf::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo(vertexBindingDescriptions, vertexAttributeDescriptions);
		vkf::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
	    vkf::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL,
		VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f);
		vkf::PipelineViewportStateCreateInfo viewportStateInfo(viewPortExtents);
		vkf::PipelineMultisampleStateCreateInfo multisampleinfo;
		std::vector<vkf::PipelineColorBlendAttachementState> colBlendAttachements = { vkf::PipelineColorBlendAttachementState() };
		vkf::PipelineColorBlendStateCreateInfo colblendInfo(colBlendAttachements);
		vkf::PipelineDepthStencilStateCreateInfo depthStencilpipelineinfo(VK_TRUE,VK_TRUE,VK_COMPARE_OP_LESS);

		std::vector<PipelineShaderStageCreateInfo> shaderstages = shader.getShaderStages();

		return 	device->createPipeline(pipelineLayout, renderpass, 0, shaderstages, &vertexInputStateCreateInfo,
			&inputAssemblyStateCreateInfo, &rasterizationStateCreateInfo, &viewportStateInfo, &multisampleinfo, &colblendInfo, &depthStencilpipelineinfo);
	}

	std::shared_ptr<Pipeline> PipelineManager::createTessPipeline(std::shared_ptr<Device> device, Shader shader,
		VkExtent2D viewPortExtents, std::shared_ptr<RenderPass> renderpass, std::shared_ptr<PipelineLayout> pipelineLayout,uint32_t patchPoints) {

		VkPipelineTessellationStateCreateInfo tessInfo;
		tessInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		tessInfo.pNext = nullptr;
		tessInfo.flags = NULL;
		tessInfo.patchControlPoints = patchPoints;

		std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions = { vkf::Vertex::getBindingDescription() };
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions = { vkf::Vertex::getAttributeDescriptions() };
		vkf::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo(vertexBindingDescriptions, vertexAttributeDescriptions);
		vkf::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_PATCH_LIST);
		PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_LINE);
		PipelineViewportStateCreateInfo viewportStateInfo({ viewPortExtents.width, viewPortExtents.height });
		PipelineMultisampleStateCreateInfo multisampleinfo;
		std::vector<vkf::PipelineColorBlendAttachementState> colBlendAttachements = { vkf::PipelineColorBlendAttachementState() };
		PipelineColorBlendStateCreateInfo colblendInfo(colBlendAttachements);
		PipelineDepthStencilStateCreateInfo depthStencilpipelineinfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS);
		std::vector<PipelineShaderStageCreateInfo> shaderstages = shader.getShaderStages();
		std::shared_ptr<Pipeline> pipe = device->createPipeline(pipelineLayout, renderpass, 0, shaderstages, &vertexInputStateCreateInfo,
			&inputAssemblyStateCreateInfo, &rasterizationStateCreateInfo, &viewportStateInfo, &multisampleinfo, &colblendInfo, &depthStencilpipelineinfo,&tessInfo);
		return pipe;

	}
}


