// Corresponding Header
#include "VulkanRenderer.h"  // Class definition and Vulkan/GLFW dependencies

// Standard Library Headers
#include <chrono>            // For frame timing and performance measurement
#include <fstream>			 // For reading in Files
#include <set>               // For managing unique queue families
#include <iomanip>           // For formatted numeric output
#include <cstdint>           // For fixed-width integer types like uint32_t
#include <limits>            // For std::numeric_limits (used in ChooseSwapExtent)
#include <algorithm>         // For std::clamp (used in ChooseSwapExtent)

/**
 * @brief Reads the contents of a file into a vector of bytes.
 *
 * @param filename Path to the file.
 * @return std::vector<char> Contents of the file.
 * @throws std::runtime_error If the file cannot be opened.
 */
static std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

/**
 * @brief Initializes a GLFW window with Vulkan-specific settings.
 *
 * This function sets up GLFW, disables OpenGL context creation, and prevents window resizing.
 * It creates a window with specified dimensions and stores its pointer for later use.
 */
void VulkanRenderer::InitWindow() {				
	// Initialize the GLFW library
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

	// Initialize GLFW settings for Vulkan
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	// Disable OpenGL context creation
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		// Disable resizing pf Window

	// Create the Window
	if (!(window = glfwCreateWindow(WIDTH, HEIGHT, appName.c_str(), nullptr, nullptr))) {
		throw std::runtime_error("Failed to create GLFW window!\n");
	}
}
/**
 * @brief Main entry point for the Vulkan renderer.
 *
 * This function initializes the window and Vulkan resources, runs the main rendering loop,
 * and cleans up all resources upon completion.
 */
void VulkanRenderer::Run() {
	InitWindow();	// Step 1: Initialize the GLFW window
	InitVulkan();	// Step 2. Initialize Vulkan resources

	MainLoop();		// Step 3: Enter the main Rendering/Event loop

	CleanUp();		// Step 4: Cleanup + Release resources
}


/**
 * @brief Initializes Vulkan resources.
 *
 * This function creates the Vulkan instance, sets up validation layers (if enabled),
 * and prepares the application for Vulkan rendering.
 */
void VulkanRenderer::InitVulkan() {
	CreateInstance();		   // Create the Vulkan Instace
	SetupDebugMessenger();	   // Setup Debug
	CreateSurface();		   // Create Surface
	GetPhysicalDevice();	   // Get Graphics card to use
	CreateLogicalDevice();	   // Create Logical device 
	CreateSwapChain();		   // Create Swachain
	CreateImageViews();        // Create Image Views
	CreateRenderPass();		   // Create Renderpass
	CreateGraphicsPipeline();  // Create GraphicsPipeline
	CreateFramebuffers();	   // Create Framebuffers
	CreateCommandPool();	   //
	CreateCommandBuffers();	   //
	CreateSyncObjects();	   // Create Semaphores & Fences
}
/**
 * @brief Main rendering loop for the application.
 *
 * This function keeps the application running and polls for input events until
 * the window is closed by the user.
 */
void VulkanRenderer::MainLoop() {
	auto lastFrameTime = std::chrono::high_resolution_clock::now();	// Track Time

	// Keep polling for events until Window is closed
	while (!glfwWindowShouldClose(window)) {
		// Frame timing calculations	
		auto currentFrameTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float, std::milli> frameTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		float fps = 1000.0f / frameTime.count();	// Calculate FPS

		// Output frame time and FPS
		std::cout << std::fixed << std::setprecision(2);	// Format output to two decimals places
		std::cout << "\rFrame Time: " << frameTime.count() << "ms | FPS: " << fps << "	" << std::flush;


		glfwPollEvents();	// Process window events
		DrawFrame();
	}

	vkDeviceWaitIdle(device);
}
/**
 * @brief Cleans up Vulkan and GLFW resources.
 *
 * This function ensures that all Vulkan and GLFW objects are properly destroyed
 * to prevent resource leaks.
 */
void VulkanRenderer::CleanUp() {
	vkDeviceWaitIdle(device);  // Ensure all GPU tasks are completed

	// Destroy synchronization objects
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}


	// Destroy Commandpool
	vkDestroyCommandPool(device, commandPool, nullptr);

	// Destroy Framebuffer
	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	// Destroy Graphics Pipeline
	vkDestroyPipeline(device, graphicsPipeline, nullptr);

	// Festroy Pipeline Layout
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

	// Destroy RenderPass
	vkDestroyRenderPass(device, renderPass, nullptr);


	// Destroy Image Views
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	// Destroy Swapchain
	if (swapChain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(device, swapChain, nullptr);
	}

	// Destroy logical device
	if (device != VK_NULL_HANDLE) {
		vkDestroyDevice(device, nullptr);
	}

	// Destroy the debug messenger if validation layers are enabled
	if (enableValidationLayer) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	// Destroy Surface
	if (surface != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

	// Destroy the Vulkan Instance, releasing allocated resources
	if (instance != VK_NULL_HANDLE) {
		vkDestroyInstance(instance, nullptr);
	}

	// Destroy the Window instance
	if (window) {
		glfwDestroyWindow(window);	// Destroy the GLFW window
		glfwTerminate();			// Terminate the GLFW library
	}
}



/**
 * @brief Creates a Vulkan instance, connecting the application to the Vulkan API.
 *
 * This function sets up application metadata, enables required extensions,
 * and optionally includes validation layers for debugging.
 *
 * @throws std::runtime_error If instance creation fails or validation layers are unavailable.
 */
void VulkanRenderer::CreateInstance() {
	// If Validation layers are enabled, verify their availablity and attach them
	if (enableValidationLayer && !CheckValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but available...!\n");
	}

	// Fill out Application-Specific information
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;		// Specify structure type
	appInfo.pApplicationName = appName.c_str();				// Name of application
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);	// Application Version
	appInfo.pEngineName = "N/A";							// Engine name(if applicable)
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);		// Engine version
	appInfo.apiVersion = VK_API_VERSION_1_3;				// Target Vulkan API version

	// Specify details for creating a Vulkan instance
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;	// Specify structure type
	createInfo.pApplicationInfo = &appInfo;						// Attach Application info

	// Retrieve the required extensions for GLFW
	auto glfwExtensions = GetRequiredExtensions();
	// Check if list of retirved extensions exists
	if (!CheckInstanceExtensionSupport(&glfwExtensions)) {
		throw std::runtime_error("Instance does not support extensions");
	}
	createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());	// # of extensions
	createInfo.ppEnabledExtensionNames = glfwExtensions.data();							// List of extensions

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayer) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		PopulateDebugMessengerCreateInfo(debugCreateInfo);							// Setup debug info
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;	// Attach debug messenegr info
	}
	else {
	createInfo.enabledLayerCount = 0;	// No validation layers
	createInfo.pNext = nullptr;			// No additional info
	}

	// Attempt to create the Vulkan instance
	if(vkCreateInstance(&createInfo, nullptr,
		&instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to Create Instance...!\n");
	}
}
/**
 * @brief Sets up the debug messenger for validation layers.
 *
 * This function configures a debug messenger to capture and handle validation
 * layer messages if validation layers are enabled.
 *
 * @throws std::runtime_error If the debug messenger creation fails.
 */
void VulkanRenderer::SetupDebugMessenger() {
	if (!enableValidationLayer) return;	// Skip if validation layers are disabled

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};	// Debug messenger info
	PopulateDebugMessengerCreateInfo(createInfo);		// Fill out the debug info

	// Attempt to create the debug messenger
	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr,
		&debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("Failed to setup up Debug Messeger! \n");
	}
}
/**
 * @brief Creates a Vulkan-compatible window surface.
 *
 * Integrates the Vulkan instance with the GLFW window for rendering.
 *
 * @throws std::runtime_error If surface creation fails.
 */
void VulkanRenderer::CreateSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr,
		&surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}
/**
 * @brief Retrieves and selects a suitable physical GPU device for Vulkan operations.
 *
 * This function queries all physical devices (GPUs) available on the system and checks each one
 * to determine if it meets the application's requirements. It selects the first suitable device
 * and stores it for later use.
 *
 * @throws std::runtime_error If no suitable GPU with Vulkan support is found.
 */
void VulkanRenderer::GetPhysicalDevice() {
	// Query the number of physical devices that support Vulkan
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	// If no devices are found, throw an error
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support!\n");
	}

	// Retrieve the list of physical devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// Iterate through the list of devices to find a suitable one
	for (const auto& device : devices) {
		// Check if the current device meets the criteria
		if (IsDeviceSuitable(device)) {
			physicalDevice = device;	// Store the first suitable device
			break;						// Exit once a suitablle device is found
		}
	}

	// If no suitable device found, throw an error
	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("Failed to find a suitable GPU");
	}
}
/**
 * @brief Creates the logical Vulkan device and retrieves the graphics queue.
 *
 * This function sets up the logical device, which acts as an interface to the physical GPU.
 * It specifies the required queue families, device features, and optional validation layers.
 *
 * @throws std::runtime_error If device creation fails.
 */
void VulkanRenderer::CreateLogicalDevice() {
	// Find the queue family indices for the selected physical device
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	// Set to hold unique queue family indices (graphics and present).
	std::set<uint32_t> uniqueQueueFamilies = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	// Vector to store the queue creation info structures.

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	float queuePriority = 1.0f; // Priority for the queues (range: 0.0 - 1.0)
	
	// Iterate over each unique queue family and populate the queue creation info
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; // Specify structure type
		queueCreateInfo.queueFamilyIndex = queueFamily; // Queue family index
		queueCreateInfo.queueCount = 1; // Number of queues to create in this family
		queueCreateInfo.pQueuePriorities = &queuePriority; // Pointer to the priority value
		queueCreateInfos.push_back(queueCreateInfo); // Add to the list of queue create infos
	}

	// Specify features to be enabled for the device (default: no additional features enabled)
	VkPhysicalDeviceFeatures deviceFeatures{};

	// Logical device creation info structure to configure the Vulkan logical device
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;							// Specify structure type
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());	// Num of queue creation info structures
	createInfo.pQueueCreateInfos = queueCreateInfos.data();								// Pointer to the queue creation info
	createInfo.pEnabledFeatures = &deviceFeatures;										// Pointer to the enabled featers
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());	// No device extensions enabled by default

	// Configure validation layers if they are enabled
	if (enableValidationLayer) {
		createInfo.enabledLayerCount =
			static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	// Create the logical device using the specified configuration
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr,
		&device) != VK_SUCCESS) {
	 throw std::runtime_error("failed to create logical device!");
	}

	// Retrieve the handle for the graphics queue from the logical device
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}
/**
 * @brief Creates the Vulkan swap chain for rendering.
 *
 * This function queries the swap chain support details from the physical device,
 * selects optimal settings for surface format, present mode, and extent, and creates
 * the Vulkan swap chain with the specified configurations.
 *
 * @throws std::runtime_error If swap chain creation fails.
 */
void VulkanRenderer::CreateSwapChain() {
	// Query the support details of the swap chain from the physical device
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

	// Choose the best surface format, presentation mode, and swap extent
	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	// Store the chosen image format and extent for later use
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	// Determine the number of images in the swap chain, ensuring it's within the supported range
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// Define the swap chain creation info structure
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;								  // Target rendering surface
	createInfo.minImageCount = imageCount;						  // Number of images in the swap chain
	createInfo.imageFormat = swapChainImageFormat;				  // Image format
	createInfo.imageColorSpace = surfaceFormat.colorSpace;		  // Color space
	createInfo.imageExtent = swapChainExtent;							  // Image dimensions
	createInfo.imageArrayLayers = 1;							  // Number of layers per image
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;  // Usage flags for the images

	// Determine the sharing mode for the swap chain images based on queue family indices
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Images can be used across multiple queues
		createInfo.queueFamilyIndexCount = 2;					  // Number of queue families
		createInfo.pQueueFamilyIndices = queueFamilyIndices;      // Indices of queue families
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;  // Images are exclusive to a single queue
		createInfo.queueFamilyIndexCount = 0;				      // No additional indices required
		createInfo.pQueueFamilyIndices = nullptr;				  // No additional indices required
	}

	// Set additional swap chain properties
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;  // Pre-transform for the surface
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;			   // Blending with other windows
	createInfo.presentMode = presentMode;									   // Presentation mode
	createInfo.clipped = VK_TRUE;											   // Enable clipping of obscured pixels
	createInfo.oldSwapchain = VK_NULL_HANDLE;								   // Handle for replacing an old swap chain (not used here)

	// Create the Vulkan swap chain
	if (vkCreateSwapchainKHR(device, &createInfo, nullptr,
		&swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	// Retrieve the images from the swap chain
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
}
/**
 * @brief Creates image views for all images in the swap chain.
 *
 * Vulkan requires image views to access and manipulate the data in images. This function creates
 * a 2D image view for each image in the swap chain, specifying how the image data should be
 * interpreted (e.g., format, subresource range, etc.).
 *
 * The image views are stored in the `swapChainImageViews` member variable, which is resized to
 * match the number of images in the swap chain. Each image view is created with the same format
 * as the swap chain images and is configured for color attachment usage.
 *
 * @throws std::runtime_error If any of the image views cannot be created.
 */
void VulkanRenderer::CreateImageViews() {
	// Resize the vector to hold image views for all swap chain images
	swapChainImageViews.resize(swapChainImages.size());
	
	// Loop through all images in the swap chain to create corresponding image views
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		// Structure to specify the image view creation parameters
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;		 // Specify the structure type
		createInfo.image = swapChainImages[i];								 // Assign the swap chain image
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;						 // Specify 2D view type
		createInfo.format = swapChainImageFormat;							 // Use the format from the swap chain

		// Specify component mapping (identity mapping, meaning no swizzle)
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		// Define the subresource range (color aspect, single mip level, single layer)
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;  // Focus on color aspect
		createInfo.subresourceRange.baseMipLevel = 0;						 // Start at mip level 0
		createInfo.subresourceRange.levelCount = 1;							 // Only one mip level
		createInfo.subresourceRange.baseArrayLayer = 0;						 // Start at array layer 0
		createInfo.subresourceRange.layerCount = 1;							 // Single array layer

		// Attempt to create the image view and store it in the vector
		if (vkCreateImageView(device, &createInfo, nullptr,
			&swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}
/**
 * @brief Creates the Vulkan render pass.
 *
 * Sets up a single render pass with a color attachment for rendering.
 * Configures attachment load and store operations, as well as subpass dependencies.
 *
 * @throws std::runtime_error If render pass creation fails.
 */
void VulkanRenderer::CreateRenderPass() {
	// Define the color attachment and its properties
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;						// Match swap chain image format
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;					// No multisampling
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;				// Clear the attachment at the start
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;				// Store the result after rendering
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;	// Not using stencil buffer
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;  //
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;			// Initial layout before rendering
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;		// Layout for presentation	

	// Reference to the color attachment in the subpass
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;									   // Index of the attachment
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // Optimal layout for color attachment

	// Define a single subpass
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;  // Graphics pipeline
	subpass.colorAttachmentCount = 1;							  // One color attachment
	subpass.pColorAttachments = &colorAttachmentRef;			  // Reference to the color attachment

	// Define subpass dependencies
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;							  // External source (previous operations)
	dependency.dstSubpass = 0;												  // Current subpass
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;  // Wait for color output stage
	dependency.srcAccessMask = 0;											  // No specific access required
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;  // Perform color output
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;		  // Write access to color attachment
	
	// Create the render pass
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;  
	renderPassInfo.attachmentCount = 1;			     // One attachment
	renderPassInfo.pAttachments = &colorAttachment;  // Attachment description
	renderPassInfo.subpassCount = 1;				 // One subpass
	renderPassInfo.pSubpasses = &subpass;			 // Subpass description
	renderPassInfo.dependencyCount = 1;				 // One dependency
	renderPassInfo.pDependencies = &dependency;      // Subpass dependency

	// Atemnpt to create the Renderpass
	if (vkCreateRenderPass(device, &renderPassInfo, nullptr,
		&renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}
/**
 * @brief Creates the graphics pipeline.
 *
 * Configures shader stages, input assembly, viewport, rasterizer, and other states
 * necessary to render graphics in Vulkan.
 *
 * @throws std::runtime_error If pipeline creation fails.
 */
void VulkanRenderer::CreateGraphicsPipeline() {
	// Load shaders
	auto vertShaderCode = readFile("../Shaders/vert.spv");
	auto fragShaderCode = readFile("../Shaders/frag.spv");

	VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

	// Vertex shader stage
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	// Fragment shader stage
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[]{
		vertShaderStageInfo, fragShaderStageInfo
	};

	// Vertex input state (empty for now as we're not using vertex buffers)
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;								 // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;								 // Optional

	// Input assembly state
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Viewport and scissor
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	std::vector<VkDynamicState> dynamicStates{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	/*VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;*/

	// Rasterizer state
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;		// Optional
	rasterizer.depthBiasClamp = 0.0f;				// Optional
	rasterizer.depthBiasSlopeFactor = 0.0f;			// Optional

	// Multisampling state
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f;						// Optional
	multisampling.pSampleMask = nullptr;						// Optional
	multisampling.alphaToCoverageEnable = VK_FALSE;				// Optional
	multisampling.alphaToOneEnable = VK_FALSE;					// Optional

	// Color blending state
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;			  // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;   // Optional
	colorBlending.blendConstants[1] = 0.0f;   // Optional
	colorBlending.blendConstants[2] = 0.0f;   // Optional
	colorBlending.blendConstants[3] = 0.0f;   // Optional

	// Pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
	
	// Attempt to create Pipleine layout
	 if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr,
		 &pipelineLayout) != VK_SUCCESS) {
		 throw std::runtime_error("failed to create pipeline layout!");
	 }

	 // Graphice Pipeline
	 VkGraphicsPipelineCreateInfo pipelineInfo{};
	 pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	 pipelineInfo.stageCount = 2;
	 pipelineInfo.pStages = shaderStages;
	 pipelineInfo.pVertexInputState = &vertexInputInfo;
	 pipelineInfo.pInputAssemblyState = &inputAssembly;
	 pipelineInfo.pViewportState = &viewportState;
	 pipelineInfo.pRasterizationState = &rasterizer;
	 pipelineInfo.pMultisampleState = &multisampling;
	 pipelineInfo.pDepthStencilState = nullptr; // Optional
	 pipelineInfo.pColorBlendState = &colorBlending;
	 pipelineInfo.pDynamicState = &dynamicState;
	 pipelineInfo.layout = pipelineLayout;
	 pipelineInfo.renderPass = renderPass;
	 pipelineInfo.subpass = 0;
	 pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	 pipelineInfo.basePipelineIndex = -1; // Optional

	 // Attempt to create Graphic Pipeline
	 if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
		 &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		 throw std::runtime_error("failed to create graphics pipeline!");
	 }

	 // Clean up shader modules
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void VulkanRenderer::CreateFramebuffers() {
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
		swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType =
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr,
			&swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void VulkanRenderer::CreateCommandPool() {
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}

}

void VulkanRenderer::CreateCommandBuffers() {
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

}

void VulkanRenderer::CreateSyncObjects() {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}


}



/**
 * @brief Retrieves the Vulkan extensions required by GLFW.
 *
 * @return std::vector<const char*> A list of required extension names.
 *
 * This function queries GLFW for the extensions it needs and optionally adds
 * the debug utilities extension if validation layers are enabled.
 */
std::vector<const char*> VulkanRenderer::GetRequiredExtensions() {
	// Retrieve the extension names required by GLFW
	uint32_t glfwExtensionCount = 0;	// # of extensions required
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// Copy the Extensions into the vector for easier handling
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	// Add debug utilities extension if validionat layers are anabled
	if (enableValidationLayer) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	if (extensions.empty()) {
		throw std::runtime_error(" Failed to find Extensions...!\n");
	}

	return extensions; // Return the list of extensions
}
/**
 * @brief Checks if the requested validation layers are available.
 *
 * @return bool True if all requested validation layers are supported, false otherwise.
 */
bool VulkanRenderer::CheckValidationLayerSupport() {
	uint32_t layerCount;										// # of available validation layers
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);	// Get layer count

	// Retrieve the list of available validatio layers
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	std::unordered_set<std::string> availableLayerNames;
	for (const auto& layer : availableLayers) {
		availableLayerNames.insert(layer.layerName);
		//std::cout << "\t" << layer.layerName << "\n";
	}

	// Iterate through each requested validation layer
	for (const char* layerName : validationLayers) {
		if (availableLayerNames.find(layerName) == availableLayerNames.end()) {
			return false;
		}
	}

	return true; // All requested layers layers found
}
/**
 * @brief Verifies if the required Vulkan instance extensions are supported.
 *
 * @param checkExtensions Pointer to a vector of required extension names (`const char*`).
 * @return true  If all required extensions are supported.
 * @return false If any required extension is not supported.
 */
bool VulkanRenderer::CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions) {
	// Retrieve the count of available instance extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// Resize the vector to hold all the available instance extensions
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// Iterate through the list of required extensions
	for (const auto& checkExtension : *checkExtensions) {
		bool hasExtension = false;	// Flag to track if the extension is supported

		// Compare required extension against available extensions
		for (const auto& extension : extensions) {
			// Check if the extension matches and set the flag if it does
			if (!strcmp(extension.extensionName, checkExtension)) {
				hasExtension = true;
			}
		}
		// If a required extension is not found, return false
		if (!hasExtension) {
			return false;
		}
	}

	return true;
}

bool VulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(),
		deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
/**
 * @brief Evaluates if a Vulkan physical device meets the application's requirements.
 *
 * This function checks if the given Vulkan physical device is suitable for rendering.
 * The criteria for suitability are:
 * 1. The device must be a discrete GPU (for performance).
 * 2. The device must support geometry shaders.
 * 3. The device must have a queue family that supports graphics operations.
 *
 * @param device The Vulkan physical device to evaluate.
 * @return true If the device meets all the criteria.
 * @return false Otherwise.
 */
bool VulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device) {
	// Check for queue families that support graphics operations
	QueueFamilyIndices indices = FindQueueFamilies(device);

	// Retrieve device properties and supported features
	VkPhysicalDeviceProperties deviceProperties;	// Holds properties like device name, type, etc.
	VkPhysicalDeviceFeatures deviceFeatures;		// Holds features supported by the GPU
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	// Define suitability criteria : must be a discrete GPU and support geometry shaders
	bool isDiscreteGPU = (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
	bool hasGeometryShader = deviceFeatures.geometryShader;
	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() &&
			!swapChainSupport.presentModes.empty();

	}
	/*
	// Log detected GPU details
	std::cout << "Detected GPU: " << deviceProperties.deviceName << "\n";
	std::cout << "\t|- Discrete GPU: " << (isDiscreteGPU ? "Yes" : "No") << "\n";
	std::cout << "\t|- Geometry Shader Support: " << (hasGeometryShader ? "Yes" : "No") << "\n";
	*/

	// Return true if the GPU satisfies all criteria
	return  indices.IsComplete() && isDiscreteGPU && hasGeometryShader && extensionsSupported && swapChainAdequate;
}
/**
 * @brief Finds the queue families for a Vulkan physical device.
 *
 * Vulkan devices have multiple queue families, each with different capabilities.
 * This function searches for a queue family that supports graphics operations (VK_QUEUE_GRAPHICS_BIT).
 * It ensures the selected physical device can handle graphical workloads.
 *
 * @param device The Vulkan physical device to evaluate.
 * @return QueueFamilyIndices A structure containing indices of the required queue families.
 */
VulkanRenderer::QueueFamilyIndices VulkanRenderer::FindQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;  // Structure to store the indices of required queue families.

	// Retrieve queue family properties
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties>queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// Search for a queue family that supports graphics operations
	int i = 0;  // Index to keep track of queue family position
	VkBool32 presentSupport = false;
	for (const auto& queueFamily : queueFamilies) {
		// Check if the current queue family supports graphics operations
		if (queueFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;  // Save the index of the graphics queue family
		}

		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		// If all required families are found, break out of the loop
		if (indices.IsComplete()) {
			break;
		}

		i++;  // Move to the next queue family
	}

	// Return the indices of the required queue families
	return indices;
}
/**
 * @brief Queries swap chain support details for a given physical device.
 *
 * Retrieves capabilities, supported formats, and presentation modes for the Vulkan swap chain
 * associated with the specified physical device and surface.
 *
 * @param device The Vulkan physical device to query.
 * @return SwapChainSupportDetails A structure containing the swap chain support details.
 */
VulkanRenderer::SwapChainSupportDetails VulkanRenderer::QuerySwapChainSupport(VkPhysicalDevice device) {
	// Query the basic surface capabilities (e.g., min/max number of images, extent)
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// Query the supported surface formats
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
			details.formats.data());
	}

	// Query the supported presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
			details.presentModes.data());
	}

	// Return the Swapchain details
	return details;
}
/**
 * @brief Selects the optimal surface format for the swap chain.
 *
 * Chooses a surface format from the available options, preferring a specific format
 * (VK_FORMAT_B8G8R8A8_SRGB) and color space (VK_COLOR_SPACE_SRGB_NONLINEAR_KHR).
 * Falls back to the first available format if the preferred one is not found.
 *
 * @param availableFormats A vector of supported surface formats for the swap chain.
 * @return VkSurfaceFormatKHR The chosen surface format.
 */
VkSurfaceFormatKHR VulkanRenderer::ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats) {
	// Iterate through available formats to find the preferred format and color space
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;  // Return the preferred format if found
		}
	}

	// Fallback: Return the first available format if the preferred one is not found
	return availableFormats[0];
}
/**
 * @brief Selects the optimal presentation mode for the swap chain.
 *
 * Chooses a presentation mode from the available options, prioritizing
 * VK_PRESENT_MODE_MAILBOX_KHR for low latency and reduced tearing.
 * Defaults to VK_PRESENT_MODE_FIFO_KHR, which is guaranteed to be supported.
 *
 * @param availablePresentModes A vector of supported presentation modes for the swap chain.
 * @return VkPresentModeKHR The chosen presentation mode.
 */
VkPresentModeKHR VulkanRenderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	// Iterate through available presentation modes to find the preferred mode
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	// Fallback: Return FIFO mode, which is always supported by Vulkan
	return VK_PRESENT_MODE_FIFO_KHR;
}
/**
 * @brief Chooses the swap extent (resolution) for the swap chain.
 *
 * Determines the resolution of the swap chain images based on the surface capabilities
 * and the current framebuffer size. If the surface specifies a fixed extent, it is used.
 * Otherwise, the extent is calculated and clamped to the supported range.
 *
 * @param capabilities The surface capabilities, including supported extents.
 * @return VkExtent2D The chosen swap extent.
 */
VkExtent2D VulkanRenderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	// Check if the surface specifies a fixed extent
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		// Query the current framebuffer size from the GLFW window
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Calculate the swap extent based on the framebuffer size
		VkExtent2D actualExtent = {
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
		};

		// Clamp the width and height to the supported range
		actualExtent.width = std::clamp(actualExtent.width,
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height,
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height);

		// Return the calculated extent
		return actualExtent;
	}
}

VkShaderModule VulkanRenderer::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr,
		&shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");

	}

	return shaderModule;
}

void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;				   // Optional
	beginInfo.pInheritanceInfo = nullptr;  // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording commandbuffer!");
	}

	// Define Render Pass Begin Info
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;							 // Your render pass handle
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];  // Framebuffer for this swapchain image
	renderPassInfo.renderArea.offset = { 0, 0 };					 // Start at the top-left corner
	renderPassInfo.renderArea.extent = swapChainExtent;				 // Match the swapchain dimensions

	// Clear values for color and depth attachments
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	// Begin the Render Pass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Bind Pipeline
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	// Bind Vertex Buffers, Index Buffers, and Descriptor Sets if necessary
	// (These steps are usually next, but ensure proper setup in your implementation.)

	// Issue Draw Command (for now, this is just a placeholder)
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);  // Draw 3 vertices (e.g., a triangle)

	// End the Render Pass
	vkCmdEndRenderPass(commandBuffer);

	// End Command Buffer Recording
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void VulkanRenderer::DrawFrame() {
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame],
		VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	RecordCommandBuffer(commandBuffers[currentFrame], imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	 submitInfo.waitSemaphoreCount = 1;
	 submitInfo.pWaitSemaphores = waitSemaphores;
	 submitInfo.pWaitDstStageMask = waitStages;
	 submitInfo.commandBufferCount = 1;
	 submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	 VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame]};
	 submitInfo.signalSemaphoreCount = 1;
	 submitInfo.pSignalSemaphores = signalSemaphores;

	 if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) !=
		 VK_SUCCESS) {
		  throw std::runtime_error("failed to submit draw commandbuffer!");	 
	 }

	 VkPresentInfoKHR presentInfo{};
	 presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	 presentInfo.waitSemaphoreCount = 1;
	 presentInfo.pWaitSemaphores = signalSemaphores;
	 VkSwapchainKHR swapChains[] = { swapChain };
	 presentInfo.swapchainCount = 1;
	 presentInfo.pSwapchains = swapChains;
	 presentInfo.pImageIndices = &imageIndex;
	 presentInfo.pResults = nullptr; // Optional


	 vkQueuePresentKHR(presentQueue, &presentInfo);

	 currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}



/**
 * @brief Dynamically creates a Vulkan debug messenger for validation layers.
 *
 * This function dynamically loads the Vulkan function to create a debug messenger
 * and sets up a callback for validation layer messages.
 *
 * @param _instance The Vulkan instance to associate with the debug messenger.
 * @param _pCreateInfo Pointer to the structure specifying debug messenger creation info.
 * @param _pAllocator Custom memory allocator (optional).
 * @param _pDebugMessenger Pointer to the debug messenger to be created.
 *
 * @return VkResult VK_SUCCESS if the debug messenger is created successfully, or an error code.
 */
VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(VkInstance _instance, const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo, const VkAllocationCallbacks* _pAllocator,
	VkDebugUtilsMessengerEXT* _pDebugMessenger) {
	// Load the function pointer for creating the debug messenger
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	// Call the loaded function if it exists: otherwise, return an error
	if (func != nullptr) {
		return func(_instance, _pCreateInfo, _pAllocator, _pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}
/**
 * @brief Dynamically destroys a Vulkan debug messenger.
 *
 * This function dynamically loads the Vulkan function to destroy a debug messenger
 * and releases any resources associated with it.
 *
 * @param _instance The Vulkan instance associated with the debug messenger.
 * @param _pDebugMessenger The debug messenger to destroy.
 * @param _pAllocator Custom memory allocator (optional).
 */
void VulkanRenderer::DestroyDebugUtilsMessengerEXT(VkInstance _instance, VkDebugUtilsMessengerEXT _pDebugMessenger,
	const VkAllocationCallbacks* _pAllocator) {
	// Load the function pointer for destroying the debug messenger
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");

	// Call the loaded function if it exists
	if (func != nullptr) {
		func(_instance, _pDebugMessenger, _pAllocator);
	}
}
/**
 * @brief Callback function for validation layer messages.
 *
 * This function handles messages generated by validation layers, such as errors, warnings,
 * and performance issues, and outputs them to the standard error stream.
 *
 * @param _messageSeverity The severity of the message (e.g., verbose, warning, error).
 * @param _messageType The type of the message (e.g., general, validation, performance).
 * @param _pCallbackData Pointer to the callback data structure containing the message details.
 * @param _pUserData Custom user data (optional, typically null).
 *
 * @return VkBool32 VK_FALSE to indicate that the operation should not be aborted.
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT _messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
	void* _pUserData) {
	std::string message = _pCallbackData->pMessage;

	// Suppress specific warnings
	if (message.find("loader_add_layer_properties") != std::string::npos ||
		message.find("dzn_icd") != std::string::npos) {
		return VK_FALSE;
	}

	// Print the validation layer message to the standard error stream
	std::cerr << "Validation Layer: " << message << "\n";

	return VK_FALSE; // Do not abort the operation
}
/**
 * @brief Populates the structure for creating a Vulkan debug messenger.
 *
 * This function configures the severity levels and message types to capture,
 * and assigns the callback function to handle validation layer messages.
 *
 * @param _createInfo Reference to the debug messenger creation info structure to populate.
 */
void VulkanRenderer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo) {
	// Zero out the structure + Set it's type
	_createInfo = {};
	_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	// Specify the severity levels of messages to capture 
	_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		//VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
	
	// Specify the types of messages to capture
	_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	// Assign the callback function for handling messages 
	_createInfo.pfnUserCallback = debugCallBack;
	_createInfo.pUserData = nullptr;	// No custom user data
}

