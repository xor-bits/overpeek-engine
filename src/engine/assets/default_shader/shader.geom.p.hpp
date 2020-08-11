constexpr char shader_geom_points_gl[] = R"shader(
#version 330
#extension GL_ARB_separate_shader_objects : enable

layout(triangles) in;
layout(points, max_vertices = 3) out;

in vec2 shader_uv_in[];
in vec4 shader_color_in[];

out vec2 shader_uv_out;
out vec4 shader_color_out;

void vertex(int i)
{
    shader_uv_frag = shader_uv_geom[i];
    shader_color_frag = shader_color_geom[i];
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
}

void main()
{
    // no loop
    vertex(0);
    vertex(1);
    vertex(2);

    EndPrimitive();
}
)shader";