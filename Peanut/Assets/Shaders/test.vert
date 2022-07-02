#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec3 a_offset;

out vec3 v_color;

void main()
{
    gl_Position = vec4(a_pos + a_offset, 1.0);
    v_color = a_color;
}
