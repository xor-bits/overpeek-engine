#pragma once

#include "../shader.h"



namespace oe {

	class BasicPostprocessShader : public Shader {
	public:
		BasicPostprocessShader();

		enum post_process {	normal = 6, invert_colors = 3, gauss_blur_horizontal = 1, gauss_blur_vertical = 2, edge_detection = 4, trippy = 5 };
		void setPostprocessType(post_process type) {
			setUniform1i("unif_effect", type);
		}

		void time(float _time) {
			setUniform1i("unif_t", _time);
		}
	};

}