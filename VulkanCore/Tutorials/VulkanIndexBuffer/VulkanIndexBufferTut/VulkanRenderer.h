#pragma once

// Vulkan and GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>  
#include "glm.hpp"

// GLM for matrix and vector operations
#define GLM_FORCE_RADIANS            
#define GLM_FORCE_DEPTH_ZERO_TO_ONE  
#include <vec4.hpp>                  
#include <mat4x4.hpp>                

// Standard Libraries for debugging and error handling
#include <cstring>      
#include <array>
#include <unordered_set>  
#include <stdexcept>      
#include <optional>       
#include <cstdlib>        
#include <iostream>       



/**
 * @class VulkanRenderer
 * @brief Handles Vulkan initialization, rendering loop, and resource cleanup.
 */
class VulkanRenderer {
public:
	void Run();  

	
private:
	// === Constants ===	
	const std::string appName = "Hoe";  
	const uint32_t WIDTH = 800;         
	const uint32_t HEIGHT = 600;        

	// === GLFW Window ===
	GLFWwindow* window = nullptr;       

	// === Vulkan Core Objects ===
	VkInstance instance = VK_NULL_HANDLE;                      
	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;  
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;          
	VkDevice device = VK_NULL_HANDLE;                          
	VkSurfaceKHR surface;                                      

	// Queues
	VkQueue graphicsQueue;  
	VkQueue presentQueue;   

	// Swap Chain
	VkSwapchainKHR swapChain;					   
	std::vector<VkImage> swapChainImages;		   
	VkFormat swapChainImageFormat;				   
	VkExtent2D swapChainExtent;					   
	std::vector<VkImageView> swapChainImageViews;  

	// Rendering Pipeline
	VkRenderPass renderPass = VK_NULL_HANDLE;          
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;  
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;	   
	std::vector<VkFramebuffer> swapChainFramebuffers;  

	// Command Buffers
	VkCommandPool commandPool = VK_NULL_HANDLE;	  
	std::vector<VkCommandBuffer> commandBuffers;  

	// Maximum number of frames that can be processed concurrently
	const int MAX_FRAMES_IN_FLIGHT = 2;
	uint32_t currentFrame = 0;

	// Synchronization Objects
	std::vector<VkSemaphore> imageAvailableSemaphores{};  
	std::vector<VkSemaphore> renderFinishedSemaphores{};  
	std::vector<VkFence> inFlightFences{};	

	bool framebufferResized = false;

	// === Utility Structures ===
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily{};  
		std::optional<uint32_t> presentFamily{};   

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
		VkSurfaceCapabilitiesKHR capabilities{};	 
		std::vector<VkSurfaceFormatKHR> formats;	 
		std::vector<VkPresentModeKHR> presentModes;  
	};

	// === Configuration Constants ===
	const std::vector<const char*> validationLayers{
		"VK_LAYER_LUNARG_monitor",     
		"VK_LAYER_KHRONOS_validation"  
	};

	// Required Extensions 
	const std::vector<const char*> deviceExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME  
	};
#ifdef NDEBUG
	static constexpr bool enableValidationLayer = false;  ///< Disable validation layers in release mode
#else
	static constexpr bool enableValidationLayer = true;   ///< Enable validation layers in debug mode
#endif

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	// ** NEW TUT ** //
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;


	struct Vertex {
		glm::vec2 pos;
		glm::vec3 color;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}
		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			return attributeDescriptions;
		}
	};

	// ** NEW TUT ** //
	const std::vector<Vertex> vertices = {
		{{-0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f,-0.5f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	// ** NEW TUT ** //
	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};



	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	// === Core Methods ===
	void InitWindow();  
	void InitVulkan();  
	void MainLoop();    
	void CleanUp();     




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
	void CreateVertexBuffer();
	void createIndexBuffer();  // ** NEW TUT ** //
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
	void RecreateSwapChain();  
	void CleanupSwapChain(); 
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize
		size);



	// === Debug Utilities ===
	VkResult CreateDebugUtilsMessengerEXT(  
		VkInstance _instance,
		const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo,
		const VkAllocationCallbacks* _pAllocator,
		VkDebugUtilsMessengerEXT* _pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(     
		VkInstance _instance,
		VkDebugUtilsMessengerEXT _pDebugMessenger,
		const VkAllocationCallbacks* _pAllocator);
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
};

