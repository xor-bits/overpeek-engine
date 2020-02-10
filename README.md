# overpeek-engine
Minimal game engine/framework
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
cmake -DBUILD_TESTS=TRUE ..
```

#### Depends:
- Vulkan
- OpenGL
- OpenAL

### Features
- Window creation (glfw) 
- Audio (OpenAL) 
- Rendering (OpenGL)
    - Text rendering (Freetype) (basic) 
    - GUI (basic)
- Networking (basic)
- General utility tools (that I find useful) 


### TODO
- [ ] ECS controlled GUI
- [ ] Vulkan
- [x] Texture packer - rework textures and renderer
- [ ] Freetype -> stb_truetype
- [ ] Optimized text rendering
- [ ] ImGUI 
- [ ] Easy to use mode
