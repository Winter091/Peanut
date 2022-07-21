#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coord;
layout (location = 2) in vec3 a_offset;

out vec2 v_tex_coord;

void main()
{
    gl_Position = vec4(a_pos + a_offset, 1.0);
    v_tex_coord = a_tex_coord;
}
