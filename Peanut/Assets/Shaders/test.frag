#version 450 core

in vec2 v_texCoord;

layout (binding = 0) uniform sampler2D u_texSampler;

out vec4 outColor;

void main()
{
    outColor = texture(u_texSampler, v_texCoord);
}
