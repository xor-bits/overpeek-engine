#version 400 core

in vec4 shader_color;
in vec2 shader_uv;
flat in int shader_id;

layout(location = 0) out vec4 color;

uniform sampler2DArray tex;
uniform bool unif_text;
uniform bool unif_blending;

void main()
{
	if (unif_text) {

		float textureColor = texture(tex, vec3(shader_uv.x, shader_uv.y, shader_id)).x;
		color = vec4(shader_color.x, shader_color.y, shader_color.z, textureColor);
	}
	else {
		vec4 textureColor = texture(tex, vec3(shader_uv, shader_id));
		textureColor.x *= shader_color.x;
		textureColor.y *= shader_color.y;
		textureColor.z *= shader_color.z;
		textureColor.a *= shader_color.a;

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