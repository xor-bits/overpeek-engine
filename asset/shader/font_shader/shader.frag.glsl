#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out vec4 color;
		
in vec2 shader_uv_frag;
in vec4 shader_color_frag;

uniform sampler2D u_tex;
uniform int u_usetex = 1;
uniform int u_usesdf = 0;
uniform float u_sdf_width = 0.25;
uniform float u_sdf_edge = 0.05;
uniform float u_sdf_outline_width = 0.15;
uniform float u_sdf_outline_edge = 0.05;
uniform vec4 u_sdf_outline_color = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec4 u_color = vec4(1.0);

void main()
{
	color = vec4(1.0);
	
	if (u_usetex != 0 && u_usesdf == 0) // normal font
		color.a *= texture(u_tex, shader_uv_frag).r;

	if(u_usetex != 0 && u_usesdf != 0) // sdf font
	{
		float d = 1.0 - texture(u_tex, shader_uv_frag).r;
		
		float  in_a = smoothstep(u_sdf_width + u_sdf_edge, u_sdf_width, d);
		float out_a = smoothstep(u_sdf_width + u_sdf_outline_width + u_sdf_outline_edge, u_sdf_width + u_sdf_outline_width, d);

		color.a = in_a + (1.0-in_a) * out_a;
		color *= mix(u_sdf_outline_color, shader_color_frag, in_a / color.a);
	}

	color *= u_color;
}