#pragma once

// Vulkan and GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>  

// GLM for matrix and vector operations
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/hash.hpp>
#define GLM_FORCE_RADIANS            
#define GLM_FORCE_DEPTH_ZERO_TO_ONE  
#include "glm.hpp"

#include <gtc/matrix_transform.hpp>
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


	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 textCoor;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, textCoor);

			return attributeDescriptions;
		}

		bool operator==(const Vertex& other) const {
			return pos == other.pos && color == other.color && textCoor == other.textCoor;
		}
	};
private:
	// === Constants ===	
	const std::string appName = "Hoe";
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;

	// === 3D Model === 
	const std::string MODEL_PATH = "../VulkanModels/viking_room.obj";
	const std::string TEXTURE_PATH = "../VulkanTextures/viking_room.png";

	// === GLFW Window ===
	GLFWwindow* window = nullptr;

	// === Vulkan Core Objects ===
	VkInstance instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;

	// === Queues ===
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	// === Swap Chain ===
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	// === Rendering Pipeline ===
	VkRenderPass renderPass = VK_NULL_HANDLE;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	// === Command Buffers ===
	VkCommandPool commandPool = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> commandBuffers;

	//  === Maximum number of frames that can be processed concurrently ===
	const int MAX_FRAMES_IN_FLIGHT = 2;
	uint32_t currentFrame = 0;

	// === Synchronization Objects === 
	std::vector<VkSemaphore> imageAvailableSemaphores{};
	std::vector<VkSemaphore> renderFinishedSemaphores{};
	std::vector<VkFence> inFlightFences{};

	bool framebufferResized = false;

	// === Memory Buffers ===
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;
	VkImage textureImage;
	uint32_t mipLevels;		
	VkDeviceMemory textureImageMemory;
	VkImage colorImage;					// ** NEW TUT ** //
	VkDeviceMemory colorImageMemory;	// ** NEW TUT ** //
	VkImageView colorImageView;			// ** NEW TUT ** //


	// === Descriptor Pool & Sets ===
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	// === Textures ===
	VkImageView textureImageView;
	VkSampler textureSampler;

	// === Depth Buffer
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

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
	struct UniformBufferObject {
		alignas(16)glm::mat4 model;
		alignas(16)glm::mat4 view;
		alignas(16)glm::mat4 proj;
	};

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	// === Configuration Constants ===
	const std::vector<const char*> validationLayers{
		"VK_LAYER_LUNARG_monitor",     
		"VK_LAYER_KHRONOS_validation"  
	};
	const std::vector<const char*> deviceExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME  
	};
#ifdef NDEBUG
	static constexpr bool enableValidationLayer = false;  ///< Disable validation layers in release mode
#else
	static constexpr bool enableValidationLayer = true;   ///< Enable validation layers in debug mode
#endif


	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	

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
	void GetPhysicalDevice();       // ** NEW TUT ** //
	void CreateLogicalDevice();     
	void CreateSwapChain();		    
	void CreateImageViews();	     
	void CreateRenderPass();
	void CreateDescriptorSetLayout();  
	void CreateGraphicsPipeline();  
	void CreateFramebuffers();		
	void CreateCommandPool();
	void CreateColorResources();    // ** NEW TUT ** //
	void CreateDepthResources();	
	void CreateTextureImage();		
	void CreateTextureImageView();	
	void CreateTextureSampler();	
	void CreateVertexBuffer();
	void CreateIndexBuffer();  
	void CreateUniformBuffers();  
	void CreateDescriptorPool();  
	void CreateDescriptorSets();  
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
	void UpdateUniformBuffer(uint32_t currentImage);
	// ** NEW TUT ** //
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
		VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image, 
		VkDeviceMemory& imageMemory);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void TransitionImageLayout(VkImage image, VkFormat format,
		VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t
		width, uint32_t height);
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
		uint32_t mipLevels);
	VkFormat FindSupportedFormat(const std::vector<VkFormat>&
		candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat format);
	void LoadModel();
	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, 
		int32_t texHeight, uint32_t mipLevels);
	// ** NEW TUT ** //
	VkSampleCountFlagBits GetMaxUsableSampleCount();




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


namespace std {
	template<> struct hash<VulkanRenderer::Vertex> {
		size_t operator()(VulkanRenderer::Vertex const& vertex) const {
		 return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.textCoor) << 1);
		}
	};
}
