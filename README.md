# overpeek-engine
Minimal game engine/framework.
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

#### Depends:
- Vulkan
- OpenGL
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


### TODO
- [ ] Lua scripting
- [ ] TextInput string member that holds allowed characters
- [ ] ECS controlled GUI
- [ ] Vulkan
- [x] Texture packer - rework textures and renderer
- [ ] Freetype -> stb_truetype
- [ ] Optimized text rendering (draw text to framebuffer)
- [ ] ImGUI 
- [ ] Easy to use mode
