#pragma once

#include "Utilities.h"


/**
 * @file Model.h
 * @brief Defines the Model class and its associated structures for managing 3D models and their transformations in Vulkan.
 */

 /**
  * @brief Struct for passing small, frequently updated data to shaders via push constants.
  *
  * Push constants allow shaders to receive small amounts of data efficiently.
  */
struct PushConstants {
    glm::mat4 model; ///< Model transformation matrix (position, rotation, and scale).
};

/**
 * @class Model
 * @brief Represents a 3D model with geometry, transformations, and textures for rendering in Vulkan.
 */
class Model {
public:
    Model(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);
    ~Model();

    void Bind(VkCommandBuffer commandBuffer);
    void Draw(VkCommandBuffer commandBuffer);

    void LoadFromFile(const std::string& filepath);
    void LoadTexture(const std::string& texturePath);

    VkImageView GetTextureImageView();
    VkSampler GetTextureSampler();

    void CreateTextureImage(const std::string& texturePath);
    void CreateTextureImageView();
    void CreateTextureSampler();

    // === Transformation Methods ===
    void SetPosition(const glm::vec3& position);
    glm::vec3 GetPosition();

    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);

    glm::mat4 GetModelMatrix() const;

private:
    // Vulkan handles
    VkDevice device = VK_NULL_HANDLE;                 ///< Vulkan logical device handle.
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; ///< Vulkan physical device handle.
    VkQueue graphicsQueue = VK_NULL_HANDLE;           ///< Vulkan queue for graphics commands.
    VkCommandPool commandPool = VK_NULL_HANDLE;       ///< Vulkan command pool for command buffers.

    // Geometry data
    std::vector<Vertex> vertices;  ///< Vertex data for the model.
    std::vector<uint32_t> indices; ///< Index data for the model.
    uint32_t vertexCount = 0;      ///< Number of vertices.
    uint32_t indexCount = 0;       ///< Number of indices.

    // Buffers
    VkBuffer vertexBuffer = VK_NULL_HANDLE;             ///< Vulkan vertex buffer.
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE; ///< Memory for the vertex buffer.
    VkBuffer indexBuffer = VK_NULL_HANDLE;              ///< Vulkan index buffer.
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;  ///< Memory for the index buffer.

    // Transformation properties
    glm::vec3 position{ 0.0f };    ///< Model position in world space.
    glm::vec3 rotation{ 0.0f };    ///< Model rotation (in degrees) around each axis.
    glm::vec3 scale{ 1.0f };       ///< Model scale along each axis.
    glm::mat4 modelMatrix{ 1.0f }; ///< Combined transformation matrix.

    // Texture-related resources
    VkImage textureImage = VK_NULL_HANDLE;              ///< Vulkan image for the texture.
    VkDeviceMemory textureImageMemory = VK_NULL_HANDLE; ///< Memory for the texture image.
    VkImageView textureImageView = VK_NULL_HANDLE;      ///< Vulkan image view for the texture.
    VkSampler textureSampler = VK_NULL_HANDLE;          ///< Vulkan sampler for the texture.
    uint32_t mipLevels = 0;                             ///< Number of mipmap levels for the texture.

    // Private methods for internal functionality
    void LoadOBJ(const std::string& filepath); ///< Loads geometry from an OBJ file.
    void LoadFBX(const std::string& filepath); ///< Loads geometry from an FBX file.
    void CreateVertexBuffer(); ///< Creates the Vulkan vertex buffer.
    void CreateIndexBuffer();  ///< Creates the Vulkan index buffer.
    void UpdateModelMatrix();  ///< Updates the model's transformation matrix.
    void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels); ///< Generates mipmaps for the texture.
};


