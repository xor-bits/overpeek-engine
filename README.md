![Logo](/.github/oe_logo.png)
# overpeek-engine
Minimal game engine/library.
Primary focus on 2D side.
Should be easy to use.
 - Windows
 - Linux
 - OSX, possibly (not tested)

## Cloning
```
git clone --recursive https://github.com/Overpeek/overpeek-engine.git
```

#### CMake
```
mkdir build
cd build
cmake ..
```
```
mkdir build
cd build
cmake -DBUILD_TESTS=TRUE ..
```
```
mkdir build
cd build
cmake -DBUILD_TESTS=TRUE -DBUILD_VULKAN=TRUE ..
```

#### Depends on:
- OpenGL
- Vulkan (if BUILD_VULKAN=TRUE)
- OpenAL
- vcpkg

#### vcpkg packages
```vcpkg install openal-soft glfw3 glad box2d libzip enet freetype ms-gsl entt spdlog fmt stb minimp3 glm gcem```
optionally shaderc and/or vulkan' ```vcpkg install shaderc vulkan```
#### additional glfw3 dependencies for ubuntu:
```sudo apt install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev libxrandr-dev```

## Features
- Window creation (glfw) 
- Audio (OpenAL) 
- Rendering (OpenGL)
    - Sprite rendering
    - Text rendering (Freetype) (basic) 
    - GUI (basic)
- Networking (basic)
- General utility tools (that I find useful)

## Demo images
#### tests/guis
- Simple gui demo
- List Widget is under developement so the quad is not animated

![guis](/.github/tests/guis.png)

#### tests/entities
- Box2D physics and entt entities
- Translucent bordered window

![entities](/.github/tests/entities.png)

## Why?
I created this as I am making my game entirely from scratch and I thought someone could find this project somewhat useful. Or so I hope.
