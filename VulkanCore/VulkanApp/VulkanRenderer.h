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
<<<<<<< HEAD
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
=======
 *
 * This class encapsulates the core Vulkan setup, main rendering pipeline,
 * and a separate IMGUI debug pipeline. The header is optimized to follow
 * modern C++ best practices: it minimizes header dependencies by using forward
 * declarations and includes only what is necessary.
 */
class VulkanRenderer {
public:
    // Public Methods (interface)
    void Run();
    void Update(float deltaTime);
    void AddModel(const std::string& modelPath, const std::string& texturePath);
    //void RemoveModel(uint32_t index);
    //void UpdateDescriptors();

private:
    // ====================================================
    // Configuration & Constants
    // ====================================================
    static constexpr char APP_NAME[] = "-PTE-";
    static constexpr uint32_t WIDTH = 1280;
    static constexpr uint32_t HEIGHT = 720;
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
#ifdef NDEBUG
    static constexpr bool enableValidationLayer = false;
#else
    static constexpr bool enableValidationLayer = true;
#endif

>>>>>>> Testing
    bool isCursorLocked = false;
    const std::vector<const char*> validationLayers{
        "VK_LAYER_LUNARG_monitor",
        "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char*> deviceExtensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_EXT_extended_dynamic_state3"
    };
<<<<<<< HEAD
#ifdef NDEBUG
    static constexpr bool enableValidationLayer = false;
#else
    static constexpr bool enableValidationLayer = true;
#endif

    // === Core Vulkan Objects ===
=======

    // ====================================================
    // Core Vulkan Objects & Window
    // ====================================================
>>>>>>> Testing
    GLFWwindow* window = nullptr;
    VkInstance instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
<<<<<<< HEAD

    // Queues
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    // Swap Chain
=======
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    // ====================================================
    // Swap Chain & Main Rendering Pipeline
    // ====================================================
>>>>>>> Testing
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;

<<<<<<< HEAD
    // Rendering Pipeline
=======
>>>>>>> Testing
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> swapChainFramebuffers;

<<<<<<< HEAD
    // Command Buffers
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;

    // Synchronization Objects
    const int MAX_FRAMES_IN_FLIGHT = 2;
=======
    // ====================================================
    // IMGUI Debug Pipeline (implementation details moved to .cpp)
    // ====================================================
    VkRenderPass imguiRenderPass = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> imguiFramebuffers;

    // ====================================================
    // Command Pool & Buffers
    // ====================================================
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;

    // ====================================================
    // Synchronization Objects
    // ====================================================
>>>>>>> Testing
    uint32_t currentFrame = 0;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    bool framebufferResized = false;

<<<<<<< HEAD
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
=======
    // ====================================================
    // Memory Resources & Buffers
    // ====================================================
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    VkImage colorImage = VK_NULL_HANDLE;
    VkDeviceMemory colorImageMemory = VK_NULL_HANDLE;
    VkImageView colorImageView = VK_NULL_HANDLE;
    VkImage depthImage = VK_NULL_HANDLE;
    VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
    VkImageView depthImageView = VK_NULL_HANDLE;

>>>>>>> Testing
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets;

    // === Configuration Values ===
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkPolygonMode currentPolygonMode = VK_POLYGON_MODE_FILL;

<<<<<<< HEAD
    // === Time Tracking ===
=======
    // ====================================================
    // Timing and Performance Metrics
    // ====================================================
>>>>>>> Testing
    float deltaTime = 0.0f;
    float fps = 0.0f;
    float elapsedTime = 0.0f;
    uint64_t frameCount = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;

<<<<<<< HEAD
    // === Models and Resources ===
=======
    // ====================================================
    // Models, Camera, and Resources
    // ====================================================
>>>>>>> Testing
    const std::string MODEL_PATH = "VulkanModels/viking_room.obj";
    const std::string TEXTURE_PATH = "VulkanTextures/viking_room.png";
    std::vector<std::unique_ptr<Model>> modelList;
    std::unique_ptr<Camera> camera;

<<<<<<< HEAD
    // === Utility Structures ===
=======
    // ====================================================
    // Utility Structures
    // ====================================================
>>>>>>> Testing
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool IsComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
<<<<<<< HEAD

=======
>>>>>>> Testing
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
<<<<<<< HEAD

=======
>>>>>>> Testing
    struct UniformBufferObject {
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

<<<<<<< HEAD
    // === Core Methods ===
=======
    // ====================================================
    // Initialization Methods
    // ====================================================
>>>>>>> Testing
    void InitWindow();
    void InitImGui();
    void InitVulkan();
    void MainLoop();
    void CleanUp();

<<<<<<< HEAD
    // Vulkan Setup Methods
=======
    // ====================================================
    // Vulkan Setup Methods
    // ====================================================
>>>>>>> Testing
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
<<<<<<< HEAD
=======
    void CreateImGuiRenderPass();
    void CreateImGuiFramebuffers();
>>>>>>> Testing
    void CreateCommandPool();
    void CreateColorResources();
    void CreateDepthResources();
    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
    void CreateCommandBuffers();
    void CreateSyncObjects();

<<<<<<< HEAD
    // Drawing Methods
    void LoadModels();
=======
    // ====================================================
    // Rendering & Drawing Methods
    // ====================================================
    void RenderImGui(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void LoadDefualtModels();
>>>>>>> Testing
    void UpdateUniformBuffer(uint32_t currentImage);
    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void DrawFrame();

<<<<<<< HEAD
    // Utility Methods
=======
    // ====================================================
    // Utility Methods
    // ====================================================
>>>>>>> Testing
    void AssignDebugNames();
    std::vector<const char*> GetRequiredExtensions();
    bool CheckValidationLayerSupport();
    bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    bool IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
<<<<<<< HEAD
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);
=======
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
>>>>>>> Testing
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkShaderModule CreateShaderModule(const std::vector<char>& code);
    void CleanupSwapChain();
    void RecreateSwapChain();
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat FindDepthFormat();
    VkSampleCountFlagBits GetMaxUsableSampleCount();
    void SetPolygonMode(VkCommandBuffer commandBuffer, VkPolygonMode mode);

<<<<<<< HEAD
    // Debug Utilities
=======
    // ====================================================
    // Debug Utilities
    // ====================================================
>>>>>>> Testing
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* allocator);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    void MouseCallback(double xpos, double ypos);
};



