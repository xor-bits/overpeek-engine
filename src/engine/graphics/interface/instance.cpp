#include "instance.hpp"

#include "renderer.hpp"



namespace oe::graphics {
    
    graphics::Renderer* Instance::createRenderer(const RendererInfo& renderer_info) const
    {
        return new Renderer(renderer_info);
    }

    void Instance::destroyRenderer(graphics::Renderer* renderer) const 
    {
        delete renderer;
    }

}