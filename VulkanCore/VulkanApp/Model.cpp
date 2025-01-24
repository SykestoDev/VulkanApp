#include "Model.h"

// Model Loader
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

// Image Loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Model::Model(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool)
    : device(device), physicalDevice(physicalDevice), graphicsQueue(graphicsQueue), commandPool(commandPool) {}
Model::~Model() {
    // Destroy texture resources
    if (textureImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(device, textureImageView, nullptr);
        textureImageView = VK_NULL_HANDLE;
    }

    if (textureSampler != VK_NULL_HANDLE) {
        vkDestroySampler(device, textureSampler, nullptr);
        textureSampler = VK_NULL_HANDLE;
    }
    if (textureImage != VK_NULL_HANDLE) {
        vkDestroyImage(device, textureImage, nullptr);
        textureImage = VK_NULL_HANDLE;
    }
    if (textureImageMemory != VK_NULL_HANDLE) {
        vkFreeMemory(device, textureImageMemory, nullptr);
        textureImageMemory = VK_NULL_HANDLE;
    }

    // Destroy vertex and index buffers
    if (vertexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vertexBuffer = VK_NULL_HANDLE;
    }
    if (vertexBufferMemory != VK_NULL_HANDLE) {
        vkFreeMemory(device, vertexBufferMemory, nullptr);
        vertexBufferMemory = VK_NULL_HANDLE;
    }
    if (indexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(device, indexBuffer, nullptr);
        indexBuffer = VK_NULL_HANDLE;
    }
    if (indexBufferMemory != VK_NULL_HANDLE) {
        vkFreeMemory(device, indexBufferMemory, nullptr);
        indexBufferMemory = VK_NULL_HANDLE;
    }
}   

void Model::Bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}
void Model::Draw(VkCommandBuffer commandBuffer) {
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Model::LoadOBJ(const std::string& filepath) {
    // Initialize TinyOBJ structures to load OBJ file data
    tinyobj::attrib_t attrib; // Holds vertex attributes such as positions, normals, and texture coordinates
    std::vector<tinyobj::shape_t> shapes; // Holds the geometric shapes in the OBJ file
    std::vector<tinyobj::material_t> materials; // Holds material information (not used in this example)
    std::string warn, err; // Strings to capture warnings and errors during file loading

    // Load the OBJ file and populate the TinyOBJ structures
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        // If loading fails, throw an exception with the combined warning and error messages
        throw std::runtime_error(warn + err);
    }

    // Iterate through each shape in the OBJ file
    for (const auto& shape : shapes) {
        // Iterate through each index in the shape's mesh
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{}; // Initialize a new vertex

            // Set the vertex position using the indexed position data from the attrib array
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0], // X coordinate
                attrib.vertices[3 * index.vertex_index + 1], // Y coordinate
                attrib.vertices[3 * index.vertex_index + 2]  // Z coordinate
            };

            // Set the vertex texture coordinates using the indexed texcoord data from the attrib array
            // Flip the Y coordinate by subtracting it from 1.0f (to match Vulkan's coordinate system)
            vertex.textCoor = {
                attrib.texcoords[2 * index.texcoord_index + 0], // U coordinate
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1] // V coordinate (flipped)
            };

            // Set the vertex color (default to white as the OBJ file doesn't specify colors)
            vertex.color = { 1.0f, 1.0f, 1.0f };

            // Add the vertex to the vertices vector
            vertices.push_back(vertex);

            // Add an index to the indices vector for indexed drawing
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }

    // Store the total number of vertices and indices
    vertexCount = static_cast<uint32_t>(vertices.size());
    indexCount = static_cast<uint32_t>(indices.size());

    // Create GPU buffers for the vertices and indices
    CreateVertexBuffer(); // Create a Vulkan vertex buffer and upload vertex data to the GPU
    CreateIndexBuffer();  // Create a Vulkan index buffer and upload index data to the GPU

}
void Model::LoadFBX(const std::string& filepath) {
    // Example using Assimp for FBX loading
    // Include Assimp headers and link its library in your build system
    // Assimp::Importer importer;
    // const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    // if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    //     throw std::runtime_error("Failed to load FBX file: " + filepath);
    // }

    // Parse the FBX file and fill `vertices` and `indices` with data
    // You can recursively process the scene graph to extract meshes and transform data
    std::cerr << "FBX loading not yet implemented: " << filepath << "\n";

    // For now, throw an error if unimplemented
    throw std::runtime_error("FBX loading not implemented yet.");
}
void Model::LoadFromFile(const std::string& filepath) {
    if (filepath.ends_with(".obj")) {
        LoadOBJ(filepath);
    }
    else if (filepath.ends_with(".fbx")) {
        LoadFBX(filepath);
    }
    else {
        throw std::runtime_error("Unsupported file format: " + filepath);
    }
}
void Model::LoadTexture(const std::string& texturePath) {
    CreateTextureImage(texturePath);
    CreateTextureImageView();
    CreateTextureSampler();

    // Log texture creation details
    /*std::cout << "Model Texture Created: TexturePath[" << texturePath
        << "], TextureImage: " << textureImage
        << ", TextureImageView: " << textureImageView
        << ", TextureSampler: " << textureSampler
        << std::endl;*/
}

VkImageView Model::GetTextureImageView() {
    return textureImageView;
}
VkSampler Model::GetTextureSampler() {
    return textureSampler;
}

void Model::CreateVertexBuffer() {
    if (vertices.empty()) {
        throw std::runtime_error("Vertex buffer is empty. Cannot create buffer.");
    }

    // Calculate the size of the vertex buffer in bytes
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size(); // Size = size of one vertex * number of vertices

    // Declare a staging buffer and its associated memory
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // Create a staging buffer to temporarily hold vertex data
    // The buffer is created with VK_BUFFER_USAGE_TRANSFER_SRC_BIT, allowing it to act as a source for data transfer
    createBuffer(
        device, physicalDevice, bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory
    );

    // Map the staging buffer's memory and copy vertex data into it
    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data); // Map buffer memory into CPU-accessible memory
    memcpy(data, vertices.data(), (size_t)bufferSize);  // Copy vertex data into the staging buffer
    vkUnmapMemory(device, stagingBufferMemory);         // Unmap the buffer memory

    // Create the vertex buffer on the GPU
    // The buffer is created with VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, allowing it to be used for vertex input
    createBuffer(
        device, physicalDevice, bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer, vertexBufferMemory
    );

    SetObjectName(device, (uint64_t)vertexBuffer, VK_OBJECT_TYPE_BUFFER, "MC : Vertex Buffer");

    // Copy the data from the staging buffer to the GPU vertex buffer
    // The staging buffer acts as the source, and the vertex buffer is the destination
    copyBuffer(
        device, commandPool, graphicsQueue,
        stagingBuffer, vertexBuffer, bufferSize
    );

    // Clean up the staging buffer and its associated memory
    vkDestroyBuffer(device, stagingBuffer, nullptr);    // Destroy the staging buffer
    vkFreeMemory(device, stagingBufferMemory, nullptr); // Free the memory allocated for the staging buffer
}
void Model::CreateIndexBuffer() {
    if (indices.empty()) {
        throw std::runtime_error("Index buffer is empty. Cannot create buffer.");
    }

    // Calculate the size of the index buffer in bytes
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size(); // Size = size of one index * number of indices

    // Declare a staging buffer and its associated memory
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // Create a staging buffer to temporarily hold index data
    // The buffer is created with VK_BUFFER_USAGE_TRANSFER_SRC_BIT, allowing it to act as a source for data transfer
    createBuffer(
        device, physicalDevice, bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory
    );

    // Map the staging buffer's memory and copy index data into it
    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data); // Map buffer memory into CPU-accessible memory
    memcpy(data, indices.data(), (size_t)bufferSize); // Copy index data into the staging buffer
    vkUnmapMemory(device, stagingBufferMemory); // Unmap the buffer memory

    // Create the index buffer on the GPU
    // The buffer is created with VK_BUFFER_USAGE_INDEX_BUFFER_BIT, allowing it to be used for indexed drawing
    createBuffer(
        device, physicalDevice, bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        indexBuffer, indexBufferMemory
    );

    SetObjectName(device, (uint64_t)indexBuffer, VK_OBJECT_TYPE_BUFFER, "MC : Index Buffer");

    // Copy the data from the staging buffer to the GPU index buffer
    // The staging buffer acts as the source, and the index buffer is the destination
    copyBuffer(
        device, commandPool, graphicsQueue,
        stagingBuffer, indexBuffer, bufferSize
    );

    // Clean up the staging buffer and its associated memory
    vkDestroyBuffer(device, stagingBuffer, nullptr); // Destroy the staging buffer
    vkFreeMemory(device, stagingBufferMemory, nullptr); // Free the memory allocated for the staging buffer

}

/**
 * @brief Generates mipmaps for a Vulkan image.
 *
 * Mipmaps are pre-calculated, optimized textures that improve rendering performance and quality
 * when sampling a texture at varying levels of detail. This function generates mipmaps by successively
 * downscaling the texture using linear blitting.
 *
 * @param image Vulkan image for which mipmaps are generated.
 * @param imageFormat Format of the image.
 * @param texWidth Width of the base level texture.
 * @param texHeight Height of the base level texture.
 * @param mipLevels Number of mip levels to generate.
 *
 * @throws std::runtime_error if the format does not support linear blitting.
 */
void Model::GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
{
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat, &formatProperties);

    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        // If linear blitting is not supported, throw an error
        throw std::runtime_error("texture image format does not support linear blitting!");
    }

    // Begin a single-time command buffer for generating mipmaps
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

    // Configure the barrier for transitioning image layouts
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER; // Specify the structure type
    barrier.image = image; // Specify the image to transition
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // Ignore queue family transitions
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // Ignore queue family transitions
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Specify the color aspect
    barrier.subresourceRange.baseArrayLayer = 0; // Start with the first array layer
    barrier.subresourceRange.layerCount = 1;     // Number of array layers
    barrier.subresourceRange.levelCount = 1;     // Process one mip level at a time

    // Initialize mipmap dimensions
    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    // Loop through each mip level and generate mipmaps
    for (uint32_t i = 1; i < mipLevels; i++) {
        // Transition the previous mip level to transfer source optimal layout
        barrier.subresourceRange.baseMipLevel = i - 1;            // Specify the current mip level
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL; // Current layout
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL; // New layout
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;     // Synchronize writes to the image
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;      // Prepare for read operations

        vkCmdPipelineBarrier(
            commandBuffer, // Command buffer to record into
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, // Synchronize within transfer stage
            0, // No dependency flags
            0, nullptr, // No memory barriers
            0, nullptr, // No buffer barriers
            1, &barrier // Single image barrier
        );

        // Configure the blit operation for downscaling
        VkImageBlit blit{};
        blit.srcOffsets[0] = { 0, 0, 0 }; // Source region start
        blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };            // Source region end
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Source aspect
        blit.srcSubresource.mipLevel = i - 1;   // Source mip level
        blit.srcSubresource.baseArrayLayer = 0; // First layer
        blit.srcSubresource.layerCount = 1;     // Single layer
        blit.dstOffsets[0] = { 0, 0, 0 };       // Destination region start
        blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 }; // Destination region end
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Destination aspect
        blit.dstSubresource.mipLevel = i;       // Destination mip level
        blit.dstSubresource.baseArrayLayer = 0; // First layer
        blit.dstSubresource.layerCount = 1;     // Single layer

        vkCmdBlitImage(
            commandBuffer, // Command buffer to record into
            image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, // Source image and layout
            image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, // Destination image and layout
            1, &blit,        // Single blit region
            VK_FILTER_LINEAR // Use linear filtering for downscaling
        );

        // Transition the previous mip level to shader read-only layout
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;     // Current layout
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // New layout
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;          // Synchronize reads from the image
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;            // Prepare for shader read operations

        vkCmdPipelineBarrier(
            commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, // Synchronize transfer and fragment shader stages
            0, // No dependency flags
            0, nullptr, // No memory barriers
            0, nullptr, // No buffer barriers
            1, &barrier // Single image barrier
        );

        // Update dimensions for the next mip level
        mipWidth = mipWidth > 1 ? mipWidth / 2 : 1;
        mipHeight = mipHeight > 1 ? mipHeight / 2 : 1;
    }

    // Transition the last mip level to shader read-only layout
    barrier.subresourceRange.baseMipLevel = mipLevels - 1;        // Last mip level
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;     // Current layout
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // New layout
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;         // Synchronize writes
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;            // Prepare for shader read operations

    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, // Synchronize transfer and fragment shader stages
        0, // No dependency flags
        0, nullptr, // No memory barriers
        0, nullptr, // No buffer barriers
        1, &barrier // Single image barrier
    );

    // End and submit the command buffer
    endSingleTimeCommands(device, graphicsQueue, commandPool, commandBuffer);
}
/**
 * @brief Creates an image view for the texture image.
 *
 * An image view is required to use a Vulkan image in shaders. It defines how the image should
 * be accessed and interpreted in the rendering pipeline, such as its format, aspect, and mip levels.
 *
 * @throws std::runtime_error if the image view creation fails.
 */
void Model::CreateTextureImageView() {
    // Use a helper function to create an image view for the texture.
    // Parameters:
    // - device: Vulkan logical device.
    // - textureImage: Vulkan image for which the view is created.
    // - VK_FORMAT_R8G8B8A8_SRGB: Format of the image (sRGB color space).
    // - VK_IMAGE_ASPECT_COLOR_BIT: Specify that this is a color image view.
    // - mipLevels: Number of mip levels in the image.
    textureImageView = createImageView(device, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
}
/**
 * @brief Creates a Vulkan texture sampler for the model's texture.
 *
 * A texture sampler is used to control how a texture is sampled, including filtering, wrapping,
 * and mipmap behavior. This function creates a sampler with linear filtering, repeat wrapping,
 * and anisotropic filtering enabled.
 *
 * @throws std::runtime_error if the texture sampler creation fails.
 */
void Model::CreateTextureSampler() {
    // Configure sampler settings.
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO; // Specify the structure type.
    samplerInfo.magFilter = VK_FILTER_LINEAR; // Magnification filter (linear interpolation).
    samplerInfo.minFilter = VK_FILTER_LINEAR; // Minification filter (linear interpolation).
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; // Wrap mode for U coordinate (repeat texture).
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT; // Wrap mode for V coordinate (repeat texture).
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT; // Wrap mode for W coordinate (repeat texture).

    // Enable anisotropic filtering.
    samplerInfo.anisotropyEnable = VK_TRUE; // Enable anisotropic filtering.

    // Query the physical device for the maximum supported anisotropy level.
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy; // Set max anisotropy.

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; // Border color for clamp-to-border mode (not used here).
    samplerInfo.unnormalizedCoordinates = VK_FALSE;             // Use normalized texture coordinates.
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;     // Use linear filtering for mipmaps.

    // Create the Vulkan texture sampler.
    if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create texture sampler!"); // Handle creation failure.
    }
}
/**
 * @brief Creates a Vulkan texture image from a file.
 *
 * This function loads an image file, creates a Vulkan image, and uploads the image data
 * to the GPU. It also generates mipmaps for efficient texture sampling at different levels of detail.
 *
 * @param texturePath Path to the texture image file.
 *
 * @throws std::runtime_error if the image file fails to load or Vulkan operations fail.
 */
void Model::CreateTextureImage(const std::string& texturePath) {
    // Load the texture image using stb_image
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4; // Assuming 4 bytes per pixel (RGBA format).
    if (!pixels) {
        throw std::runtime_error("Failed to load texture image: " + texturePath);
    }

    // Calculate the number of mip levels based on the texture dimensions.
    mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

    // Create a staging buffer for the image data.
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(
        device, physicalDevice, imageSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, // Buffer will be used as a source for transfers.
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // Host-visible memory for easy data upload.
        stagingBuffer, stagingBufferMemory
    );

    // Map the staging buffer memory and copy the pixel data into it.
    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);

    // Free the CPU-side image data after uploading it to the staging buffer.
    stbi_image_free(pixels);

    // Create the Vulkan image.
    createImage(
        device, physicalDevice, texWidth, texHeight, mipLevels,
        VK_SAMPLE_COUNT_1_BIT,   // No multisampling for textures.
        VK_FORMAT_R8G8B8A8_SRGB, // Texture format with sRGB color space.
        VK_IMAGE_TILING_OPTIMAL, // Optimal tiling for GPU access.
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, // Usage flags for transfer and sampling.
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, // Device-local memory for optimal performance.
        textureImage, textureImageMemory
    );

    // Transition the image layout to be ready for data transfer.
    transitionImageLayout(
        device, commandPool, graphicsQueue,
        textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED,            // Initial undefined layout.
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, // Prepare for data transfer.
        mipLevels
    );

    // Step 4: Copy the staging buffer data to the Vulkan image.
    copyBufferToImage(
        device, commandPool, graphicsQueue,
        stagingBuffer, textureImage,
        static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight)
    );

    // Generate mipmaps for the texture.
    GenerateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);

    // Clean up the staging buffer and its memory.
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}


glm::vec3 Model::GetPosition()
{
    return position;
}
glm::mat4 Model::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position); // Assuming `Position` is a glm::vec3
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f,-1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale); // Assuming `Scale` is a glm::vec3
    return model;
}

void Model::SetPosition(const glm::vec3& position)
{
    this->position = position;
    UpdateModelMatrix();
}
void Model::SetRotation(const glm::vec3& rotation)
{
    this->rotation = rotation;
    UpdateModelMatrix();
}
void Model::SetScale(const glm::vec3& scale)
{
    this->scale = scale;
    UpdateModelMatrix();
}
void Model::UpdateModelMatrix() {
    modelMatrix = glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::scale(glm::mat4(1.0f), scale);;
}

