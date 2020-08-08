constexpr char shader_geom_lines_gl[] = R"shader(
#version 320
#extension GL_ARB_separate_shader_objects : enable

layout(triangles) in;
layout(line_strip, max_vertices = 3) out;

in vec2 shader_uv_geom;
in vec4 shader_color_geom;

out vec2 shader_uv_frag;
out vec4 shader_color_frag;

void main(void)
{
    int i;
    for (i = 0; i < gl_in.length(); i++)
    {
        shader_uv_frag = shader_uv_geom;
        shader_color_frag = shader_color_geom;

        gl_Position = gl_in[i].gl_Position;

        EmitVertex();
    }
    EndPrimitive();
}
)shader";