#version 450 core

in vec3 v_color;

layout (std140, binding = 0) uniform UniformData
{
    vec4 cColor;
};

out vec4 outColor;

void main()
{
    vec4 color = mix(vec4(v_color, 1.0), cColor, 0.1);
    outColor = color;
}
