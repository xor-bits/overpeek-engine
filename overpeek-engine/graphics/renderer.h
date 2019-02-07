#pragma once

#include "quadRenderer.h"
#include "fontRenderer.h"

namespace graphics {

	class Renderer {
	private:
		FontRenderer *m_fontRenderer;
		QuadRenderer *m_fontquadRenderer;

		QuadRenderer *m_quadRenderer;

		Window *m_window;


		bool m_quadMapped;
		bool m_fontMapped;

	public:
		Renderer(std::string font, Window *window) {
			m_window = window;
			m_quadRenderer = new QuadRenderer(window);
			m_fontquadRenderer = new QuadRenderer(window);
			m_fontRenderer = new FontRenderer(font, m_fontquadRenderer);
			m_quadMapped = false;
			m_fontMapped = false;
		}

		~Renderer() {
			delete m_fontquadRenderer;
			delete m_fontRenderer;
			delete m_quadRenderer;
		}

		//Submit quad to renderer
		void renderBox(glm::vec2 _pos, glm::vec2 _size, int _id, glm::vec4 _color) {
			if (m_fontMapped) {
				m_fontRenderer->stopRendering();
				m_fontMapped = false;
			}
			if (!m_quadMapped) {
				m_quadRenderer->beginRendering();
				m_quadMapped = true;
			}
			m_quadRenderer->renderBox(_pos, _size, _id, _color);
		}

		//Submit text to renderer
		void renderText(glm::vec2 pos, glm::vec2 size, std::string text, glm::vec3 color, int textOriginX, int textOriginY) {
			if (m_quadMapped) {
				m_quadRenderer->stopRendering();
				m_quadMapped = false;
			}
			if (!m_fontMapped) {
				m_fontRenderer->beginRendering();
				m_fontMapped = true;
			}
			m_fontRenderer->renderText(pos.x, pos.y, size.x, size.y, text, color, textOriginX, textOriginY);
		}

		//Draws all quads and text
		//textbool is location of int (used as boolean) in shader that enables or disables text rendering
		void draw(Shader *shader, std::string textbool, GLint texture) {
			if (m_fontMapped) {
				m_fontRenderer->stopRendering();
				m_fontMapped = false;
			}
			if (m_quadMapped) {
				m_quadRenderer->stopRendering();
				m_quadMapped = false;
			}

			shader->enable();
			shader->setUniform1f(textbool.c_str(), 0);
			m_quadRenderer->draw(texture);
			shader->setUniform1f(textbool.c_str(), 1);
			m_fontRenderer->draw();
		}

	};

}