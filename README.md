# overpeek-engine
Engine project

### Cloning
```
git clone --recursive https://github.com/Overpeek/overpeek-engine.git
```


### CMake
```
cmake -B build/
```
#### Depends:
- Freetype
- GLFW3
- OpenGL
- OpenAL

### Features
- Window creation (glfw) 
- Audio (OpenAL) 
- Rendering (OpenGL)
    - Text rendering (Freetype) (basic) 
    - GUI (basic)
- General utility tools (that I find useful) 


### TODO
- [ ] Texture packer - rework textures and renderer
- [ ] Freetype -> stb_truetype
- [ ] Optimized text rendering
- [ ] GUI (and ImGUI for debugging) 
