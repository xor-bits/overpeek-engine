![Logo](/.github/oe_logo.png)
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
- Vulkan (if BUILD_VULKAN=TRUE)
- OpenAL

### Features
- Window creation (glfw) 
- Audio (OpenAL) 
- Rendering (OpenGL)
    - Sprite rendering
    - Text rendering (Freetype) (basic) 
    - GUI (basic)
- Networking (basic)
- General utility tools (that I find useful)

### Why?
I created this as I am making my game entirely from scratch and I thought someone could find this project somewhat useful. Or so I hope.

### Demo images
## tests/guis
![Logo](/.github/tests/guis.png)
## tests/entities
![Logo](/.github/tests/entities.png)
