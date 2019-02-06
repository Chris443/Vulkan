#pragma once
#include "../../vkf/Mesh.h"

#include "../../vkf/Instance.h"
#include "../../vkf/Device.h"
#include "../../vkf/Texture.h"
#include "../../vkf/PipelineManager.h"
#include "../../vkf/Shader.h"
#include "../../vkf/ShaderRessourceManager.h"
#include "../../vkf/Util.h"

#include <chrono>
#include "glm/gtc/matrix_transform.hpp"
#include <array>

struct MVPuniformBuffer {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 lightPosition;
};


struct Apple {
	MVPuniformBuffer mvp;
	std::shared_ptr<vkf::DeskriptorSet> descriptorSet;
	std::shared_ptr<vkf::Texture> texture;
	std::shared_ptr<vkf::Buffer> uniformBuffer;
	std::shared_ptr<vkf::Buffer> materialbuffer;
};

struct cubeMapUniformBuffer {
	glm::mat4 model;
	glm::mat4 projection;
};
struct cubeMap {
	cubeMapUniformBuffer cubeMapMats;
	std::shared_ptr<vkf::DeskriptorSet> descriptorSet;
	std::shared_ptr<vkf::Buffer> uniformBuffer;
	std::shared_ptr<vkf::Texture> texture;
};

	class Application
	{
	public:
		Application(std::shared_ptr<vkf::Window> window);
		void run();
	private:
		void initDevice();
		void initRenderpass();
		void initFramebuffers();
		void initShaders();
		void initCmdBuffers();
		void recreateSwapChain();
		void handleInput();

		void demo_one();
		//Setup
		std::shared_ptr<vkf::Window> window;
		vkf::Instance instance;
		vkf::LayerAndExtensions led;
		std::shared_ptr<vkf::Device> device;
		//Physical Device
		std::vector<VkPhysicalDevice> physicalDevices;
		vkf::PhysicalDevice activePhysicalDevice;
		std::vector<uint32_t> graphicsPresentQueueIndices;
		uint32_t graphicsQueueFamilyIndex;
		std::shared_ptr<vkf::Queue> renderQueue;


		//uniform and textureBuffer
		std::shared_ptr<vkf::Buffer> uniformBuffer;

		//SwapchainFramebuffer
		std::shared_ptr<vkf::Swapchain> swapChain;
		std::vector<std::shared_ptr<vkf::Image>> swapchainImages;
		std::vector<std::shared_ptr<vkf::ImageView>> colorImageViews;
		std::shared_ptr<vkf::Image> depthImage;
		std::shared_ptr<vkf::ImageView> depthimageView;
		std::vector<std::shared_ptr<vkf::Framebuffer>> framebuffers;
		VkFormat depthFormat;
		
		//Pool&Buffer
		std::shared_ptr<vkf::CommandPool> pool;
		std::vector<std::shared_ptr<vkf::CommandBuffer>> commandBuffers;

		//objects and ressources
		vkf::Mesh appleMesh;
		vkf::Mesh cubemapMesh;
		Apple apple;

		//Pipeline independent
		std::shared_ptr<vkf::RenderPass> renderpass;
		vkf::PipelineManager pipelineManager;

		//phong pipeline
		vkf::Shader shaderSet1;
		std::shared_ptr<vkf::DeskriptorPool> descriptorPool;
		std::shared_ptr<vkf::Pipeline> pipeline;
		std::shared_ptr<vkf::PipelineLayout> pipelineLayout;
		std::array<vkf::ShaderRessourceManager,4> shadermanager;

		//cubemapPipeline
		vkf::Shader cubemapShaderSet;
		std::shared_ptr<vkf::Pipeline> cubeMapPipeline;
		std::shared_ptr<vkf::PipelineLayout> cubeMapPipelineLayout;
		vkf::ShaderRessourceManager cubemapShadermanager;
		cubeMap cubemap;

		std::shared_ptr<vkf::DeskriptorSetLayout> layout;
		void setup();
	};
