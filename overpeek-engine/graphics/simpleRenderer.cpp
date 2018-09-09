#include "simpleRenderer.h"


namespace graphics {

	void SimpleRenderer::submit(Renderable *renderable) {
		renderList.push_back(renderable);
	}

	void SimpleRenderer::render() {
		for (Renderable *r : renderList)
		{
			r->render();
		}
		renderList.clear();
	}

	//void SimpleRenderer::renderCircle(glm::mat4 modelMatrix, float x, float y, float r, unsigned int resolution, glm::vec4 color) {
	//
	//	std::vector<glm::vec2> points;
	//	points.push_back(glm::vec2(x, y));
	//	points.push_back(glm::vec2(x, y));
	//	points.push_back(glm::vec2(x, y));
	//
	//	float pi2ByResolution = glm::two_pi<float>() / resolution;
	//
	//	for (int i = 0; i < resolution; i++) {
	//		points[1] = glm::vec2(r * sin(pi2ByResolution * i), r * cos(pi2ByResolution * i));
	//		points[2] = glm::vec2(r * sin(pi2ByResolution * (i + 1)), r * cos(pi2ByResolution * (i+1)));
	//		//renderTriangle(modelMatrix, 0.0f, 0.0f, points, color);
	//	}
	//}

}