#version 400 core

in vec2 shader_uv;
in vec4 shader_color;
flat in int shader_id;

layout(location = 0) out vec4 color;

uniform sampler2DArray tex;
uniform bool unif_text;
uniform bool unif_blending;

void main()
{
	if (unif_text) {

		float textureColor = texture(tex, vec3(shader_uv.x, shader_uv.y, shader_id)).x;
		color = vec4(1.0, 1.0, 1.0, textureColor);
	}
	else {
		vec4 textureColor = texture(tex, vec3(shader_uv, shader_id));
		if (unif_blending) {
			if (shader_uv.x <= 0.25) {
				textureColor.a *= shader_uv.x * 4;
			}
			else if (shader_uv.x >= 0.75) {
				textureColor.a *= (1 - shader_uv.x) * 4;
			}

			if (shader_uv.y <= 0.25) {
				textureColor.a *= shader_uv.y * 4;
			}
			else if (shader_uv.y >= 0.75) {
				textureColor.a *= (1 - shader_uv.y) * 4;
			}
		}

		color = textureColor;
	}
}