#include <engine/include.hpp>

#include <mapbox/earcut.hpp>

#include <memory_resource>



namespace mapbox::util
{
	template<>
	struct nth<0, oe::graphics::VertexData>
	{
		inline static auto get(const oe::graphics::VertexData &t) {
			return t.data.position.x;
		};
	};
	
	template<>
	struct nth<1, oe::graphics::VertexData>
	{
		inline static auto get(const oe::graphics::VertexData &t) {
			return t.data.position.y;
		};
	};
} // namespace mapbox::util



class Application
{
public:
	Application();

	void run();

public:
	constexpr static size_t ups = 1;

private:
	static oe::WindowInfo gen_window_info();
	static oe::PolygonRendererInfo gen_renderer_info();

	void on_render(const oe::RenderEvent&);
	void on_update(const oe::UpdateEvent<ups>&);
	void on_resize(const oe::ResizeEvent&);
	void on_button(const oe::MouseButtonEvent&);

private:
	oe::graphics::Window window;


	oe::asset::DefaultShader shader;
	oe::graphics::PolygonRenderer renderer;
	std::array<std::vector<oe::graphics::VertexData>, 1> points;
	int32_t index_count = 0;
	oe::utils::connect_guard cg_on_render;
	oe::utils::connect_guard cg_on_update;
	oe::utils::connect_guard cg_on_resize;
	oe::utils::connect_guard cg_on_button;
};



oe::WindowInfo Application::gen_window_info()
{
	oe::WindowInfo w_info;
	w_info.size = { 900, 600 };
	w_info.main_updatesystem_ups = ups;
	w_info.swap_interval = 0;
	/* w_info. */

	return std::move(w_info);
}

oe::PolygonRendererInfo Application::gen_renderer_info()
{
	oe::PolygonRendererInfo r_info;
	r_info.index_count = 2500;
	r_info.vertex_count = 500;
	/* r_info. */

	return std::move(r_info);
}

Application::Application()
	: window(gen_window_info())
	, shader(oe::polygon_mode::fill)
	, renderer(oe::graphics::make_polygon_renderer(gen_renderer_info()))
{
	cg_on_render.connect<oe::UpdateEvent<ups>, &Application::on_update>(window, this);
	cg_on_update.connect<oe::RenderEvent, &Application::on_render>(window, this);
	cg_on_resize.connect<oe::ResizeEvent, &Application::on_resize>(window, this);
	cg_on_button.connect<oe::MouseButtonEvent, &Application::on_button>(window, this);
}

void Application::run()
{
	window->getGameloop().start();
}

void Application::on_render(const oe::RenderEvent& /* e */)
{
	shader.bind();
	renderer->render(0, index_count);
}

void Application::on_update(const oe::UpdateEvent<ups>& /* e */)
{
	auto& gameloop = window->getGameloop();
	spdlog::info(
		"- frametime: {:3.3f} ms ({} fps)\n- updatetime: {:3.3f} ms ({} ups)",
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getFrametime()).count(),
		gameloop.getAverageFPS(),
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getUpdatetime<ups>()).count(),
		gameloop.getAverageUPS<30>());
}

void Application::on_resize(const oe::ResizeEvent& e)
{
	shader.setProjectionMatrix(glm::ortho<float>(-e.aspect, e.aspect, 1.0f, -1.0f));
	shader.setTexture(false);
}

void Application::on_button(const oe::MouseButtonEvent& e)
{
	if(e.action != oe::actions::press)
		return;

	points[0].push_back({ e.cursor_pos.cursor_worldspace, { 0.0f, 0.0f }, oe::colors::white });
	auto res = mapbox::earcut(points);
	auto* vertics = renderer->getVertices();
	auto* indices = renderer->getIndices();
	if(vertics)
		std::copy(points[0].cbegin(), points[0].cend(), vertics->begin());
	if(indices)
		std::copy(res.cbegin(), res.cend(), indices->begin());

	index_count = res.size();
}



int main()
{
	oe::EngineInfo e_info;
	auto& engine = oe::Engine::getSingleton();
	engine.init(e_info);

	Application app;
	app.run();
}