#include "widget.h"



namespace oe::gui {

	void Widget::renderNodes(oe::graphics::Renderer& renderer) {
		for (Widget* w : m_nodes) {
			w->render(renderer);
		}
	}

}