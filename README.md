![Logo](/.github/oe_logo.png)

[![Build Status](https://travis-ci.com/Overpeek/overpeek-engine.svg?branch=master)](https://travis-ci.com/Overpeek/overpeek-engine)

# overpeek-engine
Minimal game engine/library.
Primary focus on 2D side.
Should be easy to use.
 - Windows
 - Linux
 - OSX, (most likely not working, as I do not own any Apple devices)


## Building
#### Build with Conan
```
mkdir build && cd build
conan install .. -s build_type=Debug -s compiler.runtime=MDd --build missing
cmake ..
```
possible cmake arguments:
-DBUILD_TESTS=(ON/OFF)
-DOE_BUILD_MODE=(0/1/2)

buildmode 0 being OpenGL only,
buildmode 1 being OpenGL with shaderc and
buildmode 2 being Vulkan/OpenGL with shaderc
node: Vulkan build is experimental and still lacking basic features

#### Depends on:
- OpenGL
- Vulkan (if BUILD_VULKAN=TRUE)
- OpenAL
- CMake
- Conan
- (multiple conan packages)

#### additional glfw3 dependencies for ubuntu:
```sudo apt install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev libxrandr-dev libx11-xcb-dev libxcb-render-util0-dev```


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
I created this as I am making my game and it's own 'engine' and I thought someone could find this project somewhat useful. Or so I hope.
