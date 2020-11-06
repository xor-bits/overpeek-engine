#include <engine/include.hpp>
#include <engine/graphics/renderer.hpp>

#include <string>



const oe::graphics::Sprite* sprite; // from pack 0
const oe::graphics::Sprite* sprite_white; // from pack 1
oe::graphics::SpritePack* pack_0;
oe::graphics::SpritePack* pack_1;
oe::graphics::Window window;
oe::asset::DefaultShader* shader;
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
	
	quads[1] = renderer->create();
	quads[1]->setPosition({ 0.5f, 0.0f, 1.0f });
	quads[1]->setSize({ 0.75f, 0.75f });
	quads[1]->setRotationAlignment(oe::alignments::center_center);
	quads[1]->setSprite(sprite);
	quads[1]->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
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
	quads[1]->setRotation(t);

	// render scene
	shader->bind();
	renderer->render();
}

void update_2(oe::UpdateEvent<2>)
{
	auto& gameloop = window->getGameloop();
	spdlog::info("- frametime: {:3.3f} ms ({} fps)\n- updatetime: {:3.3f} ms ({} ups)",
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getFrametime()).count(),
		gameloop.getAverageFPS(),
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getUpdatetime<30>()).count(),
		gameloop.getAverageUPS<30>());
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

void init(int argc, char** argv)
{
	auto& engine = oe::Engine::getSingleton();

	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
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
	shader = new oe::asset::DefaultShader();
	
	// sprites
	auto img = oe::asset::TextureSet::oe_logo_img;
	pack_0 = new oe::graphics::SpritePack();
	pack_1 = new oe::graphics::SpritePack();
	sprite = pack_0->create(img);
	sprite_white = pack_1->emptySprite();
	pack_0->construct();
	pack_1->construct();

	// auto close ctest after 2 seconds
	std::thread ctest_close_thread;
	for (size_t i = 0; i < argc; i++)
		if(std::strcmp(argv[i], "--ctest") != 0)
		{
			ctest_close_thread = std::thread([](){
				// test for 2 seconds
				std::this_thread::sleep_for(std::chrono::seconds(2));

				// then stop the gameloop
				window->getGameloop().stop();
			});
			break;
		}

	// start
	create_scene();
	window->getGameloop().start(); // print the average frametime 30 times in a second

	if(ctest_close_thread.joinable())
		ctest_close_thread.join();

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
		init(argc, argv);
		return 0;
	}
	catch (const std::exception& e)
	{
		spdlog::error(e.what());
		assert(e.what());
		return -1;
	}
}
