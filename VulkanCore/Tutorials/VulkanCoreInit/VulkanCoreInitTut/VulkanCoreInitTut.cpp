#include "VulkanRenderer.h"


/**
 * @brief Entry point for the Vulkan application.
 *
 * Initializes and runs the VulkanRenderer instance. Handles any runtime exceptions
 * and outputs error messages to the standard error stream.
 */
int main() {
	try {
		VulkanRenderer().Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << "\n"; ///< Output exception message
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}