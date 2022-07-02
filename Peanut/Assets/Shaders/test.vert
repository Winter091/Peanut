#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 2) in vec3 a_offset;

void main()
{
    gl_Position = vec4(a_pos + a_offset, 1.0);
}
