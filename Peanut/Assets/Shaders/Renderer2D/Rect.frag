#version 450 core

in vec4 v_color;
in vec2 v_texCoord;
in flat int v_texIndex;

layout (binding = 0) uniform sampler2D u_texSamplers[32];

out vec4 outColor;

void main()
{
    vec4 color;

    if (v_texIndex < 0) {
        color = v_color;
    } else {
        color = texture(u_texSamplers[v_texIndex], v_texCoord);
    }

    outColor = color;
}
