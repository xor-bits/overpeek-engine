set(CMAKE_CXX_STANDARD 17)

add_executable(rendering "rendering/main.cpp")
set_target_properties(rendering PROPERTIES FOLDER "engine/tests")
target_link_libraries(rendering PRIVATE engine)
target_include_directories(rendering PRIVATE ${engine_INCLUDE_DIRS})

add_executable(text "text/main.cpp")
set_target_properties(text PROPERTIES FOLDER "engine/tests")
target_link_libraries(text PRIVATE engine)
target_include_directories(text PRIVATE ${engine_INCLUDE_DIRS})

add_executable(guis "guis/main.cpp")
set_target_properties(guis PROPERTIES FOLDER "engine/tests")
target_link_libraries(guis PRIVATE engine)
target_include_directories(guis PRIVATE ${engine_INCLUDE_DIRS})

add_executable(entities "entities/main.cpp")
set_target_properties(entities PROPERTIES FOLDER "engine/tests")
target_link_libraries(entities PRIVATE engine)
target_include_directories(entities PRIVATE ${engine_INCLUDE_DIRS})

add_executable(networking "networking/main.cpp")
set_target_properties(networking PROPERTIES FOLDER "engine/tests")
target_link_libraries(networking PRIVATE engine)
target_include_directories(networking PRIVATE ${engine_INCLUDE_DIRS})

if(OE_BUILD_MODE EQUAL 2)
    target_compile_definitions(engine PRIVATE BUILD_VULKAN)

    add_executable(vulkan "vulkan/main.cpp" "vulkan/texture.png.hpp")
    set_target_properties(vulkan PROPERTIES FOLDER "engine/tests")
    target_link_libraries(vulkan PRIVATE engine)
    target_include_directories(vulkan PRIVATE ${engine_INCLUDE_DIRS})
endif()