#include "widget.h"



namespace oe::gui {

	class TextView : public Widget {
	public:
		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
	};

}