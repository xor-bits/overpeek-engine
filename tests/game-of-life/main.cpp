#include <engine/include.hpp>
#include <nfd.h>
#include <bitset>



constexpr static std::string_view shader_frag_custom = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out vec4 color;
		
in vec2 shader_uv_frag;
in vec4 shader_color_frag;

uniform sampler2D u_tex;
uniform int u_usetex = 1;
uniform vec4 u_color = vec4(1.0);

uniform vec2 u_tex_size;
uniform vec2 u_tex_offs;
uniform vec2 u_cursor;
uniform int u_gridmode;
uniform float u_brush_size;

void main()
{
	vec2 tex_pos = (gl_FragCoord.xy - u_tex_offs) / u_tex_size;
	vec2 grid_pos = fract(tex_pos);
	vec2 grid_id = floor(tex_pos);
	float grid_d_cursor = length(grid_id - u_cursor);

	float edge =
		int(u_gridmode==0) * smoothstep(0.0, 0.1, min(grid_pos.x, grid_pos.y)) +
		int(u_gridmode==1) * smoothstep(0.0, 0.1, max(grid_pos.x, grid_pos.y)) +
		int(u_gridmode==2) * (1.0f);
	
	color =
		int(u_usetex==1) * (texture(u_tex, shader_uv_frag) * shader_color_frag * u_color) +
		int(u_usetex==0) * (shader_color_frag * u_color);

	color.rgb =
		int(edge <= 0.8) * (vec3(1.0) - color.rgb) +
		int(edge >  0.8) * (color.rgb);

	color.rgb =
		int(grid_d_cursor <= u_brush_size) * vec3(clamp(color.r, 0.2f, 0.8f), clamp(color.g, 0.2f, 0.8f), 1.0f)+
		int(grid_d_cursor >  u_brush_size) * (color.rgb);
}
)";



class Application
{
public:
	Application();

	void run();
	void init();
	void fill(bool state);
	void update(bool force, bool draw);

public:
	constexpr static size_t ups = 10;
	constexpr static size_t w = 90;
	constexpr static size_t h = 70;
	constexpr static size_t cells_total = w * h;
	constexpr static size_t cell_pixel_size = 10;
	constexpr static size_t panel_width = 150;
	constexpr static size_t border = 10;

private:
	oe::WindowInfo gen_window_info();
	oe::TextureInfo gen_texture_info();

	// some events
	void on_render(const oe::RenderEvent&);
	void on_update(const oe::UpdateEvent<ups>&);
	void on_cursor(const oe::CursorPosEvent&);
	void on_button(const oe::MouseButtonEvent&);
	void on_scroll(const oe::ScrollEvent&);
	void on_key(const oe::KeyboardEvent&);

private:
	oe::TextureInfo texture_info;

	template<typename T> using two_dim_array = std::array<std::array<T, w>, h>;
	
	two_dim_array<oe::color> pixels;
	two_dim_array<bool> pixel_states_A; // back and front buffers
	two_dim_array<bool> pixel_states_B;
	two_dim_array<bool>* pixel_states_front;
	two_dim_array<bool>* pixel_states_back;

	bool paused = true;
	size_t cells_live = 0;
	size_t cells_dead = 0;
	size_t step = 0;
	int32_t gridmode = 1;
	float brush_size = 5.0f;

	oe::graphics::Window window;
	oe::asset::DefaultShader shader;
	oe::graphics::Renderer renderer;
	oe::graphics::Texture main_texture;
	oe::gui::GUI gui;
	std::shared_ptr<oe::gui::TextPanel> statistics;
	
	oe::utils::connect_guard cg_on_render;
	oe::utils::connect_guard cg_on_update;
	oe::utils::connect_guard cg_on_cursor;
	oe::utils::connect_guard cg_on_button;
	oe::utils::connect_guard cg_on_scroll;
	oe::utils::connect_guard cg_on_key;
};



oe::WindowInfo Application::gen_window_info()
{
	oe::WindowInfo w_info;
	w_info.size = { w * cell_pixel_size + panel_width, h * cell_pixel_size };
	w_info.main_updatesystem_ups = ups;
	w_info.resizeable = false;
	w_info.title = "game of life";
	w_info.swap_interval = 1;
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
	, shader(oe::shader_stages::fragment_shader, shader_frag_custom)
	, renderer(1)
	, main_texture(texture_info)
	, gui(window)
{
	// events
	cg_on_render.connect<oe::UpdateEvent<ups>, &Application::on_update>(window, this);
	cg_on_update.connect<oe::RenderEvent, &Application::on_render>(window, this);
	cg_on_cursor.connect<oe::CursorPosEvent, &Application::on_cursor>(window, this);
	cg_on_button.connect<oe::MouseButtonEvent, &Application::on_button>(window, this);
	cg_on_scroll.connect<oe::ScrollEvent, &Application::on_scroll>(window, this);
	cg_on_key.connect<oe::KeyboardEvent, &Application::on_key>(window, this);

	// shader
	const glm::vec4 s = { -static_cast<float>(panel_width), static_cast<float>(w * cell_pixel_size), static_cast<float>(h * cell_pixel_size), 0.0f };
	const glm::mat4 pr_matrix = glm::ortho(-static_cast<float>(panel_width), static_cast<float>(w * cell_pixel_size), static_cast<float>(h * cell_pixel_size), 0.0f);
	shader.setProjectionMatrix(pr_matrix);
	shader.getShader()->setUniform("u_tex_size", glm::vec2{ cell_pixel_size, cell_pixel_size });
	shader.getShader()->setUniform("u_tex_offs", glm::vec2{ panel_width, 0.0f });
	shader.getShader()->setUniform("u_gridmode", gridmode);
	shader.getShader()->setUniform("u_brush_size", brush_size);

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
	tpi.text = { fmt::format(UR"(
Game of life
cellular automata.
Press <space> to
pause/unpause.
Press <s> to step
1 update forward.
Press <shift-f> to
ff 10000 updates.
Press <tab> to
cycle gridmode.

Press/hold <lmb>
to paint life.
Press/hold <rmb>
or <shift-lmb>
to paint death.
Scroll to change
the brush size.

Press <r> to
randomize/reset.
Press <f> to
fill with life.
({} cell border)
Press <c> to
fill with death.

Press <ctrl-s>
to save to file.
Press <ctrl-o/l>
to load from file.
)", border), oe::colors::white };
	auto infobox = gui.create(tpi);
	
	tpi.text = { U"", oe::colors::white };
	tpi.widget_info.fract_render_offset = oe::alignments::bottom_left;
	tpi.widget_info.fract_origon_offset = oe::alignments::bottom_left;
	statistics = gui.create(tpi);
}

void Application::init()
{
	step = 0;
	auto& rand = oe::utils::Random::getSingleton();
	for (size_t y = 0; y < h; y++)
		for (size_t x = 0; x < w; x++)
			if (x >= border && x < w - border && y >= border && y < h - border && rand.randomf(0.0f, 1.0f) > 0.5f)
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

void Application::fill(bool state)
{
	step = 0;
	if(!state)
		for (size_t y = 0; y < h; y++)
			for (size_t x = 0; x < w; x++)
				{
					(*pixel_states_front)[y][x] = false;
					pixels[y][x] = oe::colors::white;
				}
	else
		for (size_t y = 0; y < h; y++)
			for (size_t x = 0; x < w; x++)
				if(x >= border && x < w - border && y >= border && y < h - border)
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

void Application::update(bool force, bool draw)
{
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
	
	if(!paused || force)
	{
		step++;
		cells_live = 0;
		cells_dead = 0;
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
				{
					pixels[y][x] = oe::colors::black;
					cells_live++;
				}
				else
				{
					pixels[y][x] = oe::colors::white;
					cells_dead++;
				}
			}
		std::swap(pixel_states_front, pixel_states_back);
		if(draw)
			main_texture->setData(texture_info);
	}

	// update statistics
	if(draw)
	{
		const auto& gl = window->getGameloop();
		statistics->text_panel_info.text = {
			{ (paused ? U"PAUSED\n" : U"\n"), oe::colors::white },
			{ U"Total cells: ", oe::colors::white }, { fmt::format(U"{}\n", cells_total), oe::colors::grey },
			{ U"Live cells: ", oe::colors::white }, { fmt::format(U"{}\n", cells_live), oe::colors::cyan },
			{ U"Dead cells: ", oe::colors::white }, { fmt::format(U"{}\n", cells_dead), oe::colors::light_red },
			{ U"Iteration: ", oe::colors::white }, { fmt::format(U"{}\n", step), oe::colors::lime },
			{ U"FPS: ", oe::colors::white }, { fmt::format(U"{}", gl.getAverageFPS()), oe::colors::white },
			{ U" (", oe::colors::white }, { fmt::format(U"{:.1f} ms)\n", std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gl.getFrametime()).count()), oe::colors::white },
			{ U"UPS: ", oe::colors::white }, { fmt::format(U"{}", gl.getAverageUPS<ups>()), oe::colors::white },
			{ U" (", oe::colors::white }, { fmt::format(U"{:.1f} ms)", std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gl.getUpdatetime<ups>()).count()), oe::colors::white },
		};
	}
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
	update(false, true);
}

void Application::on_cursor(const oe::CursorPosEvent& e)
{
	auto set_tile = [this](int32_t x, int32_t y, bool state)
	{
		if(x < 0 || x >= w || y < 0 || y >= h)
			return;
			
		(*pixel_states_front)[y][x] = state;
		pixels[y][x] = state ? oe::colors::black : oe::colors::white;
	};

	auto loop_tiles_check_dist = [set_tile, this](int32_t x, int32_t y, const glm::ivec2& cursor, bool state)
	{
		for(int32_t yo = -brush_size; yo < brush_size + 1; yo++)
			for(int32_t xo = -brush_size; xo < brush_size + 1; xo++)
				if(glm::length(glm::vec2{ x + xo - cursor.x, y + yo - cursor.y }) <= brush_size)
					set_tile(x + xo, y + yo, state);
	};

	const glm::ivec2 tex_space = static_cast<glm::ivec2>(static_cast<glm::vec2>(e.cursor_windowspace - glm::ivec2{ panel_width, 0 }) / static_cast<float>(cell_pixel_size));
	const glm::ivec2 cursor = glm::ivec2{ tex_space.x, tex_space.y };
	const glm::ivec2 cursor_y_flip = glm::ivec2{ tex_space.x, h - tex_space.y - 1 };
	shader.getShader()->setUniform("u_cursor", static_cast<glm::vec2>(cursor_y_flip));
	
	if(window->getButton(oe::mouse_buttons::button_left))
	{
		loop_tiles_check_dist(tex_space.x, tex_space.y, cursor, true);
		main_texture->setData(texture_info);
	}
	if((window->getButton(oe::mouse_buttons::button_left) && (window->getKey(oe::keys::key_left_shift) || window->getKey(oe::keys::key_right_shift))) || window->getButton(oe::mouse_buttons::button_right))
	{
		loop_tiles_check_dist(tex_space.x, tex_space.y, cursor, false);
		main_texture->setData(texture_info);
	}
}

void Application::on_button(const oe::MouseButtonEvent& e)
{
	on_cursor(e.cursor_pos);
}

void Application::on_scroll(const oe::ScrollEvent& e)
{
	brush_size += e.scroll_delta.x + e.scroll_delta.y;
	brush_size = std::clamp(brush_size, 0.0f, 16.0f);
	
	shader.getShader()->setUniform("u_brush_size", brush_size);
}

void Application::on_key(const oe::KeyboardEvent& e)
{
	if (e.action != oe::actions::release && e.key == oe::keys::key_space)
		paused = !paused;

	if(paused && e.action != oe::actions::release && e.mods != oe::modifiers::control && e.key == oe::keys::key_s)
		update(true, true);

	if(e.action != oe::actions::release && e.mods == oe::modifiers::shift && e.key == oe::keys::key_f)
	{
		for (size_t i = 0; i < 9999; i++)
			update(true, false);
		update(true, true);
	}
		
	if (e.action != oe::actions::release && e.key == oe::keys::key_tab)
	{
		gridmode++;
		gridmode %= 3;

		shader.getShader()->setUniform("u_gridmode", gridmode);
	}
		
	if (e.action != oe::actions::release && e.key == oe::keys::key_r)
		init();

	if (e.action != oe::actions::release && e.mods != oe::modifiers::shift && e.key == oe::keys::key_f)
		fill(true);

	if (e.action != oe::actions::release && e.key == oe::keys::key_c)
		fill(false);

	if (e.action == oe::actions::press && e.mods == oe::modifiers::control && e.key == oe::keys::key_s)
	{
		char* savePath = nullptr;
		nfdresult_t result = NFD_SaveDialog("gol", nullptr, &savePath);

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
		nfdresult_t result = NFD_OpenDialog("gol", nullptr, &savePath);

		if(result == NFD_OKAY)
		{
			std::bitset<w * h> bits;

			oe::utils::FileIO file(savePath);
			auto bytes = file.read<oe::utils::byte_string>();
			std::memcpy(&bits, bytes.data(), std::min(sizeof(bits), bytes.size()));

			free(savePath);

			// load from bitset (there are better ways for this, but this looks simple)
			for (size_t y = 0; y < h; y++)
				for (size_t x = 0; x < w; x++)
				{
					(*pixel_states_front)[y][x] = bits[x + y * w];
					pixels[y][x] = bits[x + y * w] ? oe::colors::black : oe::colors::white;
				}
			main_texture->setData(texture_info);
			paused = true;
			step = 0;
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