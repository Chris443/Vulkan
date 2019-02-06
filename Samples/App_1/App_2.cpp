#include "App_2.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

struct camera {
	glm::vec3 pos = glm::vec3(0.0f, 3.5f, 10.0f);
	glm::vec3 front = glm::vec3(0.0f, 8.0f, -4.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 256.0f);
	glm::mat4 view = glm::mat4(1.0f);
	bool firstMouse = true;
	float lastX;
	float lastY;
	float Yaw = YAW;
	float Pitch = PITCH;
}cam;


App_2::App_2(std::shared_ptr<vkf::Window> window)
{

	glfwSetCursorPosCallback(window->getGLFWWindow(), mouse_callback);
	glfwSetInputMode(window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	this->window = window;
	window->isResizable(true);
	initDevice();
	initRenderpass();
	initFramebuffers();
	initShaders();
	initCmdBuffers();
}

void App_2::initDevice() {
	vkf::LayerAndExtensions led;
	led.initBasicLayersAndExtensions();
	//create instance with global Layer & Extensions
	instance.createInstance(led.getEnabledInstanceLayers(), led.getEnabledInstanceExtensions(), "vkfFrame", "Demo");
	//initialize surface
	window->createSurface(instance.getInstance());

	//get Desired Physical Device
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, NULL);
	physicalDevices.resize(deviceCount);
	vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, physicalDevices.data());

	//init Physical Device Data
	activePhysicalDevice.initPhysicalDevice(physicalDevices[0]);
	//get (all) graphicsQueuefamilies Indices which support Graphics Operations & presentation
	graphicsPresentQueueIndices = activePhysicalDevice.getGraphicsPresentQueueIndices(window);
	graphicsQueueFamilyIndex = graphicsPresentQueueIndices[0];

	// activate Surface Support(Data) for Picked GPU
	activePhysicalDevice.initSurfaceData(window);
	//Initialize Queue by filling CreateInfos
	std::vector<vkf::DeviceQueueCreateInfo> deviceCreateInfos;
	if (graphicsPresentQueueIndices.size() > 0) {
		deviceCreateInfos.push_back(vkf::DeviceQueueCreateInfo(graphicsQueueFamilyIndex, { 1.0f }));
	}
	std::vector<std::string> allDeviceExtensions;

	std::vector<VkExtensionProperties> deviceExtensions = led.getSupportedDeviceExtensions(activePhysicalDevice.getPhysicalDevice());
	for (auto e : deviceExtensions) {
		if (std::string(e.extensionName) == std::string("VK_KHR_swapchain")) {
			allDeviceExtensions.push_back(e.extensionName);
			break;
		}
	}
	VkPhysicalDeviceFeatures enabledFeat = {};
	enabledFeat.samplerAnisotropy = VK_TRUE;
	enabledFeat.fillModeNonSolid = VK_TRUE;
	enabledFeat.tessellationShader = VK_TRUE;
	enabledFeat.geometryShader = VK_TRUE;

	//create device 
	device = vkf::Device::create(instance.getInstance(), deviceCreateInfos, activePhysicalDevice, allDeviceExtensions, led.getEnabledInstanceLayers(), enabledFeat);
	renderQueue = device->createQueue(graphicsQueueFamilyIndex, graphicsPresentQueueIndices[0]);

	pool = device->createCommandPool(graphicsQueueFamilyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);


	VkSurfaceFormatKHR SurfaceFormat = activePhysicalDevice.getSurfaceFormats()[0];
	std::vector<VkSurfaceFormatKHR> ss = activePhysicalDevice.getSurfaceFormats();
	swapChain = device->createSwapchain(window->getSurface(), activePhysicalDevice.getSurfaceCapabilities().minImageCount, SurfaceFormat.format, SurfaceFormat.colorSpace, window->getExtents(),
		1, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, { graphicsQueueFamilyIndex },
		activePhysicalDevice.getSurfaceCapabilities().currentTransform, VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, pickPresentationMode(activePhysicalDevice), VK_TRUE, nullptr);

	planeMesh.loadMesh("../../res/obj/plane.obj", device, renderQueue, pool);
}

void App_2::run() {
	std::shared_ptr<vkf::Semaphore> renderFinishedSemaphore = device->createSemaphore();
	glm::vec3 scale = glm::vec3(0.3f);
	auto startTime = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window->getGLFWWindow())) {
		auto frameTime = std::chrono::high_resolution_clock::now();

		float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime - startTime).count() / 1000.0f;
		cam.view = glm::lookAt(cam.pos, cam.pos + cam.front, cam.up);

		tessobj.mvp.model = glm::mat4(1.0f);
		tessobj.mvp.model = glm::scale(tessobj.mvp.model, glm::vec3(1.0));
		std::cout << scale.x << std::endl;
		//scale += glm::vec3(0.0001f);
		tessobj.mvp.model = glm::translate(tessobj.mvp.model, glm::vec3(0.0f, 5.5f, 7.0f));
		tessobj.mvp.model = glm::rotate(tessobj.mvp.model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		tessobj.mvp.model = glm::rotate(tessobj.mvp.model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//	tessobj.mvp.model = glm::rotate(tessobj.mvp.model, glm::radians(30.0f) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
	//	tessobj.mvp.model = glm::rotate(tessobj.mvp.model, glm::radians(10.0f) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		//tessobj.mvp.model = glm::rotate(tessobj.mvp.model, glm::radians(90.0f) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		tessobj.mvp.view = cam.view;
		tessobj.mvp.projection = cam.proj;
		tessobj.uniformBuffer->map(&tessobj.mvp);



		vkf::swapChainImageData swpimgdata;
		swpimgdata = swapChain->acquireNextImage(ULLONG_MAX);

	//	if (swpimgdata.result == VK_ERROR_OUT_OF_DATE_KHR) {
		//	recreateSwapChain();
		//}

		vkf::SubmitInfo queueSubmitInfo({ swapChain->getPresentSemaphores()[swpimgdata.imageIndex] }, { commandBuffers[swpimgdata.imageIndex] }, { renderFinishedSemaphore }, { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT });

		renderQueue->submit({ queueSubmitInfo }, VK_NULL_HANDLE);
		VkResult res = renderQueue->present({ renderFinishedSemaphore }, { swapChain }, swpimgdata.imageIndex);
		//if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
		//recreateSwapChain();
		//}
		renderQueue->wait();

		handleInput();
		glfwPollEvents();
	}
}

void App_2::initRenderpass() {
	VkAttachmentDescription ColorattachmentDescription;
	ColorattachmentDescription.flags = 0;;
	ColorattachmentDescription.format = activePhysicalDevice.getSurfaceFormats()[0].format;
	ColorattachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	ColorattachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	ColorattachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	ColorattachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	ColorattachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	ColorattachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ColorattachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	depthFormat = findSupportedDepthFormat(activePhysicalDevice, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	VkAttachmentDescription depthAttachmentDescription;
	depthAttachmentDescription.flags = 0;;
	depthAttachmentDescription.format = depthFormat;
	depthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	std::vector<VkAttachmentDescription> attachments = { ColorattachmentDescription, depthAttachmentDescription };


	VkAttachmentReference attReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	attReference.attachment = 0;
	attReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentReference;
	depthAttachmentReference.attachment = 1;
	depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassdescription = {};
	subpassdescription.flags = 0;
	subpassdescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassdescription.inputAttachmentCount = 0;
	subpassdescription.pInputAttachments = nullptr;
	subpassdescription.colorAttachmentCount = 1;
	subpassdescription.pColorAttachments = &attReference;
	subpassdescription.pResolveAttachments = nullptr;
	subpassdescription.pDepthStencilAttachment = &depthAttachmentReference;
	subpassdescription.preserveAttachmentCount = 0;
	subpassdescription.pPreserveAttachments = nullptr;

	std::vector<VkSubpassDescription> subpassdescriptions = { subpassdescription };

	renderpass = device->createRenderpass(attachments, subpassdescriptions);
}

void App_2::initFramebuffers() {
	//Color Images (Swapchain Images)
	swapchainImages = swapChain->getImages();
	//Depth Image
	depthImage = device->createImage(VK_IMAGE_TYPE_2D, depthFormat, { window->getWidth(), window->getHeight(),1 }, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE, {}, VK_IMAGE_LAYOUT_UNDEFINED, activePhysicalDevice, 0);

	//Depth ImageView
	depthimageView = depthImage->createImageView(VK_IMAGE_VIEW_TYPE_2D, depthFormat, { VK_COMPONENT_SWIZZLE_IDENTITY }, { VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,0,1,0,1 });

	for (uint32_t i = 0; i < swapchainImages.size(); ++i) {
		colorImageViews.push_back(swapchainImages[i]->createImageView());
	}

	for (uint32_t i = 0; i < swapchainImages.size(); ++i) {
		std::vector<std::shared_ptr<vkf::ImageView>> attachments;
		attachments.push_back(colorImageViews[i]);
		attachments.push_back(depthimageView);
		framebuffers.push_back(device->createFramebuffer(renderpass, attachments, window->getExtents(), 1));
	}
}

void App_2::initCmdBuffers() {
	shaderSet.createShaders(device, "../../Shader/tessShader");
	pipeline = pipelineManager.createTessPipeline(device, shaderSet, { window->getWidth(),window->getHeight() }, renderpass, pipelineLayout,3);

	for (uint32_t size = 0; size < framebuffers.size(); size++) {
		commandBuffers.push_back(pool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
	}
	std::vector<VkClearValue> cols = { { 0.5f, 0.5f, 0.5f, 1.0f },{ 1.0f,0.0f } };

	//initCmdBuffers bzw recorden
	for (uint32_t size = 0; size < commandBuffers.size(); size++) {
		commandBuffers[size]->beginRecording();

		commandBuffers[size]->beginRenderPass(renderpass, framebuffers[size], { VkOffset2D{ 0,0 },VkExtent2D{ window->getWidth(),window->getHeight() } }, cols);

		commandBuffers[size]->bindVertexBuffer(0, 1, { planeMesh.getVertexBuffer() }, { 0 });
		commandBuffers[size]->bindIndexBuffer(planeMesh.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
		commandBuffers[size]->bindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, { tessobj.descriptorSet }, {});
		commandBuffers[size]->bindPipeline(pipeline);
		commandBuffers[size]->drawIndexed(planeMesh.getIndices().size(), 1, 0, 0, 0);


		commandBuffers[size]->endRenderPass();

		commandBuffers[size]->endRecording();
	}
}

void App_2::initShaders() {
	tessobj.texture = device->create2DTexture("../../res/obj/heightmap.png", renderQueue);

	//DeskriptorPool @ TODO: implement dynamic descriptor allocation
	VkDescriptorPoolSize uniformBufferPoolSize, combinedImageSamplerPoolSize;
	uniformBufferPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformBufferPoolSize.descriptorCount = 20;
	combinedImageSamplerPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	combinedImageSamplerPoolSize.descriptorCount = 20;

	descriptorPool = device->createDeskriptorPool(100, { uniformBufferPoolSize,combinedImageSamplerPoolSize }, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);

	VkDescriptorSetLayoutBinding uniformBufferBinding;
	uniformBufferBinding.binding = 0;
	uniformBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformBufferBinding.descriptorCount = 1;
	uniformBufferBinding.stageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VK_SHADER_STAGE_GEOMETRY_BIT;
	uniformBufferBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding samplerBinding;
	samplerBinding.binding = 1;
	samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerBinding.descriptorCount = 1;
	samplerBinding.stageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	samplerBinding.pImmutableSamplers = nullptr;

	std::vector <VkDescriptorSetLayoutBinding> bindings = { uniformBufferBinding,samplerBinding };
	deskriptorSetLayout = device->createDeskriptorSetLayout(bindings);

	tessobj.uniformBuffer = device->createBuffer(activePhysicalDevice.getMemoryProperties(), sizeof(tessobj.mvp), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE, {}, (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	tessobj.descriptorSet = device->createDeskriptorSet(descriptorPool, deskriptorSetLayout);
	VkDescriptorBufferInfo descriptorBufferInfo;
	descriptorBufferInfo.buffer = tessobj.uniformBuffer->getBuffer();
	descriptorBufferInfo.offset = 0;
	descriptorBufferInfo.range = sizeof(tessobj.mvp);

	VkDescriptorImageInfo imageInfo;
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = tessobj.texture->getImageView()->getImageView();
	imageInfo.sampler = tessobj.texture->getSampler()->getSampler();

	VkWriteDescriptorSet descriptorWrite_1;
	descriptorWrite_1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite_1.pNext = 0;
	descriptorWrite_1.dstSet = tessobj.descriptorSet->getDescriptorSet();
	descriptorWrite_1.dstBinding = 0;
	descriptorWrite_1.dstArrayElement = 0;
	descriptorWrite_1.descriptorCount = 1;
	descriptorWrite_1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite_1.pBufferInfo = &descriptorBufferInfo;

	VkWriteDescriptorSet descriptorWrite_2;
	descriptorWrite_2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite_2.pNext = 0;
	descriptorWrite_2.dstSet = tessobj.descriptorSet->getDescriptorSet();
	descriptorWrite_2.dstBinding = 1;
	descriptorWrite_2.dstArrayElement = 0;
	descriptorWrite_2.descriptorCount = 1;
	descriptorWrite_2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite_2.pImageInfo = &imageInfo;

	std::vector<VkWriteDescriptorSet> writes = { descriptorWrite_1,descriptorWrite_2 };
	tessobj.descriptorSet->deskriptorWrite(writes);


	pipelineLayout = device->createPipelineLayout({ deskriptorSetLayout});


}

void App_2::handleInput() {
	int state_W = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_W);
	int state_A = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_A);
	int state_S = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_S);
	int state_D = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_D);
	int state_Space = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_SPACE);
	int state_E = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_E);
	float speed = 0.001f;
	if (state_W == GLFW_PRESS) {
		cam.pos += speed * cam.front;
	}if (state_A == GLFW_PRESS) {
		cam.pos -= glm::normalize(glm::cross(cam.front, cam.up)) * speed;
	}if (state_S == GLFW_PRESS) {
		cam.pos -= speed * cam.front;
	}if (state_D == GLFW_PRESS) {
		cam.pos += glm::normalize(glm::cross(cam.front, cam.up)) * speed;
	}
	if (state_Space == GLFW_PRESS) {
		cam.pos.y += 1.0f * speed;
	}
	if (state_E == GLFW_PRESS) {
		cam.pos.y -= 1.0f * speed;
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (cam.firstMouse)
	{
		cam.lastX = (float)xpos;
		//lastY = ypos;
		cam.firstMouse = false;
	}
	float xoffset = (float)xpos - cam.lastX;
	std::cout << xoffset << std::endl;
	float yoffset = cam.lastY - ypos; // reversed since y-coordinates go from bottom to top

	cam.lastX = (float)xpos;
	cam.lastY = ypos;

	xoffset *= 0.01f;
	yoffset *= 0.01f;

	cam.Yaw += xoffset;
	cam.Pitch += yoffset;

	// Update Front, Right and Up Vectors using the updated Euler angles
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(cam.Yaw)) * cos(glm::radians(cam.Pitch));
	front.y = sin(glm::radians(cam.Pitch));
	front.z = sin(glm::radians(cam.Yaw)) * cos(glm::radians(cam.Pitch));
	cam.front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	//Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	//Up = glm::normalize(glm::cross(Right, Front));
}