#include "gui_manager.h"

#include <engine/graphics/window.h>
#include <engine/graphics/textureManager.h>



namespace oe::gui {
	
	Widget* GUI::m_main_frame = nullptr;
	oe::graphics::Renderer* GUI::m_renderer = nullptr;
	oe::graphics::MultiTextureShader* GUI::m_shader = nullptr;

	bool GUI::m_initialized = false;



	void GUI::init() {
		m_renderer = new oe::graphics::Renderer(oe::graphics::dynamicrender, oe::graphics::staticrender, 1000);
		m_shader = new oe::graphics::MultiTextureShader();
		resize();
		m_initialized = true;
	}

	void GUI::render() {
		m_renderer->begin();
		m_renderer->clear();
		m_main_frame->render(*m_renderer);
		m_shader->bind();
		oe::graphics::TextureManager::bind();
		m_renderer->end();
		m_renderer->render();
	}

	void GUI::resize() {
		resize(oe::graphics::Window::getWidth(), oe::graphics::Window::getHeight());
	}

	void GUI::resize(int width, int height) {
		/*
		    0                  0
		  0 +------------------+ w
		    |                  |
		    |                  |
		    |                  |
		  0 +------------------+ w
		    h                  h
		*/
		glm::mat4 pr_matrix = glm::ortho(0, width, height, 0);
		m_shader->bind();
		m_shader->projectionMatrix(pr_matrix);
	}

}

