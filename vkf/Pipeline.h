#pragma once
#include "common.h"
#include "ShaderModule.h"

namespace vkf {
	class Device;
	class RenderPass;
	class PipelineLayout;

	/**
	* \brief Wrapper structure of VkPipelineVertexInputStateCreateInfo. Initializes the corresponding Createinfo.
	*/
	struct PipelineVertexInputStateCreateInfo{
		/**
		* \brief Wrapper structure of VkPipelineVertexInputStateCreateInfo. Initializes the corresponding Createinfo.
		* \param vertexBindingDescriptions A vector of VkVertexInputBindingDescription, which describes the Bindings of the vertices
		* \param vertexAttributeDesriptions A Vector of vertexAttributeDescriptions, which describes the Attributes of the vertices
		* \note An Example of how to use the VertexDescriptions is demonstraed in the Demo Vertex.h
		*/
		PipelineVertexInputStateCreateInfo(std::vector<VkVertexInputBindingDescription> &vertexBindingDescriptions ,
			std::vector<VkVertexInputAttributeDescription> &vertexAttributeDesriptions);
		VkPipelineVertexInputStateCreateInfo createInfo;
	};
	
	/**
	* \brief Wrapper structure of VkPipelineInputAssemblyStateCreateInfo. Initializes the corresponding Createinfo.
	*/
	struct PipelineInputAssemblyStateCreateInfo{
		/**
		* \brief Wrapper structure of VkPipelineInputAssemblyStateCreateInfo. Initializes the corresponding Createinfo.
		* \param topology Defines how the Vertices should be connected. i.e. VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST for the default Triangle Topology
		* \param primitiveRestartEnable Controls whether a special vertex index value is treated as restarting the assembly of primitives.
		*/
		PipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VkBool32 primitiveRestartEnable = VK_FALSE);
		VkPipelineInputAssemblyStateCreateInfo createInfo;
		};
	/**
	* \brief Wrapper structure of VkPipelineShaderStageCreateInfo. Initializes the corresponding Createinfo.
	*/
	struct PipelineShaderStageCreateInfo{
		/**
		* \brief Wrapper structure of VkPipelineShaderStageCreateInfo. Initializes the corresponding Createinfo.
		* \param stage Specifies which kind of Shader should be used. i.e. vertex,fragment,geometry...
		* \param shaderModule The ShaderModule which should be used with this ShaderStageCreateInfo
		* \param startName The entry point of the Shader to this Stage
		* \param pSpec A pointer to a VkSpecializationInfo.
		*/
		PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage,
			std::shared_ptr<ShaderModule> shaderModule, std::string const& startName = "main", VkSpecializationInfo* pSpec = nullptr);
		std::string name;
		VkPipelineShaderStageCreateInfo createInfo;
	};
	/**
	* \brief Wrapper structure of VkPipelineRasterizationStateCreateInfo. Initializes the corresponding Createinfo.
	*/
	struct PipelineRasterizationStateCreateInfo {
		/**
		* \brief Wrapper structure of VkPipelineRasterizationStateCreateInfo. Initializes the corresponding Createinfo. The default values specify a very basic Rasterizer which can be used for quick development/prototyping.
		* \param depthClampEnable If enabled, the fragments Depth values are clampes to the z plane
		* \param rasterizerDiscardEnable If enabled, primitives are discarded before rasterization stage
		* \param polygonMode Defines the Triangle rendering Mode
		* \param cullmode Defines the Triangle Direction  used for culling
		* \param frontface Defines the Front Face orientation of a Triangle. 
		* \param depthBiasEnable If enabled, fragment depth values are biased
		* \param depthBiasConstantFactor The constant depth value added to each fragment
		* \param depthBiasClamp the maximum or minimum depth bias of a fragment
		* \param depthBiasSlopeFactor A scalar factor used in the in depth bias calculations.
		* \param lineWidth The width of the rasterized lines
		*/
		PipelineRasterizationStateCreateInfo(VkBool32 depthClampEnable = VK_FALSE, VkBool32 rasterizerDiscardEnable = VK_FALSE, VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL,
			VkCullModeFlags cullmode = VK_CULL_MODE_BACK_BIT, VkFrontFace frontface = VK_FRONT_FACE_COUNTER_CLOCKWISE, VkBool32 depthBiasEnable = VK_FALSE,
			float depthBiasConstantFactor = 0.0f, float depthBiasClamp = 0.0f, float depthBiasSlopeFactor = 0.0f, float lineWidth = 1.0f);

		VkPipelineRasterizationStateCreateInfo createInfo;
	};
	/**
	* \brief Wrapper structure of VkPipelineViewportStateCreateInfo. Initializes the corresponding Createinfo.
	*/
	struct PipelineViewportStateCreateInfo {
		/**
		* \brief Constructor if custom viewports and scissors should be used.
		* \param viewport A vector of viewports which should be used
		* \param scissors A vector of scissors which should be used
		*/
		PipelineViewportStateCreateInfo(std::vector<VkViewport> viewport, std::vector<VkRect2D> scissors);
		/**
		* \brief A simplified constructor if you only want to use one viewport without scissors. The Window extents still have to be passed.
		* \param viewPortMaxima The Window extents which should be used to initialize the Viewport
		*/
		PipelineViewportStateCreateInfo(VkExtent2D viewPortMaxima);

		VkPipelineViewportStateCreateInfo createInfo;
		std::vector<VkViewport> m_viewports;
		std::vector<VkRect2D> m_scissors;
	};
	/**
	* \brief Wrapper structure of VkPipelineMultisampleStateCreateInfo. Initializes the corresponding Createinfo.
	*/
	struct PipelineMultisampleStateCreateInfo {
		/**
		* \brief Used to initializes the VkPipelineMultisampleStateCreateInfo with given parameters. The default values define that no multisampling will be used.
		* \param rasterizationSamples Specifies the number of samples per pixel which should be used for rasterization
		* \param sampleShadingEnable If enabled, Sample Shading can be used
		* \param minSampleShading If sampleShadingEnable is set to True, this specifies the minimum fraction of sample Shading.
		* \param sampleMask A Bitmask which used for coverage Information during rasterization(See Vulkan Documentation for more Information)
		* \param alphaToCoverageEnable If enabled, a temporary coverage value is generated based on the alpha component of the fragment’s first color output(See Vulkan Documentation for more Information)
		* \param alphaToOneEnable If enabled, the alpha component of the fragment’s first color output is replaced(See Vulkan Documentation for more Information)
		*/
		PipelineMultisampleStateCreateInfo(VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT, VkBool32 sampleShadingEnable = VK_FALSE,
			float minSampleShading = 1.0f, const VkSampleMask* sampleMask = nullptr, VkBool32 alphaToCoverageEnable = VK_FALSE, VkBool32 alphaToOneEnable = VK_FALSE);
		VkPipelineMultisampleStateCreateInfo createInfo;
	};
	/**
	* \brief Wrapper structure of VkPipelineColorBlendAttachmentState. Initializes the corresponding Createinfo.
	*/
	struct PipelineColorBlendAttachementState {
		/**
		* \brief Used to initialize the VkPipelineColorBlendAttachmentState with given parameters. Default values are set for a basic colorBlendAttachmentState to use. 
		* For more Information, see the Vulkan Documentation for VkPipelineColorBlendAttachmentState.
		* \param blendEnable Specifies if Blending should be enabled. If disabled, the fragments color is passed unchanged.
		* \param srcColFactor Value to specify which Blend factor should be used to determine the source factor
		* \param dstColFactor Value to specify which Blend factor should be used to determine the destination factor
		* \param colBlendOp Defines which blend operation should be used
		* \param srcAlphaFactor Selects the Blend Factor which should be used to determine the source factor
		* \param dstAlphaFactor Selects the Blend Factor which should be used to determine the destination factor
		* \param alphaOp Defines which blend operation for alpha values should be used
		* \param colorWriteMask Specifies which of the R,G,B,A values should be enabled for writing
		*/
		PipelineColorBlendAttachementState(VkBool32 blendEnable = VK_TRUE, VkBlendFactor srcColFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			VkBlendFactor dstColFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VkBlendOp colBlendOp = VK_BLEND_OP_ADD,
		VkBlendFactor srcAlphaFactor = VK_BLEND_FACTOR_ONE, VkBlendFactor dstAlphaFactor = VK_BLEND_FACTOR_ZERO,
			VkBlendOp alphaOp = VK_BLEND_OP_ADD, VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

		VkPipelineColorBlendAttachmentState attachement;
	};

	/**
	* \brief Wrapper structure of VkPipelineColorBlendStateCreateInfo. Initializes the corresponding Createinfo.
	*/
	struct PipelineColorBlendStateCreateInfo {
		/**
		* \brief Used to initialize the VkPipelineColorBlendStateCreateInfo with given parameters. Default values are set for a basic PipelineColorBlendStateCreateInfo to use.
		* \param attachments a vector of PipelineColorBlendAttachementState which should be used
		* \param logicOp If VK_LOGIC_OP_NO_OP is set, no logical operations are supported, otherwise this variable defines what kind of operations should be supported
		* \param blendConst_1 BlendFactor R
		* \param blendConst_2 BlendFactor G
		* \param blendConst_3 BlendFactor B
		* \param blendConst_4 BlendFactor A
		*/
		PipelineColorBlendStateCreateInfo(std::vector<PipelineColorBlendAttachementState> attachments = {},VkLogicOp logicOp = VK_LOGIC_OP_NO_OP,
			float blendConst_1 = 0.0f, float blendConst_2 = 0.0f, float blendConst_3 = 0.0f, float blendConst_4=0.0f);
		std::vector<VkPipelineColorBlendAttachmentState> attachements;
		VkPipelineColorBlendStateCreateInfo createInfo;
	};
	/**
	* \brief Wrapper structure of VkPipelineDepthStencilStateCreateInfo. Initializes the corresponding Createinfo.
	*/
	struct PipelineDepthStencilStateCreateInfo {
		/**
		* \brief Used to initialize the VkPipelineDepthStencilStateCreateInfo. Initializes the corresponding Createinfo.
		* If a PipelineDepthStencilStateCreateInfo is used, the default values enable Depth testing with basic values, i.e. Depth Testing from 0 to 1. For more Information, see the Vulkan Documentation for VkPipelineDepthStencilStateCreateInfo.
		* \param depthTestEnable Enables or disables Depth testing.
		* \param depthWriteEnable Enables or disables Depth Writing.
		* \param depthCompareOp the operator which should be used for depth Testing
		* \param depthBoundsTestEnable Enables or disables depth bound testing 
		* \param stencilTestEnable Enables or disables stencil testing 
		* \param front The front for Stencil testing
		* \param back The back for Stencil Testing
		* \param minDepthBounds minimum Range value for depth testing
		* \param maxDepthBounds maximum Range value for depth testing
		*/
		PipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable = VK_TRUE, VkBool32 depthWriteEnable = VK_FALSE, VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS,VkBool32 depthBoundsTestEnable = VK_FALSE
			, VkBool32 stencilTestEnable = VK_FALSE, VkStencilOpState front = {}, VkStencilOpState back = {}, float minDepthBounds = 0.0f, float maxDepthBounds = 1.0f);

		VkPipelineDepthStencilStateCreateInfo createInfo;
	};

	/*
	struct PipelineDynamicStateCreateInfo {
		PipelineDynamicStateCreateInfo();
		VkPipelineDynamicStateCreateInfo createInfo;
	};*/


	/**
	* \brief The Pipeline class encapsulates the VkPipeline Handle and takes care of initialization and deleting. For initialization, use the Device.createPipeline() function inside the Device class.
	*/
	class Pipeline
	{
	public:
		/**
		* \brief Used to create a Pipeline. Use the device.createPipeline() function inside the Device class instead of this constructor to create a Pipeline.
		* \param device The device which should create the Pipeline
		* \param pipelineLayout The Pipelinelayout which describes the Pipeline
		* \param renderpass The renderpass which should be used with this Pipeline
		* \param subpass The index of the subpass in the renderpass which should be used
		* \param shaderstages The amount of ShaderStages.
		* \param vertexInputStateCreateInfo A Pointer to a PipelineVertexInputStateCreateInfo.
		* \param inputAssemblyStateCreateInfo A pointer to a PipelineInputAssemblyStateCreateInfo, which describes the pipelinebehaviour at the input assembly state.
		* \param rasterizationStateCreateInfo A pointer to a PipelineRasterizationStateCreatInfo, which describes the pipelinebehaviour at the rasterization state.
		* \param viewportStateCreateInfo A pointer to a PipelineViewportStateCreateInfo, ignored if rasterizer is disabled.
		* \param multisampleinfo A pointer to a PipelineMultisampleStateCreateInfo, ignored if rasterizer is disabled .
		* \param colorBlendStateCreateInfo A pointer to a PipelineColorBlendStateCreateInfo, ignored if rasterizer is disabled or subpass has no support.
		* \param depthStencilCreateInfo A pointer to a PipelineDepthStencilStateCreateInfo, which is used for depth/stencil testing. Ignored if the Pipeline has rasterization disables or the subpass doesnt support depth/stencil testing.
		* \param tessellationStateCreateInfo A pointer to a VkPipelineTessellationStateCreateInfo, which describes if a Tesselationshader should be used and if so, how.
		* \param dynamicStateCreateInfo A pointer to a VkPipelineDynamicStateCreateInfo, which describes which properties of the pipeline are dynamic and can be changed independently of the pipeline state.
		* \param basePipelineHandle A pipeline to derive from.
		* \param basePipelineIndex The index of the basepipeline Handle creatinfo. Used for faster Pipeline creation.
		* \param pipelineCache The PipelineCache which can be used for creation.
		*/
		Pipeline(
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
			VkPipelineCache pipelineCache);
		~Pipeline();
		/**
		* \brief Getter function to retrieve the Vulkan Pipeline Handle of this class.
		* \return The Vulkan Pipeline Handle of this class
		*/
		VkPipeline getPipeline() { return pipeline; }
	private:
		VkPipeline pipeline;
		std::shared_ptr<Device> device;
	};

}

