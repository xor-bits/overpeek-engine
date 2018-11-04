#version 400 core

in vec2 uv;
flat in int id;

out vec4 color;

uniform sampler2DArray tex;
uniform bool unif_text;

void main()
{
	if (unif_text) {

		float textureColor = texture(tex, vec3(uv.x, uv.y, id)).x;
		color = vec4(1.0, 1.0, 1.0, textureColor);
	}
	else {
		color = texture(tex, vec3(uv, id));
	}
}