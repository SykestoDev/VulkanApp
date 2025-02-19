<<<<<<< HEAD
// Corresponding Header
=======
﻿// Corresponding Header
>>>>>>> Testing
#include "VulkanRenderer.h"  

// Image Loader
#include "stb_image.h"

// Standard Library Headers
#include <gtx/string_cast.hpp>          	 
#include <set>                     
#include <algorithm>    
#include <iostream>  




/**
 * @class VulkanRenderer
 * @brief Implementation of VulkanRenderer methods for Vulkan initialization, rendering, and cleanup.
 */

<<<<<<< HEAD


// === Pubic Methods ===
=======
 // ---------------------------------------------------------------------------
 // 1. Public Interface
 // ---------------------------------------------------------------------------
>>>>>>> Testing

/**
 * @brief Runs the Vulkan application, initializing resources and entering the main loop.
 */
void VulkanRenderer::Run() {
<<<<<<< HEAD
=======
	// Initialize window, Vulkan, and ImGui
>>>>>>> Testing
	InitWindow();
	InitVulkan();
	InitImGui();

<<<<<<< HEAD
	camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	MainLoop();
	CleanUp();
}

=======
	// Set up the camera.
	camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

	// Enter the main rendering loop.
	MainLoop();

	// Clean up all allocated resources.
	CleanUp();
}
>>>>>>> Testing
/**
 * @brief Updates the application state based on input and delta time.
 *
 * Handles camera movement, cursor locking, and toggling polygon modes.
 *
 * @param deltaTime The time elapsed since the last frame.
 */
void VulkanRenderer::Update(float deltaTime) {
<<<<<<< HEAD
	glm::vec3 movement(0.0f);
	static bool tabPressedLastFrame = false;
	static bool key1PressedLastFrame = false;

=======
	// --- Toggle Cursor Lock with Escape Key ---
	static bool tabPressedLastFrame = false;
>>>>>>> Testing
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (!tabPressedLastFrame) {
			isCursorLocked = !isCursorLocked;
			glfwSetInputMode(window, GLFW_CURSOR, isCursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
			if (isCursorLocked) {
<<<<<<< HEAD
				// Center the mouse cursor in the window
=======
				// Center the cursor when locking.
>>>>>>> Testing
				glfwSetCursorPos(window, WIDTH / 2.0, HEIGHT / 2.0);
			}
		}
		tabPressedLastFrame = true;
	}
	else {
		tabPressedLastFrame = false;
	}

<<<<<<< HEAD
	// Enable/Disable Wireframe/Fillmode
=======
	// --- Toggle Polygon Mode with Key 1 ---
	static bool key1PressedLastFrame = false;
>>>>>>> Testing
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		if (!key1PressedLastFrame) {
			currentPolygonMode = (currentPolygonMode == VK_POLYGON_MODE_FILL) ?
				VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
		}
		key1PressedLastFrame = true;
	}
	else {
		key1PressedLastFrame = false;
	}

<<<<<<< HEAD
	// Handle camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera->ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera->ProcessKeyboard(DOWN, deltaTime);

	// Handle camera rolling
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera->ProcessRoll(ROLL_LEFT, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera->ProcessRoll(ROLL_RIGHT, deltaTime);

	// Print debug information to console
	//std::cout << camera->GetDebugInfo() << "\n\n"

}


 // === Initialization Methods ===
=======
	// --- Handle Camera Movement ---
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera->ProcessKeyboard(DOWN, deltaTime);
}

void VulkanRenderer::AddModel(const std::string& modelPath, const std::string& texturePath) {
	try {
		// Create a new model instance
		auto newModel = std::make_unique<Model>(device, physicalDevice, graphicsQueue, commandPool);
		newModel->LoadFromFile(modelPath);
		newModel->LoadTexture(texturePath);

		// Optionally set initial transformation values here
		newModel->SetPosition(glm::vec3(0.0f));  // Example position

		// Add the new model to your list
		modelList.push_back(std::move(newModel));

		// Update the descriptor sets to account for the new model.
		// Depending on your current implementation, you may need to reallocate your descriptor sets.
		CreateDescriptorSets();
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to add model: " << e.what() << "\n";
	}
}



// ---------------------------------------------------------------------------
// 1. Private Interface
// ---------------------------------------------------------------------------
>>>>>>> Testing

 /**
  * @brief Initializes the GLFW window for the Vulkan application.
  *
  * Sets up GLFW, configures it for Vulkan (disabling OpenGL context), and creates a window for rendering.
  * Registers framebuffer resize and mouse callbacks.
  *
  * @throws std::runtime_error if GLFW initialization or window creation fails.
  */
void VulkanRenderer::InitWindow() {
<<<<<<< HEAD
=======
	// Initialize GLFW and check for failure.
>>>>>>> Testing
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

<<<<<<< HEAD
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(WIDTH, HEIGHT, appName.c_str(), nullptr, nullptr);
=======
	// Tell GLFW not to create an OpenGL context.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create the GLFW window with specified width, height, and title.
	window = glfwCreateWindow(WIDTH, HEIGHT, APP_NAME, nullptr, nullptr);
>>>>>>> Testing
	if (!window) {
		throw std::runtime_error("Failed to create GLFW window!");
	}

<<<<<<< HEAD
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
=======
	// Set the user pointer for callbacks.
	glfwSetWindowUserPointer(window, this);

	// Set the framebuffer resize callback.
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

	// Set the cursor position callback using a lambda to forward the event.
>>>>>>> Testing
	glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
		auto renderer = static_cast<VulkanRenderer*>(glfwGetWindowUserPointer(win));
		renderer->MouseCallback(xpos, ypos);
		});
}
/**
 * @brief Initializes Dear ImGui for Vulkan rendering.
 */
void VulkanRenderer::InitImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
<<<<<<< HEAD
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForVulkan(window, true);

=======
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForVulkan(window, true);

	// Initialize ImGui for Vulkan with a forced single sample (no multisampling)
>>>>>>> Testing
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance;
	init_info.PhysicalDevice = physicalDevice;
	init_info.Device = device;
	init_info.QueueFamily = FindQueueFamilies(physicalDevice).graphicsFamily.value();
	init_info.Queue = graphicsQueue;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = descriptorPool;
	init_info.Subpass = 0;
	init_info.MinImageCount = 3;
	init_info.ImageCount = static_cast<uint32_t>(swapChainImages.size());
<<<<<<< HEAD
	init_info.MSAASamples = msaaSamples;
	init_info.Allocator = nullptr;
	init_info.CheckVkResultFn = check_vk_result;
	init_info.RenderPass = renderPass;
=======
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;	// Force a single sample for IMGUI
	init_info.Allocator = nullptr;
	init_info.CheckVkResultFn = check_vk_result;
	init_info.RenderPass = imguiRenderPass;			// Use the dedicated ImGui render pass
>>>>>>> Testing

	ImGui_ImplVulkan_Init(&init_info);
	ImGui_ImplVulkan_CreateFontsTexture();
}
/**
 * @brief Initializes Vulkan and its associated resources.
 *
 * This method sets up the Vulkan instance, debugging tools, physical and logical devices,
 * the swap chain, and other resources necessary for rendering. It creates the rendering
 * pipeline, command buffers, synchronization objects, and prepares the application for drawing.
 *
 * @throws std::runtime_error if any initialization step fails.
 */
void VulkanRenderer::InitVulkan() {
<<<<<<< HEAD
=======
	// Initialize Timing Information
>>>>>>> Testing
	startTime = std::chrono::high_resolution_clock::now();
	lastFrameTime = startTime;

	// Core Vulkan Initialization
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface();
	GetPhysicalDevice();
	CreateLogicalDevice();

<<<<<<< HEAD
	// Swap Chain and Rendering Setup
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateDescriptorSetLayout();
	CreateGraphicsPipeline();

	// Memory and Resources
=======
	// Swap Chain and Pipeline Setup
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateImGuiRenderPass();
	CreateDescriptorSetLayout();
	CreateGraphicsPipeline();

	// Resource and Memory Setup
>>>>>>> Testing
	CreateCommandPool();
	CreateColorResources();
	CreateDepthResources();
	CreateFramebuffers();
<<<<<<< HEAD
	CreateUniformBuffers();

	// Model and Descriptor Setup
	LoadModels();
=======
	CreateImGuiFramebuffers();
	CreateUniformBuffers();

	// Model and Descriptor Setup
	LoadDefualtModels();
>>>>>>> Testing
	CreateDescriptorPool();
	CreateDescriptorSets();

	// Command Buffers and Synchronization
	CreateCommandBuffers();
	CreateSyncObjects();

	// Assign Debug Names (optional)
	AssignDebugNames();
}
/**
 * @brief Main rendering loop.
 *
 * This method continuously polls window events and renders frames until the window is closed.
 * It ensures the device is idle before exiting to complete all queued operations.
 */
void VulkanRenderer::MainLoop() {
<<<<<<< HEAD
	while (!glfwWindowShouldClose(window)) { // Loop until the user closes the window.
		glfwPollEvents();                   // Poll for window events (e.g., input, resize).

		// Calculate delta time
		auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastFrameTime).count();
		elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		lastFrameTime = currentTime;

		// Update frame count
		frameCount++;

		// Update FPS every second
=======
	while (!glfwWindowShouldClose(window)) {
		// Process window events (input, resize, etc.)
		glfwPollEvents();

		// Calculate delta time and total elapsed time.
		auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
		elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
		lastFrameTime = currentTime;

		// Update frame count.
		frameCount++;

		// Update FPS once per second.
>>>>>>> Testing
		static float fpsAccumulator = 0.0f;
		static uint32_t fpsFrameCount = 0;
		fpsAccumulator += deltaTime;
		fpsFrameCount++;
<<<<<<< HEAD

		if (fpsAccumulator >= 1.0f) {
			fps = fpsFrameCount / fpsAccumulator; // Calculate FPS
=======
		if (fpsAccumulator >= 1.0f) {
			fps = static_cast<float>(fpsFrameCount) / fpsAccumulator;
>>>>>>> Testing
			fpsAccumulator = 0.0f;
			fpsFrameCount = 0;
		}

<<<<<<< HEAD
		// Process input and update the camera
		Update(deltaTime);

		DrawFrame();   // Render a frame.
	}

	// Wait for the device to finish all operations before exiting.
	//vkDeviceWaitIdle(device);
=======
		// Process input and update camera or other state.
		Update(deltaTime);

		// Render the current frame.
		DrawFrame();
	}
>>>>>>> Testing
}
/**
 * @brief Cleans up all Vulkan and application resources.
 *
 * This method ensures all Vulkan operations are completed before releasing resources.
 * It destroys the swap chain, pipeline, descriptors, buffers, and other Vulkan objects.
 * Finally, it terminates GLFW and releases the window resources.
 */
void VulkanRenderer::CleanUp() {
	// Wait for the device to finish all pending operations.
	vkDeviceWaitIdle(device);

<<<<<<< HEAD
	// Clean up GUI 
=======
	// --- Clean up IMGUI resources ---
	// Destroy the IMGUI render pass.
	if (imguiRenderPass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, imguiRenderPass, nullptr);
	}
	// Shut down the ImGui Vulkan and GLFW implementations and destroy the context.
>>>>>>> Testing
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

<<<<<<< HEAD
	// Clean up models and their resources.
	for (auto& model : modelList) {
		model.reset(); // Calls the destructor of Model.
	}

	// Clean up swap chain-specific resources.
	CleanupSwapChain();

	// Destroy pipeline and rendering resources.
=======
	// --- Clean up model resources ---
	for (auto& model : modelList) {
		model.reset(); // Releases each model.
	}

	// --- Clean up swapchain-specific resources ---
	CleanupSwapChain();

	// --- Destroy pipeline and render resources ---
>>>>>>> Testing
	if (graphicsPipeline != VK_NULL_HANDLE) {
		vkDestroyPipeline(device, graphicsPipeline, nullptr);
	}
	if (pipelineLayout != VK_NULL_HANDLE) {
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	}
	if (renderPass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, renderPass, nullptr);
	}

<<<<<<< HEAD
	// Clean up frame-specific resources (semaphores, fences, and buffers).
=======
	// --- Clean up per-frame resources (semaphores, fences, uniform buffers) ---
>>>>>>> Testing
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		if (renderFinishedSemaphores[i] != VK_NULL_HANDLE) {
			vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		}
		if (imageAvailableSemaphores[i] != VK_NULL_HANDLE) {
			vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		}
		if (inFlightFences[i] != VK_NULL_HANDLE) {
			vkDestroyFence(device, inFlightFences[i], nullptr);
		}
		if (uniformBuffers[i] != VK_NULL_HANDLE) {
			vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		}
		if (uniformBuffersMemory[i] != VK_NULL_HANDLE) {
			vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
		}
	}

<<<<<<< HEAD
	// Destroy descriptor resources (pool and layout).
=======
	// --- Destroy descriptor resources ---
>>>>>>> Testing
	if (descriptorPool != VK_NULL_HANDLE) {
		vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	}
	if (descriptorSetLayout != VK_NULL_HANDLE) {
		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
	}

<<<<<<< HEAD
	// Destroy the command pool.
=======
	// --- Destroy the command pool ---
>>>>>>> Testing
	if (commandPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, commandPool, nullptr);
	}

<<<<<<< HEAD
	// Destroy the Vulkan logical device.
=======
	// --- Destroy the Vulkan logical device ---
>>>>>>> Testing
	if (device != VK_NULL_HANDLE) {
		vkDestroyDevice(device, nullptr);
	}

<<<<<<< HEAD
	// Destroy the debug messenger if validation layers are enabled.
=======
	// --- Destroy the debug messenger (if enabled) ---
>>>>>>> Testing
	if (enableValidationLayer && debugMessenger != VK_NULL_HANDLE) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

<<<<<<< HEAD
	// Destroy the Vulkan surface.
=======
	// --- Destroy the Vulkan surface ---
>>>>>>> Testing
	if (surface != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

<<<<<<< HEAD
	// Destroy the Vulkan instance.
=======
	// --- Destroy the Vulkan instance ---
>>>>>>> Testing
	if (instance != VK_NULL_HANDLE) {
		vkDestroyInstance(instance, nullptr);
	}

<<<<<<< HEAD
	// Destroy the GLFW window and terminate GLFW.
=======
	// --- Clean up the GLFW window and terminate GLFW ---
>>>>>>> Testing
	if (window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}




<<<<<<< HEAD

=======
>>>>>>> Testing
/**
 * @brief Creates the Vulkan instance.
 *
 * This method sets up the Vulkan instance, which serves as the connection between the application
 * and the Vulkan library. It configures application information, validation layers, and required
 * extensions. If validation layers are enabled, it sets up the debug messenger configuration.
 *
 * @throws std::runtime_error if validation layers are not supported, extensions are missing,
 * or instance creation fails.
 */
void VulkanRenderer::CreateInstance() {
	// Check if validation layers are enabled and supported.
	if (enableValidationLayer && !CheckValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but not available!\n");
	}

	// Populate application information structure.
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;     // Specify the structure type.
<<<<<<< HEAD
	appInfo.pApplicationName = appName.c_str();             // Application name.
=======
	appInfo.pApplicationName = APP_NAME;             // Application name.
>>>>>>> Testing
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);  // Application version.
	appInfo.pEngineName = "N/A";                            // Engine name (not using an engine).
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);       // Engine version.
	appInfo.apiVersion = VK_API_VERSION_1_3;                // Vulkan API version.

	// Populate instance creation information structure.
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;  // Specify the structure type.
	createInfo.pApplicationInfo = &appInfo;                     // Link application information.

	// Retrieve required extensions for Vulkan and GLFW.
	auto glfwExtensions = GetRequiredExtensions();

	// Check if all required extensions are supported.
	if (!CheckInstanceExtensionSupport(&glfwExtensions)) {
		throw std::runtime_error("Instance does not support required extensions");
	}

	// Assign extensions to the instance creation info structure.
	createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
	createInfo.ppEnabledExtensionNames = glfwExtensions.data();

	// If validation layers are enabled, configure them and set up the debug messenger.
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayer) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		// Populate debug messenger information and link it to instance creation.
		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		// Disable validation layers and debug messenger.
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	// Create the Vulkan instance and check for errors.
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Vulkan instance!\n");
	}
}
/**
 * @brief Sets up the debug messenger for validation layers.
 *
 * This method configures and creates the debug messenger, which provides detailed
 * debugging and validation feedback during Vulkan development. It is only enabled
 * if validation layers are active.
 *
 * @throws std::runtime_error if the debug messenger creation fails.
 */
void VulkanRenderer::SetupDebugMessenger() {
	// Skip setup if validation layers are not enabled.
	if (!enableValidationLayer) return;

	// Populate the debug messenger creation information.
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	// Create the debug messenger and check for errors.
	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("Failed to set up Debug Messenger!\n");
	}
}
/**
 * @brief Creates a Vulkan surface for rendering.
 *
 * This method creates a surface that connects the Vulkan instance to the window system,
 * enabling Vulkan to present rendered images to the screen.
 *
 * @throws std::runtime_error if the surface creation fails.
 */
void VulkanRenderer::CreateSurface() {
	// Create a window surface using GLFW, associating it with the Vulkan instance.
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface!");
	}
}
/**
 * @brief Selects a suitable physical device (GPU) for Vulkan operations.
 *
 * This method enumerates all physical devices available on the system, evaluates them for
 * suitability, and selects the first device that meets the application's requirements.
 * It also determines the maximum usable multisampling count for the selected device.
 *
 * @throws std::runtime_error if no Vulkan-compatible GPUs are found or no suitable device is selected.
 */
void VulkanRenderer::GetPhysicalDevice() {
	// Query the number of physical devices available.
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// Ensure at least one physical device is available.
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support!\n");
	}

	// Retrieve a list of available physical devices.
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// Iterate through the devices to find a suitable one.
	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) { // Check if the device meets application requirements.
			physicalDevice = device;    // Assign the selected device.
			msaaSamples = GetMaxUsableSampleCount(); // Determine the max usable MSAA sample count.
			break; // Stop searching once a suitable device is found.
		}
	}

	// Verify that a suitable physical device was found.
	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("Failed to find a suitable GPU");
	}
}
/**
 * @brief Creates a logical device and retrieves queue handles.
 *
 * This method configures and creates a Vulkan logical device from the selected physical device.
 * It enables required device features and extensions, and retrieves handles for the graphics
 * and presentation queues.
 *
 * @throws std::runtime_error if the logical device creation fails.
 */
void VulkanRenderer::CreateLogicalDevice() {
	// Find queue families for the selected physical device.
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	// Identify unique queue families (graphics and presentation) to create queues for.
	std::set<uint32_t> uniqueQueueFamilies = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	// Configure queue creation for each unique queue family.
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	float queuePriority = 1.0f; // Priority of the queues (highest priority).

	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; // Specify the structure type.
		queueCreateInfo.queueFamilyIndex = queueFamily;     // Specify the queue family index.
		queueCreateInfo.queueCount = 1;                     // Request a single queue.
		queueCreateInfo.pQueuePriorities = &queuePriority;  // Set the queue priority.
		queueCreateInfos.push_back(queueCreateInfo);        // Add to the queue creation info list.
	}

	// Specify device features to enable (e.g., anisotropic filtering, sample shading).
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;  // Enable anisotropic filtering.
	deviceFeatures.sampleRateShading = VK_TRUE;  // Enable sample shading for smoother rendering.
	deviceFeatures.fillModeNonSolid = VK_TRUE;   // Enable non-solid fill modes

	VkPhysicalDeviceExtendedDynamicState3FeaturesEXT extendedDynamicState3Features{};
	extendedDynamicState3Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT;
	extendedDynamicState3Features.extendedDynamicState3PolygonMode = VK_TRUE;

	// Configure the logical device creation information.
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;       // Specify the structure type.
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()); // Number of queues.
	createInfo.pQueueCreateInfos = queueCreateInfos.data();        // Pointer to queue creation info.
	createInfo.pNext = &extendedDynamicState3Features;			   // Link the feature struct to the chain
	createInfo.pEnabledFeatures = &deviceFeatures;                 // Pointer to enabled device features.
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();  // Extensions to enable.
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()); // Number of extensions.

	// Include validation layers if they are enabled.
	if (enableValidationLayer) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;  // No validation layers.
	}

	// Create the logical device and check for errors.
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device!");
	}

	// Retrieve handles for the graphics and presentation queues.
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}
/**
 * @brief Creates the Vulkan swap chain.
 *
 * The swap chain manages the presentation of rendered images to the screen.
 * This method configures the swap chain's format, presentation mode, and image properties
 * based on the physical device and surface capabilities. It also retrieves the swap chain images.
 *
 * @throws std::runtime_error if swap chain creation fails.
 */
void VulkanRenderer::CreateSwapChain() {
	// Query the swap chain support details for the selected physical device.
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

	// Choose the best surface format, presentation mode, and swap extent.
	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	// Store the chosen format and extent for future use.
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	// Determine the number of images in the swap chain.
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// Configure the swap chain creation information.
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;  // Specify the structure type.
	createInfo.surface = surface;                                    // The rendering surface.
	createInfo.minImageCount = imageCount;                           // Number of images in the swap chain.
	createInfo.imageFormat = swapChainImageFormat;                   // Image format for the swap chain.
	createInfo.imageColorSpace = surfaceFormat.colorSpace;           // Color space for the images.
	createInfo.imageExtent = swapChainExtent;                        // Resolution of the swap chain images.
	createInfo.imageArrayLayers = 1;                                 // Number of layers in each image.
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;     // Usage for rendering.

	// Configure sharing mode based on queue families.
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;  // Images can be used by multiple queues.
		createInfo.queueFamilyIndexCount = 2;                      // Specify the queue family indices.
		createInfo.pQueueFamilyIndices = queueFamilyIndices;       // Pointer to the queue family indices.
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;  // Images are used by a single queue.
		createInfo.queueFamilyIndexCount = 0;                     // No additional queue family indices needed.
		createInfo.pQueueFamilyIndices = nullptr;
	}

	// Set additional swap chain parameters.
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;  // Preserve the current transform.
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;             // Opaque alpha blending.
	createInfo.presentMode = presentMode;                                      // Chosen presentation mode.
	createInfo.clipped = VK_TRUE;                                              // Enable clipping for obscured regions.
	createInfo.oldSwapchain = VK_NULL_HANDLE;                                  // No existing swap chain to replace.

	// Create the swap chain and check for errors.
	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create swap chain!");
	}

	// Retrieve the images from the swap chain.
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
}
<<<<<<< HEAD


=======
>>>>>>> Testing
/**
 * @brief Creates image views for the swap chain images.
 *
 * This method creates a view for each image in the swap chain, allowing the images to be accessed
 * in the shaders or used as attachments in the rendering pipeline.
 */
void VulkanRenderer::CreateImageViews() {
	// Resize the vector to hold image views for all swap chain images.
	swapChainImageViews.resize(swapChainImages.size());

	// Create an image view for each swap chain image.
	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
		swapChainImageViews[i] = createImageView(
			device,                     // Logical device handle.
			swapChainImages[i],         // Swap chain image to create a view for.
			swapChainImageFormat,       // Image format (matches swap chain format).
			VK_IMAGE_ASPECT_COLOR_BIT,  // Aspect mask specifying the color aspect.
			1                           // Number of mip levels (no mipmaps for swap chain images).
		);
	}
}
/**
 * @brief Creates the Vulkan render pass.
 *
 * A render pass defines how rendering operations interact with attachments (color, depth, resolve).
 * This method sets up color, depth, and resolve attachments, a subpass for rendering, and dependencies
 * to ensure proper synchronization.
 *
 * @throws std::runtime_error if the render pass creation fails.
 */
void VulkanRenderer::CreateRenderPass() {
	// Configure the color attachment.
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;							 // Format of the swap chain images.
	colorAttachment.samples = msaaSamples;								     // Use multisampling for the color attachment.
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;					 // Clear the attachment at the start of rendering.
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;					 // Store the attachment contents after rendering.
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;		 // No stencil operations.
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;               // Image layout before rendering.
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // Layout for rendering.

	// Reference the color attachment in the subpass.
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;                                     // Index of the attachment in the attachment array.
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // Layout during rendering.
<<<<<<< HEAD
=======

	// Configure the depth attachment.
	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = FindDepthFormat();                                     // Find a suitable format for the depth attachment.
	depthAttachment.samples = msaaSamples;                                          // Use multisampling for depth.
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                           // Clear the depth buffer at the start of rendering.
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;                     // Depth data is not needed after rendering.
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;                // No stencil operations.
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;                      // Layout before rendering.
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // Layout for depth rendering.

	// Reference the depth attachment in the subpass.
	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1; // Index of the depth attachment.
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
>>>>>>> Testing
	
	// Configure the resolve attachment (for resolving MSAA to a non-MSAA image).
	VkAttachmentDescription colorAttachmentResolve{};
	colorAttachmentResolve.format = swapChainImageFormat;				      // Same format as the color attachment.
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;					  // No multisampling for resolved image.
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;		  // No load operation.
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;            // Store the resolved image after rendering.
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;   // No stencil operations.
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;         // Layout before rendering.
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;     // Layout for presentation.

	// Reference the resolve attachment in the subpass.
	VkAttachmentReference colorAttachmentResolveRef{};
	colorAttachmentResolveRef.attachment = 2;  // Index of the resolve attachment.
	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

<<<<<<< HEAD
	// Configure the depth attachment.
	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = FindDepthFormat();                                     // Find a suitable format for the depth attachment.
	depthAttachment.samples = msaaSamples;                                          // Use multisampling for depth.
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                           // Clear the depth buffer at the start of rendering.
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;                     // Depth data is not needed after rendering.
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;                // No stencil operations.
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;                      // Layout before rendering.
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // Layout for depth rendering.

	// Reference the depth attachment in the subpass.
	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1; // Index of the depth attachment.
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
=======
>>>>>>> Testing

	// Configure the subpass.
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // Graphics pipeline binding.
	subpass.colorAttachmentCount = 1;                           // Number of color attachments.
	subpass.pColorAttachments = &colorAttachmentRef;            // Reference to the color attachment.
	subpass.pDepthStencilAttachment = &depthAttachmentRef;      // Reference to the depth attachment.
	subpass.pResolveAttachments = &colorAttachmentResolveRef;   // Reference to the resolve attachment.

	// Configure the subpass dependency for synchronization.
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;       // External subpass (pre-rendering).
	dependency.dstSubpass = 0;                         // Current subpass.
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
		VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;    // Wait on color and depth stages.
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
		VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;    // Sync color and depth stages.
	dependency.srcAccessMask = 0;                      // No access requirements for the source.
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
		VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;  // Write access for color and depth.

	// Combine attachments into an array.
	std::array<VkAttachmentDescription, 3> attachments = {
		colorAttachment, depthAttachment, colorAttachmentResolve
	};

	// Configure the render pass.
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO; // Specify the structure type.
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size()); // Number of attachments.
	renderPassInfo.pAttachments = attachments.data();  // Pointer to the attachment array.
	renderPassInfo.subpassCount = 1;                   // Number of subpasses.
	renderPassInfo.pSubpasses = &subpass;              // Pointer to the subpass description.
	renderPassInfo.dependencyCount = 1;                // Number of dependencies.
	renderPassInfo.pDependencies = &dependency;        // Pointer to the dependency.

	// Create the render pass and check for errors.
	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create render pass!");
	}
}
<<<<<<< HEAD
=======
void VulkanRenderer::CreateImGuiRenderPass() {
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;			// Use a single sample for ImGui
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;		// Use LOAD to preserve the previously rendered scene
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &imguiRenderPass) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create ImGui render pass!");
	}
}
>>>>>>> Testing
/**
 * @brief Creates the descriptor set layout.
 *
 * A descriptor set layout defines the structure of resource bindings (e.g., uniform buffers,
 * samplers) used by shaders. This method sets up bindings for a uniform buffer and a combined
 * image sampler.
 *
 * @throws std::runtime_error if descriptor set layout creation fails.
 */
void VulkanRenderer::CreateDescriptorSetLayout() {
	// Define a binding for a uniform buffer object (UBO).
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;                                // Binding index in the shader.
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // Type of resource (UBO).
	uboLayoutBinding.descriptorCount = 1;                        // Number of UBOs bound.
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;    // Accessible in the vertex shader.
	uboLayoutBinding.pImmutableSamplers = nullptr;               // No immutable samplers.

	// Define a binding for a combined image sampler (texture).
	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;                               // Binding index in the shader.
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Type: texture sampler.
	samplerLayoutBinding.descriptorCount = 1;                       // Number of samplers bound.
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; // Accessible in the fragment shader.
	samplerLayoutBinding.pImmutableSamplers = nullptr;              // No immutable samplers.

	// Combine both bindings into an array.
	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

	// Configure the descriptor set layout creation information.
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO; // Specify the structure type.
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());       // Number of bindings.
	layoutInfo.pBindings = bindings.data();                                 // Pointer to the bindings.

	// Create the descriptor set layout and check for errors.
	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor set layout!");
	}
}
/**
 * @brief Creates the Vulkan graphics pipeline.
 *
 * The graphics pipeline defines the entire rendering process, including shaders, input assembly,
 * rasterization, blending, and more. This method sets up and configures all pipeline stages,
 * creates a pipeline layout, and compiles the graphics pipeline.
 *
 * @throws std::runtime_error if the graphics pipeline or pipeline layout creation fails.
 */
void VulkanRenderer::CreateGraphicsPipeline() {
	// Load and create shader modules.
	auto vertShaderCode = readFile("VulkanShaders/vert.spv");
	auto fragShaderCode = readFile("VulkanShaders/frag.spv");

	VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

	// Configure the vertex shader stage.
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main"; // Entry point function.

	// Configure the fragment shader stage.
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main"; // Entry point function.

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// Configure vertex input for passing vertex data to the pipeline.
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// Configure input assembly for assembling primitives.
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;  // Draw triangles.
	inputAssembly.primitiveRestartEnable = VK_FALSE;               // No primitive restart.

	// Define a viewport and scissor rectangle (dynamic states).
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
		VK_DYNAMIC_STATE_SCISSOR,
		VK_DYNAMIC_STATE_POLYGON_MODE_EXT
	};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	// Configure viewport and scissor in the pipeline.
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	// Configure rasterization.
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;					 // Don't clamp depth values.
	rasterizer.rasterizerDiscardEnable = VK_FALSE;           // Ensure rasterization occurs.
	rasterizer.polygonMode = currentPolygonMode;             // Fill polygons.
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;             // Back-face culling.
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;  // Counter-clockwise is front.
	rasterizer.depthBiasEnable = VK_FALSE;

	// Configure multisampling.
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE;          // Enable sample shading.
	multisampling.rasterizationSamples = msaaSamples;     // Multisample settings.
	multisampling.minSampleShading = 0.2f;

	// Configure depth and stencil testing.
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;

	// Configure color blending.
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	// Configure push constants (optional).
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConstants);

	// Configure the pipeline layout.
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create pipeline layout!");
	}

	// Configure the graphics pipeline.
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create graphics pipeline!");
	}

	// Cleanup shader modules after pipeline creation.
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}
/**
 * @brief Creates framebuffers for the swap chain images.
 *
 * A framebuffer is an abstraction that ties together the render pass and the image attachments
 * (color, depth, and resolve attachments) for rendering. This method creates one framebuffer
 * for each swap chain image.
 *
 * @throws std::runtime_error if any framebuffer creation fails.
 */
void VulkanRenderer::CreateFramebuffers() {
	// Resize the framebuffer vector to match the number of swap chain images.
	swapChainFramebuffers.resize(swapChainImageViews.size());

	// Create a framebuffer for each swap chain image view.
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		// Specify the attachments for this framebuffer (color, depth, and swap chain image).
		std::array<VkImageView, 3> attachments = {
<<<<<<< HEAD
			colorImageView,         // Resolve attachment (for MSAA color image).
			depthImageView,         // Depth attachment (for depth testing).
			swapChainImageViews[i]  // Swap chain image (final output to screen).
=======
			colorImageView,          // Resolve attachment (for MSAA color image).
			depthImageView,          // Depth attachment (for depth testing).
			swapChainImageViews[i]   // Swap chain image (final output to screen).
>>>>>>> Testing
		};

		// Configure the framebuffer creation information.
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;  // Specify the structure type.
		framebufferInfo.renderPass = renderPass;                            // Render pass this framebuffer is for.
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size()); // Number of attachments.
		framebufferInfo.pAttachments = attachments.data();                  // Attachments for this framebuffer.
		framebufferInfo.width = swapChainExtent.width;                      // Framebuffer width.
		framebufferInfo.height = swapChainExtent.height;                    // Framebuffer height.
		framebufferInfo.layers = 1;                                         // Number of layers (1 for 2D images).

		// Create the framebuffer and check for errors.
		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create framebuffer!");
		}
	}
}
<<<<<<< HEAD
=======
void VulkanRenderer::CreateImGuiFramebuffers() {
	imguiFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		VkImageView attachments[] = { swapChainImageViews[i] };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = imguiRenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &imguiFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create ImGui framebuffer!");
		}
	}
}
>>>>>>> Testing
/**
 * @brief Creates the command pool for managing command buffer allocation.
 *
 * Command pools allocate and manage command buffers for submitting rendering commands
 * to a specific queue family. This method creates a command pool associated with the
 * graphics queue family.
 *
 * @throws std::runtime_error if the command pool creation fails.
 */
void VulkanRenderer::CreateCommandPool() {
	// Retrieve the queue family indices for the selected physical device.
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

	// Configure the command pool creation information.
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;            // Specify the structure type.
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;       // Allow individual command buffers to be reset.
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();  // Use the graphics queue family.

	// Create the command pool and check for errors.
	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create command pool!");
	}
}
/**
 * @brief Creates the color attachment resources for multisampling.
 *
 * This method creates a color image and its associated memory to store the intermediate
 * multisampled rendering results. A corresponding image view is also created for use in
 * the rendering pipeline.
 *
 * @throws std::runtime_error if image or image view creation fails.
 */
void VulkanRenderer::CreateColorResources() {
	// Use the swap chain image format for the color attachment.
	VkFormat colorFormat = swapChainImageFormat;

	// Create the color image with the specified dimensions, format, and usage.
	createImage(
		device,
		physicalDevice,
		swapChainExtent.width,                     // Image width (matches swap chain extent).
		swapChainExtent.height,                    // Image height (matches swap chain extent).
		1,                                         // Single mip level (no mipmapping for color attachments).
		msaaSamples,                               // Multisampling count for anti-aliasing.
		colorFormat,                               // Format of the color attachment (matches swap chain).
		VK_IMAGE_TILING_OPTIMAL,                   // Optimal tiling for GPU operations.
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |  // Used as a transient attachment.
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,       // Used as a color attachment.
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,       // Memory property for device-local storage.
		colorImage,                                // Output parameter for the created image.
		colorImageMemory                           // Output parameter for the allocated image memory.
	);

	// Create an image view for the color image, allowing it to be used in the rendering pipeline.
	colorImageView = createImageView(
		device,
		colorImage,
		colorFormat,
		VK_IMAGE_ASPECT_COLOR_BIT,  // Specify that this is a color image.
		1                           // Single mip level.
	);
}
/**
 * @brief Creates the depth attachment resources for the render pass.
 *
 * This method creates a depth image and its associated memory for depth testing during rendering.
 * It also creates an image view for the depth image and transitions the image layout for use
 * as a depth-stencil attachment.
 *
 * @throws std::runtime_error if image, image view, or layout transition fails.
 */
void VulkanRenderer::CreateDepthResources() {
	// Find a suitable format for the depth attachment (e.g., depth stencil format).
	VkFormat depthFormat = FindDepthFormat();

	// Create the depth image with the specified parameters.
	createImage(
		device,
		physicalDevice,
		swapChainExtent.width,                        // Image width (matches swap chain extent).
		swapChainExtent.height,                       // Image height (matches swap chain extent).
		1,                                            // Single mip level (no mipmapping for depth attachments).
		msaaSamples,                                  // Multisampling count for anti-aliasing.
		depthFormat,                                  // Format of the depth attachment.
		VK_IMAGE_TILING_OPTIMAL,                      // Optimal tiling for GPU operations.
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,  // Depth-stencil usage.
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,          // Memory property for device-local storage.
		depthImage,                                   // Output parameter for the created depth image.
		depthImageMemory                              // Output parameter for the allocated image memory.
	);

	// Create an image view for the depth attachment to make it accessible in the rendering pipeline.
	depthImageView = createImageView(
		device,
		depthImage,
		depthFormat,
		VK_IMAGE_ASPECT_DEPTH_BIT,  // Specify that this is a depth image.
		1                           // Single mip level.
	);

	// Transition the depth image layout to make it suitable as a depth-stencil attachment.
	transitionImageLayout(
		device,
		commandPool,
		graphicsQueue,
		depthImage,
		depthFormat,
		VK_IMAGE_LAYOUT_UNDEFINED,                         // Initial layout (undefined).
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,  // Target layout for depth testing.
		1                                                  // Single mip level.
	);
}
/**
 * @brief Creates uniform buffers for each frame in flight.
 *
 * Uniform buffers store data that is updated frequently and passed to shaders, such as transformation matrices.
 * This method creates one uniform buffer for each swap chain image and maps its memory for CPU-side updates.
 *
 * @throws std::runtime_error if buffer creation or memory mapping fails.
 */
void VulkanRenderer::CreateUniformBuffers() {
	// Define the size of the uniform buffer (e.g., size of the UniformBufferObject structure).
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	// Resize the vectors to store buffers, memory, and mapped memory for all frames.
	uniformBuffers.resize(swapChainImages.size());
	uniformBuffersMemory.resize(swapChainImages.size());
	uniformBuffersMapped.resize(swapChainImages.size());

	// Create a uniform buffer for each frame in the swap chain.
	for (size_t frame = 0; frame < swapChainImages.size(); frame++) {
		// Create a uniform buffer and allocate device memory for it.
		createBuffer(
			device,
			physicalDevice,
			bufferSize,                            // Size of the buffer.
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,    // Usage as a uniform buffer.
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |  // Host-visible memory for CPU updates.
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,  // Coherent memory for automatic synchronization.
			uniformBuffers[frame],                 // Output buffer handle.
			uniformBuffersMemory[frame]            // Output memory handle.
		);

		// Check for errors in buffer creation.
		if (uniformBuffers[frame] == VK_NULL_HANDLE) {
			throw std::runtime_error("Uniform buffer is VK_NULL_HANDLE!");
		}

		// Map the memory to a CPU-accessible pointer for updates.
		vkMapMemory(
			device,
			uniformBuffersMemory[frame],  // Memory to map.
			0,                            // Offset.
			bufferSize,                   // Size of the memory region to map.
			0,                            // Flags (reserved).
			&uniformBuffersMapped[frame]  // Output pointer to the mapped memory.
		);
	}
}
/**
 * @brief Creates a descriptor pool for allocating descriptor sets.
 *
 * The descriptor pool allows Vulkan to allocate descriptor sets, which are used to bind
 * resources like uniform buffers and image samplers to shaders. This method calculates
 * the required pool size based on the number of frames and models.
 *
 * @throws std::runtime_error if the descriptor pool parameters are invalid or pool creation fails.
 */
void VulkanRenderer::CreateDescriptorPool() {
<<<<<<< HEAD
	// Define the descriptor pool sizes
	std::vector<VkDescriptorPoolSize> poolSizes = {
		// Uniform buffers
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(swapChainImages.size() * modelList.size()) },

		// Combined image samplers
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(swapChainImages.size() * modelList.size()) },

		// Additional types for ImGui and other resources
=======
	// Use at least 1 as the model count to avoid 0 descriptors.
	uint32_t modelCount = modelList.empty() ? 1 : static_cast<uint32_t>(modelList.size());
	uint32_t imageCount = static_cast<uint32_t>(swapChainImages.size());

	std::vector<VkDescriptorPoolSize> poolSizes = {
		// Uniform buffers for each frame-model combination.
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, imageCount * modelCount },
		// Combined image samplers for each frame-model combination.
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, imageCount * modelCount },
		// Additional descriptors for IMGUI and other resources.
>>>>>>> Testing
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
	};

	// Configure the descriptor pool creation information.
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
<<<<<<< HEAD
	poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size() * modelList.size()) + 500; // Add extra capacity for ImGui and other resources
=======
	poolInfo.maxSets = imageCount * modelCount + 500;					// Add extra capacity for ImGui and other resources
>>>>>>> Testing
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT; // Enable freeing individual sets

	// Validate descriptor pool parameters.
	if (poolInfo.maxSets == 0 || poolSizes[0].descriptorCount == 0 || poolSizes[1].descriptorCount == 0) {
		throw std::runtime_error("Descriptor pool parameters are invalid!");
	}

	// Create the descriptor pool and check for errors.
	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor pool!");
	}
}
/**
 * @brief Creates and updates descriptor sets for uniform buffers and texture samplers.
 *
 * This method allocates descriptor sets for each frame and model combination, then updates
 * the descriptor sets with the appropriate uniform buffer and texture sampler information.
 *
 * @throws std::runtime_error if descriptor set allocation or updates fail.
 */
void VulkanRenderer::CreateDescriptorSets() {
	// Resize the descriptor sets vector to accommodate all frames and models.
<<<<<<< HEAD
	descriptorSets.resize(swapChainImages.size() * modelList.size());
=======
	uint32_t modelCount = modelList.empty() ? 1 : static_cast<uint32_t>(modelList.size());
	descriptorSets.resize(swapChainImages.size() * modelCount);
>>>>>>> Testing

	// Create a list of descriptor set layouts for allocation.
	std::vector<VkDescriptorSetLayout> layouts(descriptorSets.size(), descriptorSetLayout);

	// Configure descriptor set allocation.
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;                     // Use the descriptor pool created earlier.
	allocInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSets.size()); // Number of descriptor sets to allocate.
	allocInfo.pSetLayouts = layouts.data();                        // Layouts for the descriptor sets.

	// Allocate descriptor sets and check for errors.
	if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate descriptor sets!");
	}

	// Update descriptor sets for each frame and model.
	for (size_t currFrame = 0; currFrame < swapChainImages.size(); currFrame++) {
		for (size_t currModel = 0; currModel < modelList.size(); currModel++) {
			if (currModel >= modelList.size()) {
				throw std::runtime_error("Model index out of bounds!");
			}

			// Access the current model.
			Model* model = modelList[currModel].get();

			// Calculate the descriptor index for the current frame and model.
			size_t descriptorIndex = currFrame * modelList.size() + currModel;
			VkDescriptorSet& descriptorSet = descriptorSets[descriptorIndex];

			// Configure the uniform buffer descriptor.
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffers[currFrame]; // Use the frame-specific uniform buffer.
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject); // Size of the UBO structure.

			// Configure the texture sampler descriptor for the model.
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = model->GetTextureImageView(); // Model-specific image view.
			imageInfo.sampler = model->GetTextureSampler();     // Model-specific sampler.

			// Write descriptors for the uniform buffer and texture sampler.
			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

			// Binding 0: Uniform buffer.
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSet;                   // Descriptor set to update.
			descriptorWrites[0].dstBinding = 0;                          // Matches UBO binding in the shader.
			descriptorWrites[0].dstArrayElement = 0;                     // Array index (0 for single descriptor).
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // Descriptor type.
			descriptorWrites[0].descriptorCount = 1;                     // Number of descriptors.
			descriptorWrites[0].pBufferInfo = &bufferInfo;               // Uniform buffer info.

			// Binding 1: Texture sampler.
			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSet;   // Descriptor set to update.
			descriptorWrites[1].dstBinding = 1;           // Matches sampler binding in the shader.
			descriptorWrites[1].dstArrayElement = 0;      // Array index (0 for single descriptor).
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Descriptor type.
			descriptorWrites[1].descriptorCount = 1;      // Number of descriptors.
			descriptorWrites[1].pImageInfo = &imageInfo;  // Image sampler info.

			// Update the descriptor sets.
			vkUpdateDescriptorSets(
				device,
				static_cast<uint32_t>(descriptorWrites.size()),  // Number of descriptors to update.
				descriptorWrites.data(),                         // Descriptors to update.
				0,
				nullptr
			);
		}
	}
}
/**
 * @brief Allocates command buffers for rendering.
 *
 * Command buffers are used to record rendering and compute commands that are submitted to a queue.
 * This method allocates one primary command buffer for each swap chain image to handle per-frame rendering.
 *
 * @throws std::runtime_error if command buffer allocation fails.
 */
void VulkanRenderer::CreateCommandBuffers() {
	// Resize the command buffer vector to match the number of swap chain images.
	commandBuffers.resize(swapChainImages.size());

	// Configure the command buffer allocation information.
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;  // Specify the structure type.
	allocInfo.commandPool = commandPool;                               // Command pool to allocate from.
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;                 // Allocate primary command buffers.
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size()); // Number of command buffers to allocate.

	// Allocate command buffers and check for errors.
	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers!");
	}
}
<<<<<<< HEAD

=======
>>>>>>> Testing
/**
 * @brief Creates synchronization objects for rendering.
 *
 * Synchronization objects ensure proper sequencing of rendering operations. This method creates:
 * - Semaphores to signal image availability and render completion.
 * - Fences to ensure a frame is not reused until rendering is complete.
 *
 * @throws std::runtime_error if any synchronization object creation fails.
 */
void VulkanRenderer::CreateSyncObjects() {
	// Resize vectors to store synchronization objects for each frame in flight.
	imageAvailableSemaphores.resize(swapChainImages.size());  // Signals when an image is available for rendering.
	renderFinishedSemaphores.resize(swapChainImages.size());  // Signals when rendering is finished.
	inFlightFences.resize(swapChainImages.size());            // Ensures frames are not reused prematurely.

	// Configure semaphore creation information.
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;  // Specify the structure type.

	// Configure fence creation information.
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;  // Specify the structure type.
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;         // Initialize fences in the signaled state.

	// Create synchronization objects for each frame in flight.
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		// Create semaphores and fences and check for errors.
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create synchronization objects for a frame!");
		}
	}
}




<<<<<<< HEAD

=======
void VulkanRenderer::RenderImGui(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().FontGlobalScale = 1.5f; // Scale text size (default is 1.0f)

	// === Performance Metrics Panel ===
	ImGui::Begin("Performance Metrics");
	ImGui::Text("FPS: %.2f", fps);
	ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);
	ImGui::Text("Elapsed Time: %.2f s", elapsedTime);
	ImGui::Text("Frame Count: %llu", frameCount);
	ImGui::Text("# of Models: %llu", modelList.size());
	// --- New Add Model Button ---
	if (ImGui::Button("Add Model")) {
		// Here we use default paths; you could also allow the user to input a path.
		AddModel(MODEL_PATH, TEXTURE_PATH);
	}
	if (ImGui::Button("Quit")) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	ImGui::End();

	// === Models Debug Info Panel ===
	ImGui::Begin("Models Debug Info");
	for (size_t i = 0; i < modelList.size(); ++i) {
		const auto& model = modelList[i];
		glm::mat4 modelMatrix = model->GetModelMatrix();

		ImGui::Text("Model %zu Matrix:", i + 1);
		for (int row = 0; row < 4; ++row) {
			ImGui::Text("[ %.2f, %.2f, %.2f, %.2f ]",
				modelMatrix[row][0], modelMatrix[row][1],
				modelMatrix[row][2], modelMatrix[row][3]);
		}
		ImGui::Separator();
	}
	ImGui::End();
	ImGui::Render();  // Ensures ImGui prepares its draw data


	// Create RenderPass for GUI 
	VkRenderPassBeginInfo imguiRenderPassInfo{};
	imguiRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	imguiRenderPassInfo.renderPass = imguiRenderPass;
	imguiRenderPassInfo.framebuffer = imguiFramebuffers[imageIndex];
	imguiRenderPassInfo.renderArea.offset = { 0, 0 };
	imguiRenderPassInfo.renderArea.extent = swapChainExtent;

	VkClearValue clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };
	imguiRenderPassInfo.clearValueCount = 1;
	imguiRenderPassInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(commandBuffer, &imguiRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	vkCmdEndRenderPass(commandBuffer);
}
>>>>>>> Testing
/**
 * @brief Loads and configures 3D models for rendering.
 *
 * This method initializes, configures, and loads 3D models into the Vulkan rendering pipeline.
 * Models are scaled, rotated, and positioned in the scene, and their geometry and textures are
 * loaded from files. Finally, the models are added to the `modelList`.
 *
 * @throws std::runtime_error if loading model data or textures fails.
 */
<<<<<<< HEAD
void VulkanRenderer::LoadModels() {
	// Create unique pointers for the models.
	std::unique_ptr<Model> model0;
	std::unique_ptr<Model> model1;

	// Initialize models with device and rendering resources.
	model0 = std::make_unique<Model>(device, physicalDevice, graphicsQueue, commandPool);
	model1 = std::make_unique<Model>(device, physicalDevice, graphicsQueue, commandPool);
=======
void VulkanRenderer::LoadDefualtModels() {
	// Create unique pointers for the models.
	std::unique_ptr<Model> model0;

	// Initialize models with device and rendering resources.
	model0 = std::make_unique<Model>(device, physicalDevice, graphicsQueue, commandPool);
>>>>>>> Testing

	// Model 1 defaults
	model0->SetPosition(glm::vec3(0.50f, 0.00f, 0.00f));
	model0->SetScale(glm::vec3(0.50f, 0.50f, 0.50f));
	model0->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

<<<<<<< HEAD
	// Model 2 defaults
	model1->SetPosition(glm::vec3(-0.50f, 0.00f, 0.00f));
	model1->SetScale(glm::vec3(1.00f, 1.00f, 1.00f));
	model1->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));


	try {
		// Load geometry data for the models.
		model0->LoadFromFile("VulkanModels/girl OBJ.obj");  // Load geometry for the first model.
		model1->LoadFromFile(MODEL_PATH.c_str());           // Load geometry for the second model.

		// Load textures for the models.
		model0->LoadTexture("VulkanTextures/texture.jpeg");  // Load texture for the first model.
		model1->LoadTexture(TEXTURE_PATH.c_str());           // Load texture for the second model.

		// Add the models to the rendering model list.
		modelList.push_back(std::move(model0));
		modelList.push_back(std::move(model1));
=======

	try {
		// Load geometry data for the models.
		model0->LoadFromFile(MODEL_PATH.c_str());   // Load geometry for the second model.
		model0->LoadTexture(TEXTURE_PATH.c_str());  // Load texture for the second model.

		// Add the models to the rendering model list.
		//modelList.push_back(std::move(model0));
>>>>>>> Testing
	}
	catch (const std::runtime_error& e) {
		// Throw an error if loading fails, with details about the failure.
		throw std::runtime_error(std::string("Failed to load model: ") + e.what());
	}
}
/**
 * @brief Updates the uniform buffer with per-frame transformation data.
 *
 * This method updates the uniform buffer for the specified frame with new view and projection matrices.
 * The view matrix defines the camera's position and orientation, while the projection matrix handles
 * perspective projection.
 *
 * @param currentImage The index of the current frame's swap chain image.
 */
void VulkanRenderer::UpdateUniformBuffer(uint32_t currentImage) {
	float aspectRatio = swapChainExtent.width / static_cast<float>(swapChainExtent.height);

	// Create a uniform buffer object to hold the transformation data.
	UniformBufferObject ubo{};
	ubo.view = camera->GetViewMatrix();  // Get dynamic view matrix from the camera

	// Generate the projection matrix using glm::perspective
	ubo.proj = glm::perspective(
		glm::radians(45.0f),  // Field of view (45 degrees)
		aspectRatio,          // Aspect ratio from swap chain extent
		0.1f,                 // Near clipping plane
		10.0f                 // Far clipping plane
	);

	// Invert the Y-axis in the projection matrix to match Vulkan's coordinate system
	ubo.proj[1][1] *= -1;

<<<<<<< HEAD
	/*
	PrintMatrix(ubo.view, "View");
	PrintMatrix(ubo.proj, "Proj");
	std::cout << "-------------\n";
	*/

	/*
	// Define the view matrix (camera position and orientation).
	// Camera positioned slightly to the left, above, and looking at the origin.
	ubo.view = glm::lookAt(
		glm::vec3(-0.2f, 2.25f, 0.5f),   // Camera position.
		glm::vec3( 0.0f, 0.0f, 0.25f),   // Target position.
		glm::vec3( 0.0f, 0.0f, 1.0f)     // Up direction.
	);

	// Define the projection matrix (perspective projection).
	ubo.proj = glm::perspective(
		glm::radians(45.0f),  // Field of view (90 degrees).
		swapChainExtent.width / (float)swapChainExtent.height, // Aspect ratio.
		0.1f,												   // Near clipping plane.
		10.0f                                                  // Far clipping plane.
	);

	// Invert the Y-axis in the projection matrix to match Vulkan's coordinate system.
	ubo.proj[1][1] *= -1;
	*/

	/*
	PrintMatrix(ubo.view, "View");
	PrintMatrix(ubo.proj, "Proj");
	*/

=======
>>>>>>> Testing
	// Copy the uniform buffer object data to the mapped memory for the current frame.
	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}
/**
 * @brief Records commands into a command buffer for rendering a frame.
 *
 * This method sets up the render pass, configures viewports and scissors, binds the pipeline,
 * and issues draw commands for each model in the scene. The commands recorded here are submitted
 * for execution during rendering.
 *
 * @param commandBuffer The command buffer to record commands into.
 * @param imageIndex The index of the current swap chain image being rendered.
 *
 * @throws std::runtime_error if command buffer recording or descriptor set binding fails.
 */
void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
	// Begin recording commands into the command buffer.
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;  // Specify the structure type.
	beginInfo.flags = 0;                                            // No special flags.
	beginInfo.pInheritanceInfo = nullptr;                           // Not using secondary command buffers.

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin recording command buffer!");
	}

<<<<<<< HEAD
	// Configure the render pass begin info.
=======
	// Configure main render pass begin info.
>>>>>>> Testing
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;  // Specify the structure type.
	renderPassInfo.renderPass = renderPass;                           // Render pass to use.
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];   // Framebuffer for the current swap chain image.
	renderPassInfo.renderArea.offset = { 0, 0 };                      // Render from the top-left corner.
	renderPassInfo.renderArea.extent = swapChainExtent;               // Render to the full extent of the swap chain image.

<<<<<<< HEAD
	// Define clear values for the color and depth buffers.
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 0.0f };  // Clear color buffer to the background color.
	clearValues[1].depthStencil = { 1.0f, 0 };     // Clear depth buffer to 1.0 and stencil to 0.

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// Begin the render pass.
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f }; // Green for geometry pass
	BeginDebugMarker(device, commandBuffer, "Geometry Pass", color);

=======
	
	// Define clear values for the color and depth buffers.
	std::array<VkClearValue, 3> clearValues = { {
		{ { 0.0f, 0.0f, 0.0f, 1.0f } },		// Color clear value
		{ 1.0f, 0 },						// Depth must be between 0.0 and 1.0
		{ { 0.0f, 0.0f, 0.0f, 1.0f } }		// Resolve attachment clear
	} };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// RenderDoc Dubeg Tag for geomotry pass
	float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f }; // Green for geometry pass
	BeginDebugMarker(device, commandBuffer, "Geometry Pass", color);

	// Begin the render pass.
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	// Bind the graphics pipeline.
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	// Set the polygon mode dynamically
	SetPolygonMode(commandBuffer, currentPolygonMode);

>>>>>>> Testing
	// Set up the viewport for rendering.
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	// Set up the scissor rectangle.
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

<<<<<<< HEAD
	// Bind the graphics pipeline.
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	// Set the polygon mode dynamically
	SetPolygonMode(commandBuffer, currentPolygonMode);

	// Record draw commands for each model.
	for (size_t modelIndex = 0; modelIndex < modelList.size(); modelIndex++) {
		const auto& model = modelList[modelIndex];

		// Bind the vertex and index buffers for the model.
		model->Bind(commandBuffer);

		// Calculate the descriptor set index.
		size_t descriptorIndex = imageIndex * modelList.size() + modelIndex;

=======
	// Record draw commands for each model.
	const size_t numModels = modelList.size();
	for (size_t i = 0; i < numModels; i++) {
		const auto& currModel = modelList[i].get();
		// Bind the vertex and index buffers for the model.
		currModel->Bind(commandBuffer);

		// Calculate the descriptor set index.
		size_t descriptorIndex = imageIndex * modelList.size() + i;
>>>>>>> Testing
		// Validate the descriptor index.
		if (descriptorIndex >= descriptorSets.size()) {
			throw std::runtime_error("Descriptor index out of bounds!");
		}

<<<<<<< HEAD
		VkDescriptorSet descriptorSet = descriptorSets[descriptorIndex];

		// Bind the descriptor set.
		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0,
			1,
			&descriptorSet,
			0,
			nullptr
=======
		// Bind the descriptor set.
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout, 0, 1, &descriptorSets[descriptorIndex],
			0, nullptr
>>>>>>> Testing
		);

		// Push constants for the model matrix.
		PushConstants pushConstants{};
<<<<<<< HEAD
		pushConstants.model = model->GetModelMatrix();
		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);

		// Issue the draw command for the model.
		model->Draw(commandBuffer);
	}

	
	// Start ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().FontGlobalScale = 1.5f; // Scale text size (default is 1.0f)

	// Add Performance Metrics Panel
	ImGui::Begin("Performance Metrics");
	ImGui::Text("FPS: %.2f", fps);
	ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);
	ImGui::Text("Elapsed Time: %.2f s", elapsedTime);
	ImGui::Text("Frame Count: %llu", frameCount);
	if (ImGui::Button("Quit")) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	ImGui::End();

	//// Add Camera Debug Info Panel
	//ImGui::Begin("Camera Debug Info");
	//std::string debugInfo = camera->GetDebugInfo();
	//ImGui::TextWrapped("%s", debugInfo.c_str());
	//ImGui::End();

	// Models Debug Info Panel
	ImGui::Begin("Models Debug Info");
	for (size_t i = 0; i < modelList.size(); ++i) {
		const auto& model = modelList[i];
		glm::mat4 modelMatrix = model->GetModelMatrix();

		// Display the matrix in ImGui
		ImGui::Text("Model %zu Matrix:", i + 1);
		for (int row = 0; row < 4; ++row) {
			ImGui::Text("[ %.2f, %.2f, %.2f, %.2f ]",
				modelMatrix[row][0],
				modelMatrix[row][1],
				modelMatrix[row][2],
				modelMatrix[row][3]);
		}
		ImGui::Separator();
	}
	ImGui::End();

	// Render ImGui data
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	EndDebugMarker(device, commandBuffer);

	// End the render pass.
	vkCmdEndRenderPass(commandBuffer);

=======
		pushConstants.model = currModel->GetModelMatrix();
		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);

		// Issue the draw command for the model.
		currModel->Draw(commandBuffer);
	}

	EndDebugMarker(device, commandBuffer);
	// End the render pass.
	vkCmdEndRenderPass(commandBuffer);

	// Render the IMGUI overlay.
	RenderImGui(commandBuffer, imageIndex);

>>>>>>> Testing
	// Finish recording commands into the command buffer.
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to record command buffer!");
	}
}
/**
 * @brief Manages the rendering of a single frame.
 *
 * This method handles synchronization, command buffer recording, drawing, and presenting the rendered
 * frame to the screen. It also deals with swap chain recreation if required.
 *
 * @throws std::runtime_error if any Vulkan operations (e.g., acquiring images, submitting commands, or presenting) fail.
 */
void VulkanRenderer::DrawFrame() {
	// Wait for the current frame's fence to ensure the GPU has finished processing the previous frame.
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	// Acquire the next image from the swap chain.
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(
		device,
		swapChain,
		UINT64_MAX,                              // Timeout (wait indefinitely).
		imageAvailableSemaphores[currentFrame],  // Signal when the image is available.
		VK_NULL_HANDLE,
		&imageIndex                              // Index of the acquired image.
	);

	// Handle swap chain recreation if out of date.
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	// Reset the fence for the current frame to unsignaled state.
	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	// Reset and record the command buffer for the current frame.
	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	RecordCommandBuffer(commandBuffers[currentFrame], imageIndex);

	// Update the uniform buffer with frame-specific data.
	UpdateUniformBuffer(currentFrame);
	
	// Configure the submit info for rendering.
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	// Wait for the image to become available before rendering.
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	// Specify the command buffer for rendering.
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	// Signal the semaphore once rendering is complete.
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	// Submit the command buffer to the graphics queue.
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	// Configure the present info for displaying the rendered image.
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;  // Wait for rendering to complete.
	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	// Present the rendered image to the screen.
	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	// Handle swap chain recreation if needed (e.g., out of date or resized).
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swap chain image!");
	}

	// Advance to the next frame, looping back after the last frame.
	currentFrame = (currentFrame + 1) % swapChainImages.size();
}




/**
 * @brief Assigns debug names to Vulkan objects for easier debugging.
 */
void VulkanRenderer::AssignDebugNames() {
	// Logical Device
	SetObjectName(device, (uint64_t)device, VK_OBJECT_TYPE_DEVICE, "Logical Device");

	// Swap Chain
	SetObjectName(device, (uint64_t)swapChain, VK_OBJECT_TYPE_SWAPCHAIN_KHR, "Swap Chain");
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		std::string name = "Swap Chain Image View " + std::to_string(i);
		SetObjectName(device, (uint64_t)swapChainImageViews[i], VK_OBJECT_TYPE_IMAGE_VIEW, name.c_str());
	}

	// Render Pass
	SetObjectName(device, (uint64_t)renderPass, VK_OBJECT_TYPE_RENDER_PASS, "Main Render Pass");

	// Graphics Pipeline
	SetObjectName(device, (uint64_t)graphicsPipeline, VK_OBJECT_TYPE_PIPELINE, "Main Graphics Pipeline");

	// Command Pool
	SetObjectName(device, (uint64_t)commandPool, VK_OBJECT_TYPE_COMMAND_POOL, "Main Command Pool");

	// Depth Resources
	SetObjectName(device, (uint64_t)depthImage, VK_OBJECT_TYPE_IMAGE, "Depth Image");
	SetObjectName(device, (uint64_t)depthImageView, VK_OBJECT_TYPE_IMAGE_VIEW, "Depth Image View");

	// Framebuffers
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
		std::string name = "Framebuffer " + std::to_string(i);
		SetObjectName(device, (uint64_t)swapChainFramebuffers[i], VK_OBJECT_TYPE_FRAMEBUFFER, name.c_str());
	}

	// Uniform Buffers
	for (size_t i = 0; i < uniformBuffers.size(); i++) {
		std::string name = "Uniform Buffer " + std::to_string(i);
		SetObjectName(device, (uint64_t)uniformBuffers[i], VK_OBJECT_TYPE_BUFFER, name.c_str());
	}

	// Command Buffers
	for (size_t i = 0; i < commandBuffers.size(); i++) {
		std::string name = "Command Buffer " + std::to_string(i);
		SetObjectName(device, (uint64_t)commandBuffers[i], VK_OBJECT_TYPE_COMMAND_BUFFER, name.c_str());
	}

	// Synchronization Objects
	for (size_t i = 0; i < imageAvailableSemaphores.size(); i++) {
		std::string name = "Image Available Semaphore " + std::to_string(i);
		SetObjectName(device, (uint64_t)imageAvailableSemaphores[i], VK_OBJECT_TYPE_SEMAPHORE, name.c_str());
	}
	for (size_t i = 0; i < renderFinishedSemaphores.size(); i++) {
		std::string name = "Render Finished Semaphore " + std::to_string(i);
		SetObjectName(device, (uint64_t)renderFinishedSemaphores[i], VK_OBJECT_TYPE_SEMAPHORE, name.c_str());
	}
}
<<<<<<< HEAD

=======
>>>>>>> Testing
/**
 * @brief Retrieves the required Vulkan instance extensions.
 *
 * This method queries the extensions required by GLFW to interface with the Vulkan instance.
 * If validation layers are enabled, the debug utilities extension is also added.
 *
 * @return A vector containing the names of the required extensions.
 *
 * @throws std::runtime_error if no required extensions are found.
 */
std::vector<const char*> VulkanRenderer::GetRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;

	// Query the extensions required by GLFW for Vulkan.
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// Populate a vector with the GLFW-required extensions.
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	// If validation layers are enabled, add the debug utilities extension.
	if (enableValidationLayer) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	// Ensure that the list of extensions is not empty.
	if (extensions.empty()) {
		throw std::runtime_error("Failed to find required extensions!");
	}

	return extensions;
}
/**
 * @brief Checks if the requested validation layers are supported by the Vulkan instance.
 *
 * This method queries the available Vulkan instance layers and verifies that all
 * requested validation layers are supported on the system.
 *
 * @return true if all requested validation layers are supported; false otherwise.
 */
bool VulkanRenderer::CheckValidationLayerSupport() {
	uint32_t layerCount;

	// Query the number of available Vulkan instance layers.
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	// Retrieve the properties of the available layers.
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Create a set of available layer names for efficient lookup.
	std::unordered_set<std::string> availableLayerNames;
	for (const auto& layer : availableLayers) {
		availableLayerNames.insert(layer.layerName);
	}

	// Verify that each requested validation layer is available.
	for (const char* layerName : validationLayers) {
		if (availableLayerNames.find(layerName) == availableLayerNames.end()) {
			return false;  // Requested validation layer not found.
		}
	}

	return true;  // All requested validation layers are supported.
}
/**
 * @brief Checks if all required instance extensions are supported.
 *
 * This method queries the available Vulkan instance extensions and verifies that all
 * required extensions specified in `checkExtensions` are supported by the system.
 *
 * @param checkExtensions A pointer to a vector containing the names of the required extensions.
 *
 * @return true if all required extensions are supported; false otherwise.
 */
bool VulkanRenderer::CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions) {
	uint32_t extensionCount = 0;

	// Query the number of available Vulkan instance extensions.
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// Retrieve the properties of the available extensions.
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// Verify that each required extension is available.
	for (const auto& checkExtension : *checkExtensions) {
		bool hasExtension = false;

		// Check if the current required extension exists in the available extensions.
		for (const auto& extension : extensions) {
			if (!strcmp(extension.extensionName, checkExtension)) {
				hasExtension = true;
				break;  // Extension found, no need to check further.
			}
		}

		// If a required extension is not found, return false.
		if (!hasExtension) {
			return false;
		}
	}

	return true;  // All required extensions are supported.
}
/**
 * @brief Checks if all required device extensions are supported by a physical device.
 *
 * This method queries the available extensions for the specified physical device and verifies
 * that all extensions listed in `deviceExtensions` are supported.
 *
 * @param device The Vulkan physical device to check for extension support.
 *
 * @return true if all required device extensions are supported; false otherwise.
 */
bool VulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount = 0;

	// Query the number of available device extensions.
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	// Retrieve the properties of the available device extensions.
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	// Print the available extensions.
	/*std::cout << "Available Device Extensions:\n";
	for (const auto& extension : availableExtensions) {
		std::cout << "\t" << extension.extensionName << " (Spec version: " << extension.specVersion << ")\n";
	}*/

	// Create a set of required extensions for validation.
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	// Check each available extension against the required extensions.
	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);  // Remove from the set if supported.
	}

	// If the set of required extensions is empty, all required extensions are supported.
	return requiredExtensions.empty();
}
/**
 * @brief Determines if a physical device is suitable for use.
 *
 * This method evaluates whether a given physical device meets the requirements for rendering,
 * including queue families, required extensions, swap chain support, and specific device features.
 *
 * @param device The Vulkan physical device to evaluate.
 *
 * @return true if the device is suitable; false otherwise.
 */
bool VulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device) {
	// Check for required queue families (e.g., graphics and presentation).
	QueueFamilyIndices indices = FindQueueFamilies(device);

	// Retrieve properties and features of the physical device.
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	// Check if the device is a discrete GPU (preferred for performance).
	bool isDiscreteGPU = (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);

	// Verify the device supports geometry shaders (required for rendering).
	bool hasGeometryShader = deviceFeatures.geometryShader;

	// Check if the device supports the required extensions.
	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	// Verify swap chain support if extensions are available.
	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() &&
			!swapChainSupport.presentModes.empty();
	}

	// Ensure required features are supported, including anisotropic filtering.
	bool supportsAnisotropy = deviceFeatures.samplerAnisotropy;

	// A device is suitable if it satisfies all conditions.
	return indices.IsComplete() &&  // All required queue families are supported.
		   isDiscreteGPU &&         // Prefer discrete GPUs for performance.
		   hasGeometryShader &&     // Geometry shaders must be supported.
		   extensionsSupported &&   // All required extensions are supported.
		   swapChainAdequate &&     // Swap chain supports at least one format and present mode.
		   supportsAnisotropy;      // Anisotropic filtering is supported.
}
<<<<<<< HEAD


=======
>>>>>>> Testing
/**
 * @brief Finds queue families on the physical device that support required operations.
 *
 * This method identifies the indices of queue families that support graphics operations and presentation to a surface.
 * It ensures the Vulkan physical device can handle both rendering and presenting to the window.
 *
 * @param device The Vulkan physical device to query.
 *
 * @return QueueFamilyIndices containing indices for graphics and presentation queue families, if found.
 */
VulkanRenderer::QueueFamilyIndices VulkanRenderer::FindQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices; // Structure to hold the queue family indices.

	// Query the number of available queue families on the device.
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	// Retrieve properties of all available queue families.
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// Iterate over the queue families to find required capabilities.
	int i = 0; // Index of the current queue family.
	VkBool32 presentSupport = false;

	for (const auto& queueFamily : queueFamilies) {
		// Check if the queue family supports graphics operations.
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		// Check if the queue family supports presentation to the surface.
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport) {
			indices.presentFamily = i;
		}

		// Exit early if both required queue families are found.
		if (indices.IsComplete()) {
			break;
		}

		i++;  // Increment the queue family index.
	}

	return indices;  // Return the found queue family indices.
}
/**
 * @brief Queries the swap chain support details for a physical device.
 *
 * This method retrieves information about the swap chain capabilities, available surface formats,
 * and presentation modes supported by the specified physical device and surface.
 *
 * @param device The Vulkan physical device to query.
 *
 * @return SwapChainSupportDetails containing the swap chain capabilities, supported formats, and presentation modes.
 */
VulkanRenderer::SwapChainSupportDetails VulkanRenderer::QuerySwapChainSupport(VkPhysicalDevice device) {
	SwapChainSupportDetails details; // Structure to hold the swap chain support details.

	// Query the surface capabilities of the physical device.
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// Query the number of supported surface formats.
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	// If formats are available, retrieve their details.
	if (formatCount != 0) {
		details.formats.resize(formatCount); // Resize the vector to hold all formats.
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	// Query the number of supported presentation modes.
	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	// If presentation modes are available, retrieve their details.
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount); // Resize the vector to hold all presentation modes.
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details; // Return the swap chain support details.
}
<<<<<<< HEAD


=======
>>>>>>> Testing
/**
 * @brief Chooses the best surface format for the swap chain from available options.
 *
 * This method selects an optimal surface format for the swap chain, preferring
 * `VK_FORMAT_B8G8R8A8_SRGB` with `VK_COLOR_SPACE_SRGB_NONLINEAR_KHR`. If the preferred format
 * is not available, it defaults to the first available format.
 *
 * @param availableFormats A vector containing the supported surface formats for the swap chain.
 *
 * @return VkSurfaceFormatKHR The selected surface format.
 */
<<<<<<< HEAD
VkSurfaceFormatKHR VulkanRenderer::ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats) {
=======
VkSurfaceFormatKHR VulkanRenderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
>>>>>>> Testing
	// Iterate through the available formats to find the preferred one.
	for (const auto& availableFormat : availableFormats) {
		// Check if the format is VK_FORMAT_B8G8R8A8_SRGB with SRGB color space.
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat; // Return the preferred format if found.
		}
	}

	// If the preferred format is not available, fall back to the first available format.
	return availableFormats[0];
}
/**
 * @brief Chooses the best presentation mode for the swap chain from available options.
 *
 * This method selects an optimal presentation mode for the swap chain, preferring `VK_PRESENT_MODE_MAILBOX_KHR`
 * for low latency and minimal tearing. If the preferred mode is not available, it defaults to `VK_PRESENT_MODE_FIFO_KHR`,
 * which is guaranteed to be supported on all Vulkan implementations.
 *
 * @param availablePresentModes A vector containing the supported presentation modes.
 *
 * @return VkPresentModeKHR The selected presentation mode.
 */
VkPresentModeKHR VulkanRenderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	// Iterate through the available presentation modes to find the preferred mode.
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;  // Return Mailbox mode if available (low latency and tear-free).
		}
	}

	// Fallback to FIFO mode, which is always supported and ensures no tearing.
	return VK_PRESENT_MODE_FIFO_KHR;
}
/**
 * @brief Chooses the swap extent (resolution) for the swap chain.
 *
 * This method determines the dimensions of the images in the swap chain based on the surface capabilities
 * and the dimensions of the window. If the `currentExtent` is defined (not `std::numeric_limits<uint32_t>::max()`),
 * it is directly used. Otherwise, the window size is used, clamped to the allowed extent range.
 *
 * @param capabilities The surface capabilities provided by the physical device.
 *
 * @return VkExtent2D The chosen extent for the swap chain images.
 */
VkExtent2D VulkanRenderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	// If the surface has a defined extent, use it directly.
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		// Query the dimensions of the window's framebuffer.
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Create an extent using the framebuffer size.
		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		// Clamp the extent to the allowed range defined by the surface capabilities.
		actualExtent.width = std::clamp(
			actualExtent.width,
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		);
		actualExtent.height = std::clamp(
			actualExtent.height,
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		);

		return actualExtent;  // Return the clamped extent.
	}
}
/**
 * @brief Creates a Vulkan shader module from the given SPIR-V bytecode.
 *
 * This method encapsulates the creation of a Vulkan shader module, which represents a compiled shader
 * that can be used in the Vulkan pipeline. The shader module is created from SPIR-V bytecode.
 *
 * @param code A vector containing the SPIR-V bytecode for the shader.
 *
 * @return VkShaderModule The created shader module.
 *
 * @throws std::runtime_error if the shader module creation fails.
 */
VkShaderModule VulkanRenderer::CreateShaderModule(const std::vector<char>& code) {
	// Populate the shader module creation info.
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;     // Specify the structure type.
	createInfo.codeSize = code.size();                                  // Size of the SPIR-V bytecode.
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());  // Pointer to the SPIR-V code.

	VkShaderModule shaderModule;

	// Create the shader module.
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create shader module!");  // Throw an error if creation fails.
	}

	return shaderModule;  // Return the created shader module.
}
<<<<<<< HEAD


=======
>>>>>>> Testing
/**
 * @brief Recreates the swap chain and its associated resources.
 *
 * This method handles the process of recreating the Vulkan swap chain and all related resources when
 * the swap chain becomes invalid, such as during window resizing or surface changes.
 *
 * @throws std::runtime_error if any resource creation fails.
 */
void VulkanRenderer::RecreateSwapChain() {
	int width = 0, height = 0;

	// Wait until the window's framebuffer size is non-zero (e.g., when the window is minimized).
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents(); // Wait for window events (e.g., resizing) to ensure non-zero size.
	}

	// Wait for the device to become idle before modifying resources.
	vkDeviceWaitIdle(device);

	// Cleanup the existing swap chain and associated resources.
	CleanupSwapChain();

	// Recreate the swap chain and related resources.
	CreateSwapChain();       // Recreate the swap chain.
	CreateImageViews();      // Recreate image views for the swap chain images.
	CreateColorResources();  // Recreate resources for multisampling (if enabled).
	CreateDepthResources();  // Recreate the depth buffer resources.
	CreateFramebuffers();    // Recreate framebuffers for the swap chain.
<<<<<<< HEAD
=======
	CreateImGuiFramebuffers();
>>>>>>> Testing
}
/**
 * @brief Cleans up resources associated with the swap chain.
 *
 * This method releases all resources created for the swap chain, including image views,
 * framebuffers, color and depth resources, and the swap chain itself. It ensures proper
 * cleanup before recreating or destroying the swap chain.
 */
void VulkanRenderer::CleanupSwapChain() {
<<<<<<< HEAD
	// Destroy the color image resources (if multisampling is enabled).
	vkDestroyImageView(device, colorImageView, nullptr);  // Destroy the color image view.
	vkDestroyImage(device, colorImage, nullptr);          // Destroy the color image.
	vkFreeMemory(device, colorImageMemory, nullptr);      // Free the memory allocated for the color image.

	// Destroy the depth buffer resources.
	vkDestroyImageView(device, depthImageView, nullptr);  // Destroy the depth image view.
	vkDestroyImage(device, depthImage, nullptr);          // Destroy the depth image.
	vkFreeMemory(device, depthImageMemory, nullptr);      // Free the memory allocated for the depth image.

	// Destroy all framebuffers associated with the swap chain.
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
	}

	// Destroy all image views associated with the swap chain images.
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		vkDestroyImageView(device, swapChainImageViews[i], nullptr);
	}

	// Destroy the swap chain itself.
	vkDestroySwapchainKHR(device, swapChain, nullptr);
}


=======
	// --- Clean up IMGUI framebuffers ---
	for (auto framebuffer : imguiFramebuffers) {
		if (framebuffer != VK_NULL_HANDLE) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
	}
	imguiFramebuffers.clear();

	// --- Clean up color image resources (for multisampling) ---
	if (colorImageView != VK_NULL_HANDLE)
		vkDestroyImageView(device, colorImageView, nullptr);
	if (colorImage != VK_NULL_HANDLE)
		vkDestroyImage(device, colorImage, nullptr);
	if (colorImageMemory != VK_NULL_HANDLE)
		vkFreeMemory(device, colorImageMemory, nullptr);

	// --- Clean up depth resources ---
	if (depthImageView != VK_NULL_HANDLE)
		vkDestroyImageView(device, depthImageView, nullptr);
	if (depthImage != VK_NULL_HANDLE)
		vkDestroyImage(device, depthImage, nullptr);
	if (depthImageMemory != VK_NULL_HANDLE)
		vkFreeMemory(device, depthImageMemory, nullptr);

	// --- Destroy all swap chain framebuffers ---
	for (auto framebuffer : swapChainFramebuffers) {
		if (framebuffer != VK_NULL_HANDLE) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
	}
	swapChainFramebuffers.clear();

	// --- Destroy all swap chain image views ---
	for (auto imageView : swapChainImageViews) {
		if (imageView != VK_NULL_HANDLE) {
			vkDestroyImageView(device, imageView, nullptr);
		}
	}
	swapChainImageViews.clear();

	// --- Destroy the swap chain itself ---
	if (swapChain != VK_NULL_HANDLE)
		vkDestroySwapchainKHR(device, swapChain, nullptr);
}
>>>>>>> Testing
/**
 * @brief Finds a supported format from a list of candidates.
 *
 * This method iterates through a list of candidate formats and selects the first one that supports the specified
 * tiling and format features. It queries the physical device for each format's properties and checks compatibility.
 *
 * @param candidates A vector of candidate `VkFormat` values to search through.
 * @param tiling The desired image tiling mode (`VK_IMAGE_TILING_LINEAR` or `VK_IMAGE_TILING_OPTIMAL`).
 * @param features A bitmask of `VkFormatFeatureFlags` specifying the required format features (e.g., depth stencil support).
 *
 * @return VkFormat The first format that satisfies the tiling and feature requirements.
 *
 * @throws std::runtime_error if no supported format is found.
 */
VkFormat VulkanRenderer::FindSupportedFormat(
	const std::vector<VkFormat>& candidates,
	VkImageTiling tiling,
	VkFormatFeatureFlags features
) {
	// Iterate through the list of candidate formats.
	for (VkFormat format : candidates) {
		VkFormatProperties props;

		// Query the physical device for properties of the current format.
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		// Check if the format supports the required features with the specified tiling mode.
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;  // Return the format if it supports the required features.
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;  // Return the format if it supports the required features.
		}
	}

	// Throw an error if no format matches the requirements.
	throw std::runtime_error("Failed to find supported format!");
}
/**
 * @brief Finds a suitable format for the depth buffer.
 *
 * This method selects a format from a list of candidates that is supported for depth-stencil attachments
 * with optimal tiling. It leverages `FindSupportedFormat` to perform the search.
 *
 * @return VkFormat The selected depth format.
 *
 * @throws std::runtime_error if no suitable depth format is found.
 */
VkFormat VulkanRenderer::FindDepthFormat() {
	// Define a list of candidate formats for the depth buffer.
	return FindSupportedFormat(
		{
			VK_FORMAT_D32_SFLOAT,          // 32-bit floating-point depth format.
			VK_FORMAT_D32_SFLOAT_S8_UINT,  // 32-bit depth + 8-bit stencil format.
			VK_FORMAT_D24_UNORM_S8_UINT    // 24-bit depth + 8-bit stencil format.
		},
		VK_IMAGE_TILING_OPTIMAL,           // Optimal tiling for GPU usage.
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT  // Required feature: depth-stencil attachment.
	);
}
/**
 * @brief Determines the maximum usable multisample count for the physical device.
 *
 * This method queries the physical device properties to find the highest supported
 * sample count for both color and depth attachments. It returns the maximum sample count
 * that is supported by both.
 *
 * @return VkSampleCountFlagBits The maximum supported sample count.
 */
VkSampleCountFlagBits VulkanRenderer::GetMaxUsableSampleCount() {
	// Retrieve the properties of the physical device.
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	// Determine the supported sample counts for both color and depth attachments.
	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
		physicalDeviceProperties.limits.framebufferDepthSampleCounts;

	// Check for the highest supported sample count in descending order.
	if (counts & VK_SAMPLE_COUNT_64_BIT) {
		return VK_SAMPLE_COUNT_64_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_32_BIT) {
		return VK_SAMPLE_COUNT_32_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_16_BIT) {
		return VK_SAMPLE_COUNT_16_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_8_BIT) {
		return VK_SAMPLE_COUNT_8_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_4_BIT) {
		return VK_SAMPLE_COUNT_4_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_2_BIT) {
		return VK_SAMPLE_COUNT_2_BIT;
	}

	// If no higher sample count is supported, fallback to 1 (no multisampling).
	return VK_SAMPLE_COUNT_1_BIT;
}

void VulkanRenderer::SetPolygonMode(VkCommandBuffer commandBuffer, VkPolygonMode mode)
{
	auto vkCmdSetPolygonModeEXT = reinterpret_cast<PFN_vkCmdSetPolygonModeEXT>(
		vkGetDeviceProcAddr(device, "vkCmdSetPolygonModeEXT"));

	if (!vkCmdSetPolygonModeEXT) {
		throw std::runtime_error("Failed to load vkCmdSetPolygonModeEXT!");
	}

	vkCmdSetPolygonModeEXT(commandBuffer, mode);
	currentPolygonMode = mode; // Update the current mode
}





<<<<<<< HEAD

=======
>>>>>>> Testing
/**
 * @brief Creates a Vulkan debug messenger for logging and debugging purposes.
 *
 * This function dynamically loads the `vkCreateDebugUtilsMessengerEXT` function from the Vulkan instance
 * and uses it to create a debug messenger. The debug messenger is used to log validation messages and
 * debugging information during Vulkan operations.
 *
 * @param _instance The Vulkan instance with which the debug messenger is associated.
 * @param _pCreateInfo A pointer to the `VkDebugUtilsMessengerCreateInfoEXT` structure that specifies the
 *        configuration for the debug messenger.
 * @param _pAllocator An optional custom allocator for memory management, or `nullptr` to use the default allocator.
 * @param _pDebugMessenger A pointer to the `VkDebugUtilsMessengerEXT` handle where the created debug messenger will be stored.
 *
 * @return VkResult `VK_SUCCESS` if the debug messenger was successfully created, or `VK_ERROR_EXTENSION_NOT_PRESENT`
 *         if the debug utils extension is not available.
 */
VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(
	VkInstance _instance,
	const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo,
	const VkAllocationCallbacks* _pAllocator,
	VkDebugUtilsMessengerEXT* _pDebugMessenger
) {
	// Dynamically retrieve the vkCreateDebugUtilsMessengerEXT function from the Vulkan instance.
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	// Check if the function is available and call it if it exists.
	if (func != nullptr) {
		return func(_instance, _pCreateInfo, _pAllocator, _pDebugMessenger);
	}
	else {
		// Return an error if the extension is not present or the function could not be loaded.
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}
/**
 * @brief Destroys a Vulkan debug messenger.
 *
 * This function dynamically loads the `vkDestroyDebugUtilsMessengerEXT` function from the Vulkan instance
 * and uses it to destroy an existing debug messenger. It ensures proper cleanup of resources associated
 * with the debug messenger.
 *
 * @param _instance The Vulkan instance with which the debug messenger is associated.
 * @param _pDebugMessenger The debug messenger to destroy.
 * @param _pAllocator An optional custom allocator for memory management, or `nullptr` to use the default allocator.
 */
void VulkanRenderer::DestroyDebugUtilsMessengerEXT(
	VkInstance _instance,
	VkDebugUtilsMessengerEXT _pDebugMessenger,
	const VkAllocationCallbacks* _pAllocator
) {
	// Dynamically retrieve the vkDestroyDebugUtilsMessengerEXT function from the Vulkan instance.
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");

	// Check if the function is available and call it to destroy the debug messenger.
	if (func != nullptr) {
		func(_instance, _pDebugMessenger, _pAllocator);
	}
}
/**
 * @brief Debug callback function for Vulkan validation layers.
 *
 * This function is used by Vulkan's debug utilities to handle validation layer messages.
 * It processes messages based on their severity and type, optionally filtering or logging them.
 *
 * @param _messageSeverity The severity of the debug message (e.g., warning, error).
 * @param _messageType The type of the debug message (e.g., general, validation, performance).
 * @param _pCallbackData A pointer to a `VkDebugUtilsMessengerCallbackDataEXT` structure containing details about the message.
 * @param _pUserData A pointer to user-defined data, passed when setting up the debug messenger.
 *
 * @return VkBool32 VK_FALSE to indicate Vulkan should not abort the operation, allowing execution to continue.
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
	VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT _messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
	void* _pUserData
) {
	// Extract the debug message from the callback data.
	std::string message = _pCallbackData->pMessage;

	// Filter out specific messages based on known substrings.
	if (message.find("loader_add_layer_properties") != std::string::npos ||
		message.find("dzn_icd") != std::string::npos) {
		return VK_FALSE;  // Ignore and do not log these specific messages.
	}

	// Log the validation message to the standard error output.
	std::cerr << "Validation Layer: " << message << "\n";

	return VK_FALSE;  // Do not abort Vulkan operations; execution continues.
}
/**
 * @brief Populates a `VkDebugUtilsMessengerCreateInfoEXT` structure for setting up a Vulkan debug messenger.
 *
 * This function initializes a `VkDebugUtilsMessengerCreateInfoEXT` structure with configuration details
 * for a debug messenger, including message severities, message types, and the callback function.
 *
 * @param _createInfo A reference to the `VkDebugUtilsMessengerCreateInfoEXT` structure to populate.
 */
void VulkanRenderer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo) {
	// Initialize the structure with default values.
	_createInfo = {};
	_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;  // Specify the structure type.

	// Set the message severity levels to capture.
	_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |  // Warnings.
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;  // Errors.
	// Optionally include verbose messages:
	// | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

// Set the types of messages to capture.
	_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |  // General messages.
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |  // Validation errors and warnings.
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;  // Performance-related messages.

	// Set the callback function to handle debug messages.
	_createInfo.pfnUserCallback = debugCallBack;

	// Optional user data pointer (not used here, can pass custom data if needed).
	_createInfo.pUserData = nullptr;
}




/**
 * @brief Callback function for handling framebuffer resize events.
 *
 * This function is called by GLFW whenever the window's framebuffer is resized. It updates the
 * `framebufferResized` flag in the VulkanRenderer instance to indicate that the swap chain needs to be
 * recreated.
 *
 * @param window A pointer to the GLFW window that was resized.
 * @param width The new width of the framebuffer.
 * @param height The new height of the framebuffer.
 */
void VulkanRenderer::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	// Retrieve the VulkanRenderer instance associated with the GLFW window.
	auto app = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));

	// Set the framebufferResized flag to true to signal that the swap chain needs to be recreated.
	app->framebufferResized = true;
}
void VulkanRenderer::MouseCallback(double xpos, double ypos) {
	static bool firstMouse = true;
	static float lastX = 0.0f, lastY = 0.0f;

	if (isCursorLocked) {
		if (firstMouse) {
			lastX = static_cast<float>(xpos);
			lastY = static_cast<float>(ypos);
			firstMouse = false;
		}

		// Calculate mouse offsets
		float xOffset = static_cast<float>(xpos) - lastX;
		float yOffset = lastY - static_cast<float>(ypos); // Inverted Y for Vulkan
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);

		// Pass offsets to the camera
		camera->ProcessMouseMovement(xOffset, yOffset);
	}
	else {
		firstMouse = true;
	}
}

