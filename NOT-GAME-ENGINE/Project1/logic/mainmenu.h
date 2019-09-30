#pragma once

#include "../pch.h"

class MainMenu {

private:
	static int m_active_button;
	static oe::GameLoop *m_loop;
	static oe::Renderer *m_renderer;
	static oe::Shader *m_shader;
	static oe::Shader *m_point_shader;
	static oe::Shader *m_post_shader;

	static float m_t;

	static glm::vec3 m_objpos[1000];
	static glm::vec3 m_objvel[1000];
	static glm::vec4 m_objcol[1000];

public:
	static void init(oe::Renderer *renderer, oe::Shader *shader, oe::Shader *pointshader, oe::Shader *postshader);
	static void deinit();

	static void render(float corrector);
	static void update();

	static void key(int key, int action);
	static void button(int key, int action);

};