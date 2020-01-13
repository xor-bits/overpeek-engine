#include <vector>
#include <engine/graphics/renderer.h>



namespace oe::gui {

	class Widget {
	private:
		std::vector<Widget*> m_nodes;

	public:
		Widget(int x, int y, int width, int height);

		virtual void render(oe::graphics::Renderer& renderer) = 0;
		void renderNodes(oe::graphics::Renderer& renderer);
	};

}