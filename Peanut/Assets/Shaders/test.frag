#version 450 core

in vec3 v_color;

out vec4 outColor;

void main()
{
    outColor = vec4(v_color, 1.0);
}
