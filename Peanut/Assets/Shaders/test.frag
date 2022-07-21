#version 450 core

in vec2 v_tex_coord;

uniform sampler2D u_tex_sampler;

out vec4 outColor;

void main()
{
    outColor = texture(u_tex_sampler, v_tex_coord);
}
