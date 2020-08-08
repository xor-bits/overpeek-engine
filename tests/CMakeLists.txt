add_subdirectory(rendering) # executable
add_subdirectory(text) # executable
add_subdirectory(guis) # executable
add_subdirectory(entities) # executable
add_subdirectory(networking) # executable

if(OE_BUILD_MODE EQUAL 2)
    target_compile_definitions(engine PRIVATE BUILD_VULKAN)
    add_subdirectory(vulkan) # executable
endif()