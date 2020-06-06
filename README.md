![Logo](/github/oe_logo.png)
# overpeek-engine
Minimal game engine/library.
Primary focus on 2D side.
Should be easy to use.
 - Windows
 - Linux
 - OSX, possibly (not tested)

### Cloning
```
git clone --recursive https://github.com/Overpeek/overpeek-engine.git
```

### CMake
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
- OpenAL
- Vulkan (if BUILD_VULKAN=TRUE)

### Features
- Window creation (glfw) 
- Audio (OpenAL) 
- Rendering (OpenGL)
    - Sprite rendering
    - Text rendering (Freetype) (basic) 
    - GUI (basic)
- Networking (basic)
- General utility tools (that I find useful) 
