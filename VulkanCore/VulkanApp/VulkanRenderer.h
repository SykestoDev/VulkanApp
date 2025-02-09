#pragma once

// Standard Libraries
#include <array>
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

// Third-party Libraries
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include <GLFW/glfw3.h>

// Project Headers
#include "Camera.h"
#include "Model.h"
#include "Utilities.h"

/**
 * @class VulkanRenderer
 * @brief Handles Vulkan initialization, rendering loop, and resource cleanup.
 */
class VulkanRenderer {
public:
    // === Public Methods ===
    void Run();
    void Update(float deltaTime);

private:
    // === Constants and Configuration ===
    const std::string appName = "-PTE-";
    const uint32_t WIDTH = 1280;
    const uint32_t HEIGHT = 720;
    bool isCursorLocked = false;
    const std::vector<const char*> validationLayers{
        "VK_LAYER_LUNARG_monitor",
        "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char*> deviceExtensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_EXT_extended_dynamic_state3"
    };
#ifdef NDEBUG
    static constexpr bool enableValidationLayer = false;
#else
    static constexpr bool enableValidationLayer = true;
#endif

    // === Core Vulkan Objects ===
    GLFWwindow* window = nullptr;
    VkInstance instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    // Queues
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    // Swap Chain
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;

    // Rendering Pipeline
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    // Command Buffers
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;

    // Synchronization Objects
    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    bool framebufferResized = false;

    // Memory Buffers
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    // Descriptor Pool & Sets
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets;

    // === Configuration Values ===
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkPolygonMode currentPolygonMode = VK_POLYGON_MODE_FILL;

    // === Time Tracking ===
    float deltaTime = 0.0f;
    float fps = 0.0f;
    float elapsedTime = 0.0f;
    uint64_t frameCount = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;

    // === Models and Resources ===
    const std::string MODEL_PATH = "VulkanModels/viking_room.obj";
    const std::string TEXTURE_PATH = "VulkanTextures/viking_room.png";
    std::vector<std::unique_ptr<Model>> modelList;
    std::unique_ptr<Camera> camera;

    // === Utility Structures ===
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool IsComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct UniformBufferObject {
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    // === Core Methods ===
    void InitWindow();
    void InitImGui();
    void InitVulkan();
    void MainLoop();
    void CleanUp();

    // Vulkan Setup Methods
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void GetPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateFramebuffers();
    void CreateCommandPool();
    void CreateColorResources();
    void CreateDepthResources();
    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
    void CreateCommandBuffers();
    void CreateSyncObjects();

    // Drawing Methods
    void LoadModels();
    void UpdateUniformBuffer(uint32_t currentImage);
    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void DrawFrame();

    // Utility Methods
    void AssignDebugNames();
    std::vector<const char*> GetRequiredExtensions();
    bool CheckValidationLayerSupport();
    bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    bool IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkShaderModule CreateShaderModule(const std::vector<char>& code);
    void CleanupSwapChain();
    void RecreateSwapChain();
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat FindDepthFormat();
    VkSampleCountFlagBits GetMaxUsableSampleCount();
    void SetPolygonMode(VkCommandBuffer commandBuffer, VkPolygonMode mode);

    // Debug Utilities
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* allocator);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    void MouseCallback(double xpos, double ypos);
};



