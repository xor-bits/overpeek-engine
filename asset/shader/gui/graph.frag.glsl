#version 140
#extension GL_ARB_explicit_attrib_location : enable

#define LINE_W (3)

layout(location = 0) out vec4 color;
		
in vec2 shader_uv_frag;
in vec4 shader_color_frag;

uniform ivec2 u_viewport;
uniform float u_graph_data[256];
uniform int u_graph_size; /* should always be in range 0-255 */
uniform float u_line_w = 1.0;

float d_line(in vec2 pixel, in vec2 line_a, in vec2 line_b)
{
	float a = distance(line_a, pixel);
  	float b = distance(line_b, pixel);
  	float c = distance(line_a, line_b);

  	if ( a >= c || b >=  c ) return 0.0;

  	float p = (a + b + c) * 0.5;
  	float h = 2.0 / c * sqrt(p * (p - a) * (p - b) * (p - c));
  	return mix(1.0, 0.0, smoothstep(0.5 * u_line_w, 1.5 * u_line_w, h));
}

void main()
{
	// point in screen
	ivec2 pixel = ivec2(gl_FragCoord.xy);
	vec2 pixelf = vec2(pixel) / u_viewport.xy;

	// points of the line
	int left_i = int(floor(pixelf.x * u_graph_size));
	int right_i = left_i + 1;
	ivec2 left = ivec2(left_i * u_viewport.x / u_graph_size, u_graph_data[left_i] * u_viewport.y);
	ivec2 right = ivec2(right_i * u_viewport.x / u_graph_size, u_graph_data[right_i] * u_viewport.y);

	// distance to the line
	float d = d_line(vec2(pixel), vec2(left), vec2(right));

	// if bellow the line
	float t_left_right = mod(pixelf.x * u_graph_size, 1.0);
	int h = int(mix(u_graph_data[left_i] * u_viewport.y, u_graph_data[right_i] * u_viewport.y, t_left_right));

	// final color alpha
	float alpha = max(d, (h >= pixel.y ? 0.5 : 0.0));

	color = vec4(1.0, 1.0, 1.0, alpha);
}