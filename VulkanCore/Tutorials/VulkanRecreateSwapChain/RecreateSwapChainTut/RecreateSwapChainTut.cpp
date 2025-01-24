#include "VulkanRenderer.h"



int main()
{
	try {
		VulkanRenderer().Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << "\n"; ///< Output exception message
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

