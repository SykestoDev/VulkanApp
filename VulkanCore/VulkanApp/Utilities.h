#pragma once

// Vulkan
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

// Hash enable
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/hash.hpp>

// GLM for matrix and vector operations
#define GLM_FORCE_RADIANS            
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#include <glm.hpp> // For vec2, vec3, etc.
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/string_cast.hpp>

// Standard Libraries
#include <string>
#include <fstream>	 
#include <chrono>
#include <vector>
#include <stdexcept> 
#include <iostream>



/* 
    Vertex structure
*/
struct Vertex {
    glm::vec3 pos;       // Position
    glm::vec3 color;     // Color
    glm::vec2 textCoor;  // Texture coordinate

    // Define the binding description for this struct
    static VkVertexInputBindingDescription GetBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    // Define the attribute descriptions for this struct
    static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

        // Position attribute
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        // Color attribute
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        // Texture coordinate attribute
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

/* 
    Specialization of std::hash for the Vertex struct 
    This allows Vertex objects to be used as keys in unordered containers, such as std::unordered_map.
*/
namespace std {
    template<> struct hash<Vertex> {
        // Custom hash function for the Vertex struct
        size_t operator()(Vertex const& vertex) const {
            // Combine the hash values of the vertex attributes (position, color, texture coordinates)
            return ((hash<glm::vec3>()(vertex.pos) ^                   // Hash the position
                    (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^    // Combine with the color hash
                    (hash<glm::vec2>()(vertex.textCoor) << 1);         // Combine with the texture coordinate hash
        }
    };
}

// Helper function to check Vulkan result
inline static void check_vk_result(VkResult err) {
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

/*
    Utility function to read the contents of a file into a byte buffer.
    This function is commonly used to load binary files such as shaders or assets into memory.
*/
static std::vector<char> readFile(const std::string& filename) {
    // Open the file in binary mode and position the file pointer at the end.
    // The `ate` flag ensures we can determine the file size by its end position.
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    // Check if the file was successfully opened.
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file!");  // Throw an error if the file couldn't be opened.
    }

    // Get the size of the file by reading the current position of the file pointer.
    size_t fileSize = static_cast<size_t>(file.tellg());

    // Allocate a buffer large enough to hold the entire file content.
    std::vector<char> buffer(fileSize);

    // Move the file pointer back to the beginning of the file.
    file.seekg(0);

    // Read the entire file content into the buffer.
    file.read(buffer.data(), fileSize);

    // Close the file after reading.
    file.close();

    // Return the buffer containing the file data.
    return buffer;
}

/*
    Utility function to find a suitable memory type.
    This function determines the appropriate memory type index for a Vulkan buffer or image.
    It ensures that the memory meets both the type filter and property requirements.
*/
inline uint32_t findMemoryType(
    VkPhysicalDevice physicalDevice,  // The physical device to query memory properties from.
    uint32_t typeFilter,              // A bitmask specifying which memory types are suitable.
    VkMemoryPropertyFlags properties  // Required memory properties (e.g., host-visible, device-local).
) {
    // Retrieve memory properties for the physical device.
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    // Iterate through all memory types available on the physical device.
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        // Check if the memory type is acceptable based on the type filter.
        if ((typeFilter & (1 << i)) &&
            // Verify that the memory type has all the required properties.
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i; // Return the index of the suitable memory type.
        }
    }

    // If no suitable memory type is found, throw an error.
    throw std::runtime_error("failed to find suitable memory type!");
}

/*
    Utility function to create a Vulkan buffer.
    This function encapsulates buffer creation, memory allocation, and binding, making it reusable.
    It helps manage Vulkan's low-level API complexity.
*/
inline void createBuffer(
    VkDevice device,                   // Logical device handle.
    VkPhysicalDevice physicalDevice,   // Physical device handle to query memory properties.
    VkDeviceSize size,                 // Size of the buffer in bytes.
    VkBufferUsageFlags usage,          // Intended usage of the buffer (e.g., vertex, index).
    VkMemoryPropertyFlags properties,  // Memory properties (e.g., host-visible, device-local).
    VkBuffer& buffer,                  // Output buffer handle.
    VkDeviceMemory& bufferMemory       // Output memory handle for the buffer.
) {
    // Define the buffer creation information structure.
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO; // Structure type.
    bufferInfo.size = size;                                  // Size of the buffer.
    bufferInfo.usage = usage;                                // Usage flags for the buffer.
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;      // Only accessed by a single queue family.

    // Create the buffer and check for errors.
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    // Retrieve memory requirements for the created buffer.
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    // Define the memory allocation information structure.
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;  // Structure type.
    allocInfo.allocationSize = memRequirements.size;           // Required allocation size.
    // Find a suitable memory type index.
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

    // Allocate memory for the buffer and check for errors.
    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    // Bind the allocated memory to the buffer.
    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}




/*
    Utility function to begin a single-time command buffer.
    This function creates and begins recording a primary command buffer intended for one-time use.
    It is commonly used for transient operations like resource transfers or pipeline barriers.
*/
inline VkCommandBuffer beginSingleTimeCommands(
    VkDevice device,           // Logical device handle.
    VkCommandPool commandPool  // Command pool from which the command buffer will be allocated.
) {
    // Allocate information for the command buffer.
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;  // Specify the structure type.
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;                 // Set command buffer as primary.
    allocInfo.commandPool = commandPool;                               // Command pool to allocate from.
    allocInfo.commandBufferCount = 1;                                  // Allocate a single command buffer.

    VkCommandBuffer commandBuffer;
    // Allocate the command buffer and check for success.
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    // Configure the command buffer to begin recording.
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;  // Specify the structure type.
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;  // Indicate the buffer will be submitted only once.

    // Begin recording commands into the command buffer.
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    // Return the command buffer, now ready for recording commands.
    return commandBuffer;
}

/*
    Utility function to end and submit a single-use command buffer.
    This function finalizes recording of a command buffer, submits it to a queue for execution,
    waits for the queue to finish, and then cleans up the command buffer.
*/
inline void endSingleTimeCommands(
    VkDevice device,               // Logical device handle.
    VkQueue graphicsQueue,         // Graphics queue to which the command buffer will be submitted.
    VkCommandPool commandPool,     // Command pool from which the command buffer was allocated.
    VkCommandBuffer commandBuffer  // The command buffer to be finalized and executed.
) {
    // End recording commands in the command buffer.
    vkEndCommandBuffer(commandBuffer);

    // Set up submission information for the command buffer.
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;  // Specify the structure type.
    submitInfo.commandBufferCount = 1;                 // Number of command buffers to submit.
    submitInfo.pCommandBuffers = &commandBuffer;       // Pointer to the command buffer.

    // Submit the command buffer to the graphics queue.
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

    // Wait for the graphics queue to complete execution.
    vkQueueWaitIdle(graphicsQueue);

    // Free the command buffer, as it is no longer needed after submission.
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}
/*
    Utility function to copy data from one Vulkan buffer to another.
    This function uses a single-use command buffer to perform a buffer-to-buffer copy operation.
    It is typically used for transferring data, such as initializing vertex or uniform buffers.
*/
inline void copyBuffer(
    VkDevice device,            // Logical device handle.
    VkCommandPool commandPool,  // Command pool from which the command buffer will be allocated.
    VkQueue graphicsQueue,      // Graphics queue to which the copy command will be submitted.
    VkBuffer srcBuffer,         // Source buffer containing the data to be copied.
    VkBuffer dstBuffer,         // Destination buffer where the data will be copied to.
    VkDeviceSize size           // Size of the data to copy, in bytes.
) {
    // Begin recording a single-use command buffer.
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

    // Define the region of data to copy between the buffers.
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;  // Start copying from the beginning of the source buffer.
    copyRegion.dstOffset = 0;  // Start writing at the beginning of the destination buffer.
    copyRegion.size = size;    // The size of the data to copy.

    // Record the copy command into the command buffer.
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    // End recording and submit the command buffer, then clean it up.
    endSingleTimeCommands(device, graphicsQueue, commandPool, commandBuffer);
}


/*
    Utility function to check if a Vulkan format includes a stencil component.
    This function is used to determine whether a given format contains a stencil aspect, 
    which is often required for operations involving depth-stencil buffers.
*/
inline bool HasStencilComponent(VkFormat format) {
    // Check if the format is one of the formats that include a stencil component:
    // - VK_FORMAT_D32_SFLOAT_S8_UINT: 32-bit float for depth, 8-bit unsigned integer for stencil.
    // - VK_FORMAT_D24_UNORM_S8_UINT: 24-bit normalized integer for depth, 8-bit unsigned integer for stencil.
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}


/*
    Utility function to create a Vulkan image and allocate memory for it.
    This function abstracts the process of creating a 2D Vulkan image, allocating memory for it, 
    and binding the memory to the image. It supports configurations like mip levels, multisampling, 
    and tiling, making it flexible for various image types (e.g., textures, depth buffers).
*/
inline void createImage(
    VkDevice device,                   // Logical device handle.
    VkPhysicalDevice physicalDevice,   // Physical device handle to query memory properties.
    uint32_t width,                    // Width of the image in pixels.
    uint32_t height,                   // Height of the image in pixels.
    uint32_t mipLevels,                // Number of mip levels for the image.
    VkSampleCountFlagBits numSamples,  // Number of samples for multisampling.
    VkFormat format,                   // Format of the image (e.g., VK_FORMAT_R8G8B8A8_SRGB).
    VkImageTiling tiling,              // Tiling mode (e.g., optimal for GPU or linear for CPU).
    VkImageUsageFlags usage,           // Intended usage of the image (e.g., color attachment, sampled image).
    VkMemoryPropertyFlags properties,  // Required memory properties (e.g., device-local, host-visible).
    VkImage& image,                    // Output Vulkan image handle.
    VkDeviceMemory& imageMemory        // Output memory handle bound to the image.
) {
    // Configure the VkImageCreateInfo structure with image properties.
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO; // Specify the structure type.
    imageInfo.imageType = VK_IMAGE_TYPE_2D;               // Define the image as 2D.
    imageInfo.extent.width = width;                       // Set image width.
    imageInfo.extent.height = height;                     // Set image height.
    imageInfo.extent.depth = 1;                           // Set depth to 1 for 2D images.
    imageInfo.mipLevels = mipLevels;                      // Specify the number of mip levels.
    imageInfo.arrayLayers = 1;                            // Set the number of array layers (e.g., 1 for non-arrays).
    imageInfo.format = format;                            // Set the format of the image.
    imageInfo.tiling = tiling;                            // Set the tiling mode.
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;  // Initial image layout (undefined for creation).
    imageInfo.usage = usage;                              // Set intended usage flags.
    imageInfo.samples = numSamples;                       // Configure multisampling (e.g., 1 for no MSAA).
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;    // Exclusive queue family access.

    // Create the Vulkan image and check for success.
    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image!");
    }

    // Retrieve memory requirements for the created image.
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    // Allocate memory for the image based on the memory requirements.
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO; // Specify the structure type.
    allocInfo.allocationSize = memRequirements.size;          // Size of memory to allocate.
    allocInfo.memoryTypeIndex = findMemoryType(               // Find a suitable memory type index.
        physicalDevice, memRequirements.memoryTypeBits, properties);

    // Allocate memory for the image and check for success.
    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate image memory!");
    }

    // Bind the allocated memory to the Vulkan image.
    vkBindImageMemory(device, image, imageMemory, 0);
}
/*
    Utility function to transition the layout of a Vulkan image.
    This function performs a layout transition for an image using a pipeline barrier.
    It is commonly used for operations like preparing an image for use as a transfer destination or a shader input.
*/
inline void transitionImageLayout(
    VkDevice device,            // Logical device handle.
    VkCommandPool commandPool,  // Command pool to allocate the command buffer.
    VkQueue graphicsQueue,      // Graphics queue to execute the layout transition.
    VkImage image,              // Vulkan image to transition.
    VkFormat format,            // Format of the image (used for depth/stencil handling).
    VkImageLayout oldLayout,    // Current layout of the image.
    VkImageLayout newLayout,    // Target layout of the image.
    uint32_t mipLevels          // Number of mip levels in the image.
) {
    // Begin recording a single-use command buffer.
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

    // Configure the image memory barrier for the transition.
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;  // Specify the structure type.
    barrier.oldLayout = oldLayout;                           // Current layout of the image.
    barrier.newLayout = newLayout;                           // Target layout of the image.
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;   // Ignore queue family ownership.
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;   // Ignore queue family ownership.
    barrier.image = image;                                   // The Vulkan image being transitioned.

    // Define the subresource range for the transition.
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;  // Default to color aspect.
    barrier.subresourceRange.baseMipLevel = 0;                        // Start at the first mip level.
    barrier.subresourceRange.levelCount = mipLevels;                  // Number of mip levels to transition.
    barrier.subresourceRange.baseArrayLayer = 0;                      // Start at the first array layer.
    barrier.subresourceRange.layerCount = 1;                          // Transition only one layer.

    // Adjust the aspect mask if transitioning to a depth/stencil attachment layout.
    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;  // Set to depth aspect.
        if (HasStencilComponent(format)) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;  // Add stencil aspect if present.
        }
    }
    else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;  // Use color aspect for non-depth images.
    }

    VkPipelineStageFlags sourceStage;       // Source pipeline stage for the barrier.
    VkPipelineStageFlags destinationStage;  // Destination pipeline stage for the barrier.

    // Define access masks and pipeline stages based on the old and new layouts.
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;                             // No access from previous layout.
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;  // Enable write access for transfer.

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;    // Start of the pipeline.
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;  // Transition during the transfer stage.
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;  // Write access from the transfer stage.
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;     // Read access for shaders.

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;              // Transition from transfer stage.
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;  // Ready for fragment shader use.
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;                         // No access from previous layout.
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;  // Enable depth/stencil access.

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;                // Start of the pipeline.
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;  // Early fragment tests stage.
    }
    else {
        // Throw an error if the layout transition is unsupported.
        throw std::invalid_argument("Unsupported layout transition!");
    }

    // Record the pipeline barrier into the command buffer.
    vkCmdPipelineBarrier(
        commandBuffer,     // Command buffer to record the barrier.
        sourceStage,       // Source pipeline stage.
        destinationStage,  // Destination pipeline stage.
        0,                 // No special flags.
        0, nullptr,        // No memory barriers.
        0, nullptr,        // No buffer barriers.
        1, &barrier        // One image barrier.
    );

    // End recording and submit the command buffer, then clean it up.
    endSingleTimeCommands(device, graphicsQueue, commandPool, commandBuffer);
}
/*
    Utility function to copy data from a buffer to an image in Vulkan.
    This function uses a single-use command buffer to perform the copy operation, 
    typically used for uploading texture data to an image.
*/
inline void copyBufferToImage(
    VkDevice device,            // Logical device handle.
    VkCommandPool commandPool,  // Command pool to allocate the command buffer.
    VkQueue graphicsQueue,      // Graphics queue to execute the copy command.
    VkBuffer buffer,            // Source buffer containing the data to copy.
    VkImage image,              // Destination Vulkan image.
    uint32_t width,             // Width of the image in pixels.
    uint32_t height             // Height of the image in pixels.
) {
    // Begin recording a single-use command buffer.
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

    // Define the region of the buffer to copy to the image.
    VkBufferImageCopy region{};
    region.bufferOffset = 0;                                         // Start copying from the beginning of the buffer.
    region.bufferRowLength = 0;                                      // Tightly packed buffer (no padding between rows).
    region.bufferImageHeight = 0;                                    // Tightly packed buffer (no padding between rows).
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;  // Specify the color aspect of the image.
    region.imageSubresource.mipLevel = 0;                            // Copy to the base mip level of the image.
    region.imageSubresource.baseArrayLayer = 0;                      // Copy to the first array layer.
    region.imageSubresource.layerCount = 1;                          // Copy a single layer.
    region.imageOffset = { 0, 0, 0 };                                // Start at the top-left corner of the image.
    region.imageExtent = { width, height, 1 };                       // Define the region size (entire image in this case).

    // Record the buffer-to-image copy command.
    vkCmdCopyBufferToImage(
        commandBuffer,                         // Command buffer to record the command.
        buffer,                                // Source buffer.
        image,                                 // Destination image.
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,  // Image layout expected for the destination.
        1, &region                             // Number of regions and pointer to the region data.
    );

    // End recording, submit the command buffer, and clean it up.
    endSingleTimeCommands(device, graphicsQueue, commandPool, commandBuffer);
}
/*
    Utility function to create a Vulkan image view.
    An image view provides access to an image's subresources, such as mip levels or array layers,
    and is required to use an image in shaders or as a render target.
*/
inline VkImageView createImageView(
    VkDevice device,                 // Logical device handle.
    VkImage image,                   // Vulkan image for which the view is created.
    VkFormat format,                 // Format of the image (e.g., VK_FORMAT_R8G8B8A8_SRGB).
    VkImageAspectFlags aspectFlags,  // Aspect of the image to view (e.g., color, depth, stencil).
    uint32_t mipLevels               // Number of mip levels in the image.
) {
    // Configure the image view creation information.
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;  // Specify the structure type.
    viewInfo.image = image;                                     // Image to create a view for.
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;                  // Define the view type (2D image view).
    viewInfo.format = format;                                   // Specify the format of the image.

    // Define the subresource range for the image view.
    viewInfo.subresourceRange.aspectMask = aspectFlags;         // Specify the image aspect (e.g., color, depth, stencil).
    viewInfo.subresourceRange.baseMipLevel = 0;                 // Start at the first mip level.
    viewInfo.subresourceRange.levelCount = mipLevels;           // Include all specified mip levels.
    viewInfo.subresourceRange.baseArrayLayer = 0;               // Start at the first array layer.
    viewInfo.subresourceRange.layerCount = 1;                   // Include only one array layer.

    // Create the image view.
    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        // Throw an error if image view creation fails.
        throw std::runtime_error("Failed to create texture image view!");
    }

    // Return the created image view handle.
    return imageView;
}

inline void PrintMatrix(const glm::mat4& matrix, const std::string& name) {
    std::cout << name << ":\n";
    for (int i = 0; i < 4; ++i) { // Loop through rows
        std::cout << matrix[i][0] << " " << matrix[i][1] << " " << matrix[i][2] << " " << matrix[i][3] << "\n";
    }
    std::cout << std::endl;
}

inline void SetObjectName(VkDevice device, uint64_t object, VkObjectType objectType, const char* name) {
    VkDebugUtilsObjectNameInfoEXT nameInfo = {};
    nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    nameInfo.objectType = objectType;
    nameInfo.objectHandle = object;
    nameInfo.pObjectName = name;

    auto func = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetDeviceProcAddr(device, "vkSetDebugUtilsObjectNameEXT");
    if (func) {
        func(device, &nameInfo);
    }
}
inline void BeginDebugMarker(VkDevice device, VkCommandBuffer commandBuffer, const char* name, const float color[4]) {
    VkDebugUtilsLabelEXT labelInfo = {};
    labelInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
    labelInfo.pLabelName = name;
    memcpy(labelInfo.color, color, sizeof(float) * 4);

    auto func = (PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetDeviceProcAddr(device, "vkCmdBeginDebugUtilsLabelEXT");
    if (func) {
        func(commandBuffer, &labelInfo);
    }
}
inline void EndDebugMarker(VkDevice device, VkCommandBuffer commandBuffer) {
    auto func = (PFN_vkCmdEndDebugUtilsLabelEXT)vkGetDeviceProcAddr(device, "vkCmdEndDebugUtilsLabelEXT");
    if (func) {
        func(commandBuffer);
    }
}
