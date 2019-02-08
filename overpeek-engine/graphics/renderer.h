#pragma once

#include "quadRenderer.h"
#include "pointRenderer.h"
#include "fontRenderer.h"

namespace graphics {

	class Renderer {
	private:
		//Font
		FontRenderer *m_fontRenderer;

		//Quad
		QuadRenderer *m_quadRenderer;

		//Point
		PointRenderer *m_pointRenderer;

		//Dependecies
		Window *m_window;


		//Framebuffers
		GLuint m_framebuffer1;
		GLuint m_frametexture1;
		GLuint m_framebuffer2;
		GLuint m_frametexture2;
		VertexArray *m_ScreenVAO;
		Buffer *m_ScreenVBO;

		bool m_quadMapped;
		bool m_fontMapped;
		bool m_pointMapped;

	public:
		Renderer(std::string font, Window *window) {
			m_window = window;
			m_fontRenderer = new FontRenderer(font, new QuadRenderer(window));
			m_quadRenderer = new QuadRenderer(window);
			m_pointRenderer = new PointRenderer(window);
			m_quadMapped = false;
			m_fontMapped = false;
			m_pointMapped = false;

			//Post processing

			// The fullscreen quad's VBO
			GLfloat g_quad_vertex_buffer_data[] = {
				-1.0f, -1.0f,
				 1.0f, -1.0f,
				 1.0f,  1.0f,
				-1.0f, -1.0f,
				 1.0f,  1.0f,
				-1.0f,  1.0f
			};

			m_ScreenVAO = new VertexArray();
			m_ScreenVBO = new Buffer(g_quad_vertex_buffer_data, 12, 2, sizeof(GLfloat), GL_STATIC_DRAW);
			m_ScreenVAO->addBuffer(m_ScreenVBO, 0);
			m_ScreenVAO->unbind();

			//Render buffer object
			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_window->getWidth(), m_window->getHeight());

			//First framebuffer and frametexture
			//buffer
			glGenFramebuffers(1, &m_framebuffer1);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);

			//texture
			glGenTextures(1, &m_frametexture1);
			glBindTexture(GL_TEXTURE_2D, m_frametexture1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_window->getWidth(), m_window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_frametexture1, 0);

			//attach
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) tools::Logger::error("Framebuffer is not complete!");

			//Second framebuffer and frametexture
			//buffer
			glGenFramebuffers(1, &m_framebuffer2);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);

			//texture
			glGenTextures(1, &m_frametexture2);
			glBindTexture(GL_TEXTURE_2D, m_frametexture2);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_window->getWidth(), m_window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_frametexture2, 0);

			//attach
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) tools::Logger::error("Framebuffer is not complete!");

			//Unbinding
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		~Renderer() {
			delete m_fontRenderer;
			delete m_quadRenderer;
			delete m_pointRenderer;
		}

		//Submit quad to renderer
		void renderBox(glm::vec2 _pos, glm::vec2 _size, int _id, glm::vec4 _color) {
			if (m_fontMapped) {
				m_fontRenderer->stopRendering();
				m_fontMapped = false;
			}
			if (m_pointMapped) {
				m_pointRenderer->stopRendering();
				m_pointMapped = false;
			}
			if (!m_quadMapped) {
				m_quadRenderer->beginRendering();
				m_quadMapped = true;
			}
			m_quadRenderer->renderBox(_pos, _size, _id, _color);
		}

		//Submit point to renderer
		void renderPoint(glm::vec2 pos, glm::vec2 size, int id, glm::vec4 color) {
			if (m_quadMapped) {
				m_quadRenderer->stopRendering();
				m_quadMapped = false;
			}
			if (m_fontMapped) {
				m_fontRenderer->stopRendering();
				m_fontMapped = false;
			}
			if (!m_pointMapped) {
				m_pointRenderer->beginRendering();
				m_pointMapped = true;
			}
			m_pointRenderer->renderPoint(pos, size, id, color);
		}

		//Submit text to renderer
		void renderText(glm::vec2 pos, glm::vec2 size, std::string text, glm::vec3 color, int textOriginX, int textOriginY) {
			if (m_quadMapped) {
				m_quadRenderer->stopRendering();
				m_quadMapped = false;
			}
			if (m_pointMapped) {
				m_pointRenderer->stopRendering();
				m_pointMapped = false;
			}
			if (!m_fontMapped) {
				m_fontRenderer->beginRendering();
				m_fontMapped = true;
			}
			m_fontRenderer->renderText(pos.x, pos.y, size.x, size.y, text, color, textOriginX, textOriginY);
		}

		//Draws all quads and text
		//textbool is location of int (used as boolean) in shader that enables or disables text rendering
		void draw(Shader *shader, Shader * pointshader, std::string textbool, GLint texture) {
			if (m_fontMapped) {
				m_fontRenderer->stopRendering();
				m_fontMapped = false;
			}
			if (m_quadMapped) {
				m_quadRenderer->stopRendering();
				m_quadMapped = false;
			}
			if (m_pointMapped) {
				m_pointRenderer->stopRendering();
				m_pointMapped = false;
			}

			shader->enable();
			shader->setUniform1f(textbool.c_str(), 0);
			m_quadRenderer->draw(texture);
			
			pointshader->enable();
			pointshader->setUniform1f(textbool.c_str(), 0);
			m_pointRenderer->draw(texture);

			shader->enable();
			shader->setUniform1f(textbool.c_str(), 1);
			m_fontRenderer->draw();
		}

		//Draws all quads and text to specified framebuffer at index
		void drawToFramebuffer(Shader *shader, Shader * pointshader, std::string textbool, GLint texture, bool first_framebuffer) {
			if (first_framebuffer) glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
			else glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			draw(shader, pointshader, textbool, texture);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
		}
		
		//Draws (first or second) framebuffer to screen
		void drawFramebuffer(Shader *postshader, std::string texture_sampler, bool first_framebuffer) {
			//Render quad to whole screen
			glClear(GL_COLOR_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);

			postshader->enable();
			postshader->setUniform1i(texture_sampler.c_str(), 0);
			glActiveTexture(GL_TEXTURE0);

			if (first_framebuffer) glBindTexture(GL_TEXTURE_2D, m_frametexture1);
			else glBindTexture(GL_TEXTURE_2D, m_frametexture2);


			m_ScreenVAO->bind();
			m_ScreenVBO->bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);
		}
		
		//Draws framebuffer to another framebuffer
		void drawFramebufferToFramebuffer(Shader *postshader, std::string texture_sampler, bool first_framebuffer) {
			if (first_framebuffer) glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
			else glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);

			drawFramebuffer(postshader, texture_sampler, !first_framebuffer);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	};

}