#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out vec4 color;
		
in vec2 shader_uv_frag;
in vec4 shader_color_frag;

uniform ivec2 u_offset;
uniform ivec2 u_viewport;
uniform int u_usetex = 1;
uniform float u_wheel_width = 0.1;
uniform bool u_hsv = true;


vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    float aspect = float(u_viewport.x) / float(u_viewport.y);
	float wheel_width = u_wheel_width * 0.5;
    
	vec2 point = (gl_FragCoord.xy + u_offset) / vec2(u_viewport.xy);
    point.x *= aspect;

	vec4 hsv = vec4(1.0);
	if (u_hsv)
	{
		float dir = atan(point.y, point.x);
		hsv = vec4(hsv2rgb(vec3(dir / 6.28, 1.0, 1.0)), 1.0);
	}

	color = vec4(0.0);
	float distance_from_middle = length(point);
    distance_from_middle = 1.0 - abs(0.49 - wheel_width - distance_from_middle) + wheel_width;
	
	if(distance_from_middle > 1.0f)
		color = hsv * distance_from_middle;
}