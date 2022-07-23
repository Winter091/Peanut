#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in vec2 a_offset;

uniform mat4 u_viewProjMatrix;

out vec2 v_texCoord;

void main()
{
    gl_Position = u_viewProjMatrix * vec4(a_pos + vec3(a_offset, 1.0), 1.0);
    v_texCoord = a_texCoord;
}
