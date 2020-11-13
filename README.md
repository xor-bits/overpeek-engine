![Logo](/.github/oe_logo.png)

[![Build Status](https://travis-ci.com/Overpeek/overpeek-engine.svg?branch=master)](https://travis-ci.com/Overpeek/overpeek-engine)

# overpeek-engine
Minimal game engine/library.
Primary focus on 2D side.
Should be easy to use.
 - Windows
 - Linux
 - OSX


## Building
#### Build

Build with cmake and conan:
```
mkdir build && cd build
conan install .. --build=missing
cmake ..
cmake --build .
```
possible cmake arguments:
-DOE_BUILD_TESTS=(ON/OFF)
-DOE_BUILD_MODE=(0/1/2)


Create a conan package:
```
conan create . --build=missing
```




buildmode 0 being OpenGL only,
buildmode 1 being OpenGL with shaderc and
buildmode 2 being Vulkan/OpenGL with shaderc
node: Vulkan build is experimental and still lacking basic features

#### Depends on:
- A c++17 compiler (clang++, g++, msvc)
- CMake
- Conan and some packages listed in conanfile.py


## Features
- Window creation (glfw) 
- Audio (Not yet)
- Rendering (OpenGL)
    - Sprite rendering
    - Text rendering (stb_truetype) 
    - GUI (basic)
- Networking (basic)
- General utility tools (that I find useful)


## Demo images
#### tests/guis
- Simple gui demo
- List Widget is under developement so the cube is not animated

![guis](/.github/tests/guis.png)

#### tests/entities
- Box2D physics and entt entities
- Translucent bordered window

![entities](/.github/tests/entities.png)


## Why?
I created this as I am making my game and it's own 'engine' and I thought someone could find this project somewhat useful. Or so I hope.
