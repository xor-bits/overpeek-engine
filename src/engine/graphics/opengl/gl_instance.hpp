#include "engine/graphics/interface/instance.hpp"
#include "engine/enum.hpp"



namespace oe::graphics
{
	class GLInstance : public Instance
	{
	public:
		GLInstance();
		~GLInstance() override;
	
	public:
		void blending(oe::modes mode = oe::modes::enable) const override;
		void depth(oe::depth_functions func = oe::depth_functions::always) const override;
		void swapInterval(unsigned int interval = 0) const override;
		void culling(oe::culling_modes c = oe::culling_modes::neither) const override;
		void lineWidth(float w = 1.0f) const override;
		void pointRadius(float w = 1.0f) const override;
		void polygonMode(oe::polygon_mode p = oe::polygon_mode::fill) const override;
		void viewport(int x, int y, size_t w, size_t h) const override;
		int versionNumber() override;
	};
}