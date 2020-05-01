add_subdirectory(rendering) # executable
add_subdirectory(text) # executable
add_subdirectory(gui) # executable
add_subdirectory(entities) # executable
add_subdirectory(networking) # executable

if(${BUILD_VULKAN})
    target_compile_definitions(engine PRIVATE BUILD_VULKAN)
    add_subdirectory(vulkan) # executable
endif()