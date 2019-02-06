#pragma once
#include "../../vkf/Instance.h"
#include "../../vkf/Device.h"
#include "../../vkf/Mesh.h"
#include "../../vkf/PipelineManager.h"
#include "../../vkf/Mesh.h"
#include "../../vkf/Shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Util.h"

struct uniformStuff {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};



struct tessObj {
	uniformStuff mvp;
	std::shared_ptr<vkf::DeskriptorSet> descriptorSet;
	std::shared_ptr<vkf::Texture> texture;
	std::shared_ptr<vkf::Buffer> uniformBuffer;
};

class App_2 {
public:
	App_2::App_2(std::shared_ptr<vkf::Window> window);

	void run();
private:
	void handleInput();

	void initDevice();
	void initRenderpass();
	void initFramebuffers();
	void initCmdBuffers();
	void initShaders();

	vkf::Instance instance;
	std::vector<VkPhysicalDevice> physicalDevices;
	vkf::PhysicalDevice activePhysicalDevice;
	std::vector<uint32_t> graphicsPresentQueueIndices;
	uint32_t graphicsQueueFamilyIndex;
	std::shared_ptr<vkf::Queue> renderQueue;
	std::shared_ptr<vkf::Device> device;
	std::shared_ptr<vkf::Window> window;

	std::shared_ptr<vkf::CommandPool> pool;
	//SwapchainFramebuffer
	std::shared_ptr<vkf::Swapchain> swapChain;
	std::vector<std::shared_ptr<vkf::Image>> swapchainImages;
	std::vector<std::shared_ptr<vkf::ImageView>> colorImageViews;
	std::shared_ptr<vkf::Image> depthImage;
	std::shared_ptr<vkf::ImageView> depthimageView;
	std::vector<std::shared_ptr<vkf::Framebuffer>> framebuffers;
	VkFormat depthFormat;

	std::vector<std::shared_ptr<vkf::CommandBuffer>> commandBuffers;

	//Pipeline independent
	std::shared_ptr<vkf::RenderPass> renderpass;
	vkf::PipelineManager pipelineManager;

	//phong pipeline
	vkf::Shader shaderSet;
	std::shared_ptr<vkf::DeskriptorPool> descriptorPool;
	std::shared_ptr<vkf::Pipeline> pipeline;
	std::shared_ptr<vkf::PipelineLayout> pipelineLayout;

	std::shared_ptr<vkf::DeskriptorSetLayout> deskriptorSetLayout;
	vkf::Mesh planeMesh;
	tessObj tessobj;
};