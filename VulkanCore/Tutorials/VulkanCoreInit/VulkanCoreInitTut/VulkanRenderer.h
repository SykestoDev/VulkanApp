#pragma once

// Vulkan and GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>  // GLFW for Vulkan integration

// GLM for matrix and vector operations
#define GLM_FORCE_RADIANS            // Use radians for angles
#define GLM_FORCE_DEPTH_ZERO_TO_ONE  // Match Vulkan's depth range [0, 1]
#include <vec4.hpp>                  // GLM vector library
#include <mat4x4.hpp>                // GLM matrix library

// Standard Libraries for debugging and error handling
#include <cstring>        // For string manipulation
#include <unordered_set>  // For managing unique sets (e.g., validation layers)
#include <stdexcept>      // For exceptions
#include <optional>       // For optional values, used in queue families
#include <cstdlib>        // For utility functions like memory allocation
#include <iostream>       // For logging and debugging


/**
 * @class VulkanRenderer
 * @brief Handles Vulkan initialization, rendering loop, and resource cleanup.
 */
class VulkanRenderer {
public:
	// Main function
	void Run();  // Starts the Vulkan Application

	
private:
	// === Constants ===	
	const std::string appName = "Hoe";  // Name of the application, used in Vulkan initialization
	const uint32_t WIDTH = 800;         // Default width of the application window
	const uint32_t HEIGHT = 600;        // Default height of the application window

	// === GLFW Window ===
	GLFWwindow* window = nullptr;       // Pointer to the GLFW window instance

	// === Vulkan Core Objects ===
	VkInstance instance = VK_NULL_HANDLE;                      ///< Vulkan instance handle
	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;  ///< Debug messenger for validation layers
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;          ///< Selected physical GPU
	VkDevice device = VK_NULL_HANDLE;                          ///< Logical device interface
	VkSurfaceKHR surface;                                      ///< Vulkan rendering surface

	// Queues
	VkQueue graphicsQueue;  ///< Graphics command queue
	VkQueue presentQueue;   ///< Presentation queue

	// Swap Chain
	VkSwapchainKHR swapChain;					   ///< Swap chain handle
	std::vector<VkImage> swapChainImages;		   ///< Images in the swap chain
	VkFormat swapChainImageFormat;				   ///< Swap chain image format
	VkExtent2D swapChainExtent;					   ///< Swap chain image resolution
	std::vector<VkImageView> swapChainImageViews;  ///< Image views for swap chain images

	// Rendering Pipleline
	VkRenderPass renderPass = VK_NULL_HANDLE;          ///< Render pass object
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;  ///< Pipeline layout
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;	   ///< Graphics pipeline
	std::vector<VkFramebuffer> swapChainFramebuffers;  ///< Framebuffers for each swap chain image

	// Command Buffers
	VkCommandPool commandPool = VK_NULL_HANDLE;	  ///< Command pool for allocating command buffers
	std::vector<VkCommandBuffer> commandBuffers;  ///< Primary command buffer

	// Maximum number of frames that can be processed concurrently
	const int MAX_FRAMES_IN_FLIGHT = 2;
	uint32_t currentFrame = 0;

	// Synchronization Objects
	std::vector<VkSemaphore> imageAvailableSemaphores{};  ///< Semaphore for image availability
	std::vector<VkSemaphore> renderFinishedSemaphores{};  ///< Semaphore for render completion
	std::vector<VkFence> inFlightFences{};				  ///< Fence for frame synchronization

	// === Utility Structures ===
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily{};  ///< Graphics queue family index
		std::optional<uint32_t> presentFamily{};   ///< Presentation queue family index

		QueueFamilyIndices() = default;

		/**
		 * @brief Checks if the queue family indices are complete.
		 * @return true if both graphics and presentation families are available.
		 */
		bool IsComplete() const {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities{};	 ///< Surface capabilities
		std::vector<VkSurfaceFormatKHR> formats;	 ///< Supported surface formats
		std::vector<VkPresentModeKHR> presentModes;  ///< Supported presentation modes
	};

	// === Configuration Constants ===
	const std::vector<const char*> validationLayers{
		"VK_LAYER_LUNARG_monitor",     ///< Validation layer for Vulkan debugging and monitoring
		"VK_LAYER_KHRONOS_validation"  ///< Standard validation layer by Khronos
	};

	// Required Extensions 
	const std::vector<const char*> deviceExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME  ///< Swap chain extension for rendering
	};
#ifdef NDEBUG
	static constexpr bool enableValidationLayer = false;  ///< Disable validation layers in release mode
#else
	static constexpr bool enableValidationLayer = true;   ///< Enable validation layers in debug mode
#endif



	// === Core Methods ===
	void InitWindow();  ///< Initializes the GLFW window
	void InitVulkan();  ///< Initializes Vulkan resources
	void MainLoop();    ///< Main rendering loop	
	void CleanUp();     ///< Releases all Vulkan and GLFW resources


	// === Vulkan Setup Methods ===
	void CreateInstance();          
	void SetupDebugMessenger();     
	void CreateSurface();           
	void GetPhysicalDevice();       
	void CreateLogicalDevice();     
	void CreateSwapChain();		    
	void CreateImageViews();	    
	void CreateRenderPass();		
	void CreateGraphicsPipeline();  
	void CreateFramebuffers();		
	void CreateCommandPool();		
	void CreateCommandBuffers();		
	void CreateSyncObjects();		




	// === Utility Methods ===
	std::vector<const char*> GetRequiredExtensions();													 
	bool CheckValidationLayerSupport();																	 
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);						 
	bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);									 
	bool IsDeviceSuitable(VkPhysicalDevice device);														 
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);										 
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);								 
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableSFormats);      
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);  
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);						     
	VkShaderModule CreateShaderModule(const std::vector<char>& code);									 
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);						 
	void DrawFrame();




	// === Debug Utilities ===
	VkResult CreateDebugUtilsMessengerEXT(  ///< Dynamically creates a Vulkan debug messenger for capturing validation messages
		VkInstance _instance,
		const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo,
		const VkAllocationCallbacks* _pAllocator,
		VkDebugUtilsMessengerEXT* _pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(     ///< Destroys the Vulkan debug messenger to release resources
		VkInstance _instance,
		VkDebugUtilsMessengerEXT _pDebugMessenger,
		const VkAllocationCallbacks* _pAllocator);
	// Fills out the debug messenger creation info with desired message severities and types
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
};

