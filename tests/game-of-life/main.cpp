#include <engine/include.hpp>
#include <nfd.h>
#include <bitset>



class Application
{
public:
	Application();

	void run();
	void init();
	void clear();

public:
	constexpr static size_t ups = 10;
	constexpr static size_t w = 90;
	constexpr static size_t h = 60;
	constexpr static size_t cell_pixel_size = 10;
	constexpr static size_t panel_width = 150;

private:
	oe::WindowInfo gen_window_info();
	oe::TextureInfo gen_texture_info();

	// some events
	void on_render(const oe::RenderEvent&);
	void on_update(const oe::UpdateEvent<ups>&);
	void on_cursor(const oe::CursorPosEvent&);
	void on_button(const oe::MouseButtonEvent&);
	void on_key(const oe::KeyboardEvent&);

private:
	oe::TextureInfo texture_info;

	template<typename T> using two_dim_array = std::array<std::array<T, w>, h>;
	
	two_dim_array<oe::color> pixels;
	two_dim_array<bool> pixel_states_A; // back and front buffers
	two_dim_array<bool> pixel_states_B;
	two_dim_array<bool>* pixel_states_front;
	two_dim_array<bool>* pixel_states_back;

	bool paused = false;

	oe::graphics::Window window;
	oe::asset::DefaultShader shader;
	oe::graphics::Renderer renderer;
	oe::graphics::Texture main_texture;
	oe::gui::GUI gui;
	
	oe::utils::connect_guard cg_on_render;
	oe::utils::connect_guard cg_on_update;
	oe::utils::connect_guard cg_on_cursor;
	oe::utils::connect_guard cg_on_button;
	oe::utils::connect_guard cg_on_key;
};



oe::WindowInfo Application::gen_window_info()
{
	oe::WindowInfo w_info;
	w_info.size = { w * cell_pixel_size + panel_width, h * cell_pixel_size };
	w_info.main_updatesystem_ups = ups;
	w_info.resizeable = false;
	w_info.title = "game of life";
	return std::move(w_info);
}

oe::TextureInfo Application::gen_texture_info()
{
	oe::TextureInfo t_info;
	t_info.empty = false;
	t_info.size_offset = { { w, 0 }, { h, 0 } };
	t_info.filter = oe::texture_filter::nearest;
	t_info.data = pixels[0].data();
	t_info.data_type = oe::TextureInfo::data_types::floats;
	return std::move(t_info);
}

Application::Application()
	: texture_info(gen_texture_info())
	, pixels({ oe::colors::white })
	, pixel_states_A({ false })
	, pixel_states_B({ false })
	, pixel_states_front(&pixel_states_A)
	, pixel_states_back(&pixel_states_B)
	, window(gen_window_info())
	, shader()
	, renderer(1)
	, main_texture(texture_info)
	, gui(window)
{
	// events
	cg_on_render.connect<oe::UpdateEvent<ups>, &Application::on_update>(window, this);
	cg_on_update.connect<oe::RenderEvent, &Application::on_render>(window, this);
	cg_on_cursor.connect<oe::CursorPosEvent, &Application::on_cursor>(window, this);
	cg_on_button.connect<oe::MouseButtonEvent, &Application::on_button>(window, this);
	cg_on_key.connect<oe::KeyboardEvent, &Application::on_key>(window, this);

	// shader
	const glm::vec4 s = { -static_cast<float>(panel_width), static_cast<float>(w * cell_pixel_size), static_cast<float>(h * cell_pixel_size), 0.0f };
	const glm::mat4 pr_matrix = glm::ortho(-static_cast<float>(panel_width), static_cast<float>(w * cell_pixel_size), static_cast<float>(h * cell_pixel_size), 0.0f);
	shader.setProjectionMatrix(pr_matrix);

	// main texture renderer
	std::unique_ptr<oe::graphics::Quad> quad = renderer.create();
	quad->setPosition({ 0.0f, 0.0f });
	quad->setSize({ static_cast<float>(w * cell_pixel_size), static_cast<float>(h * cell_pixel_size) });
	quad->setSprite({ main_texture });
	quad->setRotationAlignment(oe::alignments::top_left);
	renderer.forget(std::move(quad));

	// cell init
	init();

	// gui
	oe::gui::TextPanel::info_t tpi;
	tpi.text = { UR"(
Game of life
cellular automata.
Press <space> to
pause/unpause.

Press/hold <mbl>
to paint life.
Press/hold <mbr>
to paint death.

Press <r> to
randomize/reset.
Press <c> to
fill death.

Press <ctrl-s>
to save to file.
Press <ctrl-o/l>
to load from file.
)", oe::colors::white };
	gui.create(tpi);
}

void Application::init()
{
	constexpr size_t border = 10;
	auto& rand = oe::utils::Random::getSingleton();
	for (size_t y = 0; y < h; y++)
		for (size_t x = 0; x < w; x++)
			if (x > border && x < w - border && y > border && y < h - border && rand.randomf(0.0f, 1.0f) > 0.5f)
			{
				(*pixel_states_front)[y][x] = true;
				pixels[y][x] = oe::colors::black;
			}
			else
			{
				(*pixel_states_front)[y][x] = false;
				pixels[y][x] = oe::colors::white;
			}
	main_texture->setData(texture_info);
}

void Application::clear()
{
	for (size_t y = 0; y < h; y++)
		for (size_t x = 0; x < w; x++)
			{
				(*pixel_states_front)[y][x] = false;
				pixels[y][x] = oe::colors::white;
			}
	main_texture->setData(texture_info);
}


void Application::run()
{
	window->getGameloop().start();
}

void Application::on_render(const oe::RenderEvent& /* e */)
{
	shader.bind();
	renderer.render();

	gui.render();
}

void Application::on_update(const oe::UpdateEvent<ups>& /* e */)
{
	if(paused)
		return;

	auto calculate_neighbours = [this](size_t x, size_t y)
	{
		size_t count = 0;

		const size_t x_min = std::max(x, static_cast<size_t>(1)) - 1;
		const size_t x_max = std::min(x + 1, w - 1);
		const size_t y_min = std::max(y, static_cast<size_t>(1)) - 1;
		const size_t y_max = std::min(y + 1, h - 1);
		
		for (size_t yo = y_min; yo <= y_max; yo++)
			for (size_t xo = x_min; xo <= x_max; xo++)
				count += static_cast<size_t>((*pixel_states_front)[yo][xo] && (xo != x || yo != y));
		
		return count;
	};
	
	for (size_t y = 0; y < h; y++)
		for (size_t x = 0; x < w; x++)
		{
			const size_t neighbours = calculate_neighbours(x, y);
			(*pixel_states_back)[y][x] = (*pixel_states_front)[y][x];

			// Rules
			// 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
			if ((*pixel_states_front)[y][x] && neighbours < 2)
				(*pixel_states_back)[y][x] = false;

			// 2. Any live cell with two or three live neighbours lives on to the next generation.
			else if ((*pixel_states_front)[y][x] && neighbours == 2 && neighbours == 3)
				(*pixel_states_back)[y][x] = true;

			// 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
			else if ((*pixel_states_front)[y][x] && neighbours > 3)
				(*pixel_states_back)[y][x] = false;

			// 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
			else if (!(*pixel_states_front)[y][x] && neighbours == 3)
				(*pixel_states_back)[y][x] = true;

			// Set color
			if((*pixel_states_back)[y][x])
				pixels[y][x] = oe::colors::black;
			else
				pixels[y][x] = oe::colors::white;
		}
	std::swap(pixel_states_front, pixel_states_back);
	
	main_texture->setData(texture_info);
}

void Application::on_cursor(const oe::CursorPosEvent& e)
{
	const glm::ivec2 tex_space = static_cast<glm::ivec2>(static_cast<glm::vec2>(e.cursor_windowspace - glm::ivec2{ panel_width, 0 }) / static_cast<float>(cell_pixel_size));
	if(tex_space.x < 0 || tex_space.x >= w || tex_space.y < 0 || tex_space.y >= h)
		return;
	
	if(window->getButton(oe::mouse_buttons::button_left))
	{
		(*pixel_states_front)[tex_space.y][tex_space.x] = true;
		pixels[tex_space.y][tex_space.x] = oe::colors::black;
		main_texture->setData(texture_info);
	}
	if(window->getButton(oe::mouse_buttons::button_right))
	{
		(*pixel_states_front)[tex_space.y][tex_space.x] = false;
		pixels[tex_space.y][tex_space.x] = oe::colors::white;
		main_texture->setData(texture_info);
	}
}

void Application::on_button(const oe::MouseButtonEvent& e)
{
	on_cursor(e.cursor_pos);
}

void Application::on_key(const oe::KeyboardEvent& e)
{
	if (e.action == oe::actions::press && e.key == oe::keys::key_space)
		paused = !paused;
		
	if (e.action == oe::actions::press && e.key == oe::keys::key_r)
		init();

	if (e.action == oe::actions::press && e.key == oe::keys::key_c)
		clear();

	if (e.action == oe::actions::press && e.mods == oe::modifiers::control && e.key == oe::keys::key_s)
	{
		char* savePath = nullptr;
		nfdresult_t result = NFD_SaveDialog("gof", NULL, &savePath);

		if(result == NFD_OKAY)
		{
			std::bitset<w * h> bits;

			// save to bitset (there are better ways for this, but this looks simple)
			for (size_t y = 0; y < h; y++)
				for (size_t x = 0; x < w; x++)
					bits[x + y * w] = (*pixel_states_front)[y][x];

			oe::utils::FileIO file(savePath);
			file.write(oe::utils::byte_string{ reinterpret_cast<const uint8_t*>(&bits), reinterpret_cast<const uint8_t*>(&bits) + sizeof(bits) });

			free(savePath);
		}
	}

	if (e.action == oe::actions::press && e.mods == oe::modifiers::control && (e.key == oe::keys::key_o || e.key == oe::keys::key_l))
	{
		char* savePath = nullptr;
		nfdresult_t result = NFD_OpenDialog("gof", NULL, &savePath);

		if(result == NFD_OKAY)
		{
			std::bitset<w * h> bits;

			oe::utils::FileIO file(savePath);
			auto bytes = file.read<oe::utils::byte_string>();
			std::memcpy(&bits, bytes.data(), std::min(sizeof(bits), bytes.size()));

			// load from bitset (there are better ways for this, but this looks simple)
			for (size_t y = 0; y < h; y++)
				for (size_t x = 0; x < w; x++)
					(*pixel_states_front)[y][x] = bits[x + y * w];

			free(savePath);
		}
	}
}



int main()
{
	oe::EngineInfo e_info;
	auto& engine = oe::Engine::getSingleton();
	engine.init(e_info);

	Application app;
	app.run();
}