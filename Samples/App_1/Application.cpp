#include "Application.h"

struct cam {
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 256.0f);
	glm::mat4 view = glm::mat4(1.0f);
}cam;



Application::Application(std::shared_ptr<vkf::Window> window)
{
	this->window = window;
	window->isResizable(true);
	setup();
}

void Application::setup() {
	initDevice();
	initRenderpass();
	initFramebuffers();
	initShaders();
	initCmdBuffers();
}

void Application::run() {
	std::shared_ptr<vkf::Semaphore> renderFinishedSemaphore = device->createSemaphore();;

	auto startTime = std::chrono::high_resolution_clock::now();
	glm::vec3 lightpos = glm::vec3(0.0f, 0.0f, 30.0f);
	while (!glfwWindowShouldClose(window->getGLFWWindow())) {
		cam.view = glm::lookAt(cam.pos, cam.pos + cam.front, cam.up);
		//cam.view = glm::rotate(cam.view, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		auto frameTime = std::chrono::high_resolution_clock::now();

		float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime - startTime).count() / 1000.0f;

		lightpos.x = cos(deltaTime) * 20;
		lightpos.z = sin(deltaTime) * 20;
		apple.mvp.model = glm::mat4(1.0f);
		apple.mvp.model = glm::scale(apple.mvp.model, glm::vec3(0.05f));
		apple.mvp.model = glm::translate(apple.mvp.model, glm::vec3(0.0f, -100.0f, 0.0f));
		apple.mvp.model = glm::rotate(apple.mvp.model, glm::radians(90.0f) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		apple.mvp.view = cam.view;
		apple.mvp.projection = cam.proj;
		apple.mvp.lightPosition = lightpos;
		apple.uniformBuffer->map(&apple.mvp);


		cubemap.cubeMapMats.model = glm::mat4(1.0f);
		cubemap.cubeMapMats.model = glm::translate(cubemap.cubeMapMats.model, glm::vec3(0, 1.0f, 1.5f));
		cubemap.cubeMapMats.model = glm::scale(cubemap.cubeMapMats.model, glm::vec3(50.0f));
		cubemap.cubeMapMats.projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 256.0f) * cam.view;

		cubemap.uniformBuffer->map(&cubemap.cubeMapMats);

		vkf::swapChainImageData swpimgdata;
		swpimgdata = swapChain->acquireNextImage(ULLONG_MAX);

		if (swpimgdata.result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
		}

		vkf::SubmitInfo queueSubmitInfo({ swapChain->getPresentSemaphores()[swpimgdata.imageIndex] }, { commandBuffers[swpimgdata.imageIndex] }, { renderFinishedSemaphore }, { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT });

		renderQueue->submit({ queueSubmitInfo }, VK_NULL_HANDLE);
		VkResult res = renderQueue->present({ renderFinishedSemaphore }, { swapChain }, swpimgdata.imageIndex);
		if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
			recreateSwapChain();
		}
		renderQueue->wait();

		handleInput();
		glfwPollEvents();
	}
}

void Application::recreateSwapChain() {
	int width, height;
	glfwGetWindowSize(window->getGLFWWindow(), &width, &height);
	if (width <= 0) width = 1;
	if (height <= 0) height = 1;
	window->recreate(width, height);
	activePhysicalDevice.initSurfaceData(window);

	VkSurfaceFormatKHR SurfaceFormat = activePhysicalDevice.getSurfaceFormats()[0];

	std::shared_ptr<vkf::Swapchain> newSwapchain = device->createSwapchain(window->getSurface(), activePhysicalDevice.getSurfaceCapabilities().minImageCount, SurfaceFormat.format, SurfaceFormat.colorSpace, window->getExtents(),
		1, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, { graphicsQueueFamilyIndex },
		activePhysicalDevice.getSurfaceCapabilities().currentTransform, VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, pickPresentationMode(activePhysicalDevice), VK_TRUE, swapChain);

	swapChain = newSwapchain;

	renderQueue->wait();
	pool->reset(VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);

	initRenderpass();
	framebuffers.clear();
	colorImageViews.clear();
	initFramebuffers();
	commandBuffers.clear();
	initShaders();
	initCmdBuffers();
}

void Application::initDevice() {
	//init global layers and extensions
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
	else { std::cerr << "No QueueFamily with Graphics and Presentation Supporter available" << std::endl; }
	//initialize device Extensions (i.e. Swapchain)
	std::vector<std::string> allDeviceExtensions;

	std::vector<VkExtensionProperties> deviceExtensions = led.getSupportedDeviceExtensions(activePhysicalDevice.getPhysicalDevice());
	for (auto e : deviceExtensions) {
		if (std::string(e.extensionName) == std::string("VK_KHR_swapchain")) {
			std::cout << "Swapchain Support enabled" << std::endl;
			allDeviceExtensions.push_back(e.extensionName);
			break;
		}
	}

	/***************************************Printing Layers&Extensions**************************************/
	std::cout << "Enabled Instance - Layers: " << std::endl;
	for (auto l : led.getEnabledInstanceLayers()) {
		std::cout << "Layer: " << l.c_str() << std::endl;
	}
	std::cout << "Enabled Instance - Extensions: " << std::endl;
	for (auto e : led.getEnabledInstanceExtensions()) {
		std::cout << "Extension: " << e.c_str() << std::endl;
	}

	std::cout << "Enabled Device - Extensions:  " << std::endl;
	for (auto e : allDeviceExtensions) {
		std::cout << e.c_str() << std::endl;
	}
	/***************************************Printing Layers&Extensions**************************************/
	VkPhysicalDeviceFeatures enabledFeat = {};
	enabledFeat.samplerAnisotropy = VK_TRUE;
	enabledFeat.fillModeNonSolid = VK_TRUE;
	enabledFeat.tessellationShader = VK_TRUE;

	//create device 
	device = vkf::Device::create(instance.getInstance(), deviceCreateInfos, activePhysicalDevice, allDeviceExtensions, led.getEnabledInstanceLayers(), enabledFeat);
	renderQueue = device->createQueue(graphicsQueueFamilyIndex, graphicsPresentQueueIndices[0]);

	pool = device->createCommandPool(graphicsQueueFamilyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);


	VkSurfaceFormatKHR SurfaceFormat = activePhysicalDevice.getSurfaceFormats()[0];
	std::vector<VkSurfaceFormatKHR> ss = activePhysicalDevice.getSurfaceFormats();
	swapChain = device->createSwapchain(window->getSurface(), activePhysicalDevice.getSurfaceCapabilities().minImageCount, SurfaceFormat.format, SurfaceFormat.colorSpace, window->getExtents(),
		1, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, { graphicsQueueFamilyIndex },
		activePhysicalDevice.getSurfaceCapabilities().currentTransform, VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, pickPresentationMode(activePhysicalDevice), VK_TRUE, nullptr);

	appleMesh.loadMesh("../../res/obj/apple/apple.obj", device, renderQueue, pool);
	cubemapMesh.loadMesh("../../res/obj/cube.obj",device,renderQueue,pool);
}

void Application::initRenderpass() {
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

void Application::initFramebuffers() {
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

void Application::initCmdBuffers() {
	shaderSet1.createShaders(device, "../../Shader/basicShader");
	pipeline = pipelineManager.createBasicPipeline(device, shaderSet1, { window->getWidth(),window->getHeight() }, renderpass, pipelineLayout);
	cubemapShaderSet.createShaders(device, "../../Shader/cubemapShader");
	cubeMapPipeline = pipelineManager.createCubemapPipeline(device, cubemapShaderSet, { window->getWidth(),window->getHeight() }, renderpass, cubeMapPipelineLayout);


	for (uint32_t size = 0; size < framebuffers.size(); size++) {
		commandBuffers.push_back(pool->allocateBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
	}
	std::vector<VkClearValue> cols = { { 0.5f, 0.5f, 0.5f, 1.0f },{ 1.0f,0.0f } };

	//initCmdBuffers bzw recorden
	for (uint32_t size = 0; size < commandBuffers.size(); size++) {
		commandBuffers[size]->beginRecording();

		commandBuffers[size]->beginRenderPass(renderpass, framebuffers[size], { VkOffset2D{ 0,0 },VkExtent2D{ window->getWidth(),window->getHeight() } }, cols);

		commandBuffers[size]->bindVertexBuffer(0, 1, { appleMesh.getVertexBuffer() }, { 0 });
		commandBuffers[size]->bindIndexBuffer(appleMesh.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
	    commandBuffers[size]->bindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, { apple.descriptorSet }, {});
		commandBuffers[size]->bindPipeline(pipeline);
		commandBuffers[size]->drawIndexed(appleMesh.getIndices().size(), 1, 0, 0, 0);

		commandBuffers[size]->bindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, cubeMapPipelineLayout, 0, { cubemap.descriptorSet }, {});
		commandBuffers[size]->bindVertexBuffer(0, 1, { cubemapMesh.getVertexBuffer() }, { 0 });
		commandBuffers[size]->bindIndexBuffer(cubemapMesh.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
		commandBuffers[size]->bindPipeline(cubeMapPipeline);
		commandBuffers[size]->drawIndexed(cubemapMesh.getIndices().size(), 1, 0, 0, 0);

		commandBuffers[size]->endRenderPass();

		commandBuffers[size]->endRecording();
	}
}

void Application::initShaders() {
	apple.texture = device->create2DTexture("../../res/obj/apple/appleD.jpg", renderQueue);

	cubemap.texture = device->createCubemapTexture("../../res/tex/cubemap", renderQueue);


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
	uniformBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uniformBufferBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding samplerBinding;
	samplerBinding.binding = 1;
	samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerBinding.descriptorCount = 1;
	samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding uniformBufferBinding2;
	uniformBufferBinding2.binding = 2;
	uniformBufferBinding2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformBufferBinding2.descriptorCount = 1;
	uniformBufferBinding2.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	uniformBufferBinding2.pImmutableSamplers = nullptr;

	std::vector <VkDescriptorSetLayoutBinding> bindings = { uniformBufferBinding,samplerBinding,uniformBufferBinding2 };
	layout = device->createDeskriptorSetLayout(bindings);

	apple.materialbuffer = device->createBuffer(device->getPhysicalDevice()->getMemoryProperties(), sizeof(appleMesh.getMaterial().materials.size()),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, {}, (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

	apple.materialbuffer->map(appleMesh.getMaterial().materials.data());

	apple.uniformBuffer = device->createBuffer(activePhysicalDevice.getMemoryProperties(), sizeof(apple.mvp), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE, {}, (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	apple.descriptorSet = device->createDeskriptorSet(descriptorPool, layout);
	VkDescriptorBufferInfo descriptorBufferInfo;
	descriptorBufferInfo.buffer = apple.uniformBuffer->getBuffer();
	descriptorBufferInfo.offset = 0;
	descriptorBufferInfo.range = sizeof(apple.mvp);

	VkDescriptorImageInfo imageInfo;
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = apple.texture->getImageView()->getImageView();
	imageInfo.sampler = apple.texture->getSampler()->getSampler();

	VkDescriptorBufferInfo descriptorBufferInfodue;
	descriptorBufferInfodue.buffer = apple.materialbuffer->getBuffer();
	descriptorBufferInfodue.offset = 0;
	descriptorBufferInfodue.range = 4;

	VkWriteDescriptorSet descriptorWrite_1;
	descriptorWrite_1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite_1.pNext = 0;
	descriptorWrite_1.dstSet = apple.descriptorSet->getDescriptorSet();
	descriptorWrite_1.dstBinding = 0;
	descriptorWrite_1.dstArrayElement = 0;
	descriptorWrite_1.descriptorCount = 1;
	descriptorWrite_1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite_1.pBufferInfo = &descriptorBufferInfo;

	VkWriteDescriptorSet descriptorWrite_2;
	descriptorWrite_2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite_2.pNext = 0;
	descriptorWrite_2.dstSet = apple.descriptorSet->getDescriptorSet();
	descriptorWrite_2.dstBinding = 1;
	descriptorWrite_2.dstArrayElement = 0;
	descriptorWrite_2.descriptorCount = 1;
	descriptorWrite_2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite_2.pImageInfo = &imageInfo;

	VkWriteDescriptorSet descriptorWrite_3;
	descriptorWrite_3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite_3.pNext = 0;
	descriptorWrite_3.dstSet = apple.descriptorSet->getDescriptorSet();;
	descriptorWrite_3.dstBinding = 2;
	descriptorWrite_3.dstArrayElement = 0;
	descriptorWrite_3.descriptorCount = 1;
	descriptorWrite_3.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite_3.pBufferInfo = &descriptorBufferInfodue;
	std::vector<VkWriteDescriptorSet> writes = { descriptorWrite_1,descriptorWrite_2,descriptorWrite_3 };
	apple.descriptorSet->deskriptorWrite(writes);


	pipelineLayout = device->createPipelineLayout({ layout });

	//cubemap pipeline shadersetup
	cubemap.uniformBuffer = device->createBuffer(activePhysicalDevice.getMemoryProperties(), sizeof(glm::mat4) * 2, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE, {}, (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	cubemapShadermanager.createUniformBuffer(VK_SHADER_STAGE_VERTEX_BIT, 0);
	cubemapShadermanager.createSampler(VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	cubemapShadermanager.initDeskriptorSetLayout(device);
	cubemap.descriptorSet = device->createDeskriptorSet(descriptorPool, cubemapShadermanager.getDeskriptorSetLayout());
	cubemapShadermanager.initSet(cubemap.descriptorSet);
	cubemapShadermanager.updateUniformBuffer(cubemap.uniformBuffer, 0, sizeof(glm::mat4) * 2, 0);
	cubemapShadermanager.updateSampler(cubemap.texture, 1);

	cubeMapPipelineLayout = device->createPipelineLayout({ cubemapShadermanager.getDeskriptorSetLayout() });

}

void Application::handleInput() {
	int state_W = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_W);
	int state_A = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_A);
	int state_S = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_S);
	int state_D = glfwGetKey(window->getGLFWWindow(), GLFW_KEY_D);
	float speed = 0.01f;
	if (state_W == GLFW_PRESS) {
		cam.pos += speed * cam.front;
	}if (state_A == GLFW_PRESS) {
		cam.pos -= glm::normalize(glm::cross(cam.front, cam.up)) * speed;
	}if (state_S == GLFW_PRESS) {
		cam.pos -= speed * cam.front;
	}if (state_D == GLFW_PRESS) {
		cam.pos += glm::normalize(glm::cross(cam.front, cam.up)) * speed;
	}
}