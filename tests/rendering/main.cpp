#include <engine/include.hpp>
#include <engine/graphics/renderer.hpp>

#include <string>



const oe::graphics::Sprite* sprite; // from pack 0
const oe::graphics::Sprite* sprite_white; // from pack 1
oe::graphics::SpritePack* pack_0;
oe::graphics::SpritePack* pack_1;
oe::graphics::Window window;
oe::assets::DefaultShader* shader;
oe::graphics::Renderer* renderer;
std::array<std::unique_ptr<oe::graphics::Quad>, 2> quads;



void create_scene()
{
	quads[0] = renderer->create();
	quads[0]->setPosition({-0.5f, 0.0f, 0.0f });
	quads[0]->setSize({ 1.0f, 1.0f });
	quads[0]->setRotationAlignment(oe::alignments::center_center);
	quads[0]->setSprite(sprite_white);
	quads[0]->setColor({ 0.0f, 0.0f, 1.0f, 0.5f });
	quads[0]->update();
	
	quads[1] = renderer->create();
	quads[1]->setPosition({ 0.5f, 0.0f, 1.0f });
	quads[1]->setSize({ 0.75f, 0.75f });
	quads[1]->setRotationAlignment(oe::alignments::center_center);
	quads[1]->setSprite(sprite);
	quads[1]->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	quads[1]->update();
}

void resize(const oe::ResizeEvent& event)
{
	window->active_context();
	float aspect = event.aspect;
	glm::mat4 pr = glm::ortho(-aspect, aspect, 1.0f, -1.0f);
	shader->setProjectionMatrix(pr);
	shader->setTexture(true);
}

void render(oe::RenderEvent) 
{
	// modify scene
	float t = oe::utils::Clock::getSingleton().getSessionMillisecond() / 300.0f;
	quads[0]->setRotation(std::sin(t));
	quads[0]->update();
	quads[1]->setRotation(t);
	quads[1]->update();

	// render scene
	shader->bind();
	renderer->sort(oe::graphics::Renderer::sort_by_largest_z_and_texture);
	renderer->render();
}

void update_2(oe::UpdateEvent<2>)
{
	auto& gameloop = window->getGameloop();
	spdlog::info("frametime: {:3.3f} ms ({} fps)", gameloop.getFrametimeMS(), gameloop.getAverageFPS());
}

void keyboard(const oe::KeyboardEvent& event)
{
	if (event.action == oe::actions::press)
	{
		if (event.key == oe::keys::key_escape)
		{
			window->getGameloop().stop();
		}
		else if (event.key == oe::keys::key_enter)
		{
			window->setFullscreen(!window->getFullscreen());
		}
	}
}

void init()
{
	auto& engine = oe::Engine::getSingleton();

	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	engine_info.debug_mode = true;
	engine.init(engine_info);

	// window
	oe::WindowInfo window_info;
	window_info.title = "Renderer";
	window_info.multisamples = 4;
	window_info.size = { 900, 600 };
	window = oe::graphics::Window(window_info);

	// connect events
	window->connect_listener<oe::ResizeEvent, &resize>();
	window->connect_listener<oe::RenderEvent, &render>();
	window->connect_listener<oe::UpdateEvent<2>, &update_2>();
	
	// instance settings
	engine.culling(oe::culling_modes::neither);
	engine.swapInterval(0);
	engine.blending();

	// renderer
	renderer = new oe::graphics::Renderer(100);

	// shader
	shader = new oe::assets::DefaultShader();
	
	// sprites
	auto img = oe::assets::TextureSet::oe_logo_img;
	pack_0 = new oe::graphics::SpritePack();
	pack_1 = new oe::graphics::SpritePack();
	sprite = pack_0->create(img);
	sprite_white = pack_1->emptySprite();
	pack_0->construct();
	pack_1->construct();

	// start
	create_scene();
	window->getGameloop().start(); // print the average frametime 30 times in a second

	// closing
	delete pack_0;
	delete pack_1;
	delete shader;
	delete renderer;
}

int main(int argc, char** argv)
{
	try
	{
		init();
		return 0;
	}
	catch (const std::exception& e)
	{
		spdlog::error(e.what());
		assert(e.what());
		return -1;
	}
}
