# Paths to external libraries and include directories
GLFW_INCLUDE = C:/VulkanFolder/Externals/GLFW/include
GLM_INCLUDE = C:/VulkanFolder/Externals/GLM
VULKAN_INCLUDE = C:/VulkanSDK/1.3.296.0/Include
STB_INCLUDE = C:/VulkanFolder/Externals/stb-master
TINYOBJ_INCLUDE = C:/VulkanFolder/Externals/tiny_obj-master
IMGUI_INCLUDE = C:/VulkanFolder/repos/VulkanCore/VulkanApp/imgui-master

GLFW_LIB = C:/VulkanFolder/Externals/GLFW/lib-mingw-w64
VULKAN_LIB = C:/VulkanSDK/1.3.296.0/Lib

# Compiler and flags
CXX = g++
CFLAGS = -std=c++20 -O2 

# Include and library flags
INCLUDES = -I$(GLFW_INCLUDE) -I$(GLM_INCLUDE) -I$(VULKAN_INCLUDE) -I$(STB_INCLUDE) -I$(TINYOBJ_INCLUDE) -I$(IMGUI_INCLUDE)
LIBS = -L$(GLFW_LIB) -L$(VULKAN_LIB) -lglfw3 -lvulkan-1 -lgdi32 -luser32 -lkernel32 -lm

# Target and source files
TARGET = VulkanTest
SRCS = main.cpp \
       VulkanRenderer.cpp \
	   Camera.cpp \
       Model.cpp \
       C:/VulkanFolder/repos/VulkanCore/VulkanApp/imgui-master/imgui.cpp \
       C:/VulkanFolder/repos/VulkanCore/VulkanApp/imgui-master/imgui_draw.cpp \
       C:/VulkanFolder/repos/VulkanCore/VulkanApp/imgui-master/imgui_tables.cpp \
       C:/VulkanFolder/repos/VulkanCore/VulkanApp/imgui-master/imgui_widgets.cpp \
       C:/VulkanFolder/repos/VulkanCore/VulkanApp/imgui-master/backends/imgui_impl_vulkan.cpp \
       C:/VulkanFolder/repos/VulkanCore/VulkanApp/imgui-master/backends/imgui_impl_glfw.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Build rules
$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Additional rules
.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
