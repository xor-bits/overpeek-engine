#include "engine/graphics/interface/instance.hpp"
#include "engine/enum.hpp"



namespace oe::graphics {

	class GLInstance : public Instance {
	public:
		GLInstance(const InstanceInfo& instance_info);
		~GLInstance();
		
		// Inherited via Instance
		virtual graphics::Window* createWindow(const WindowInfo& window_config) const override;
		virtual void destroyWindow(graphics::Window* window) const override;
	
	public:
		virtual void blending(oe::modes mode = oe::modes::enable) const override;
		virtual void depth(oe::depth_functions func = oe::depth_functions::always) const override;
		virtual void swapInterval(unsigned int interval = 0) const override;
		virtual void culling(oe::culling_modes c = oe::culling_modes::neither) const override;
		virtual void lineWidth(float w = 1.0f) const override;
		virtual void pointRadius(float w = 1.0f) const override;
		virtual void polygonMode(oe::polygon_mode p = oe::polygon_mode::fill) const override;
	};

}