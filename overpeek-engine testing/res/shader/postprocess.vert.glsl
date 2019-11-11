#version 400 core
layout(location = 0) in vec3 vertex_pos;

out vec2 UV;

void main(){
	gl_Position =  vec4(vertex_pos, 1);
	UV = (vertex_pos.xy+vec2(1.0, 1.0)) /2.0;
}