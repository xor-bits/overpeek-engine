constexpr char shader_geom_lines_gl[] = R"shader(
#version 330
#extension GL_ARB_separate_shader_objects : enable

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

in vec2 shader_uv_geom[];
in vec4 shader_color_geom[];

out vec2 shader_uv_frag;
out vec4 shader_color_frag;

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
    vertex(0);

    EndPrimitive();
}
)shader";