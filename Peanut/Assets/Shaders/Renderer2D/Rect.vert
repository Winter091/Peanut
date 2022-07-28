#version 450 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in vec4 a_color;
layout (location = 3) in int a_texIndex;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_viewProjMatrix;
};

out vec4 v_color;
out vec2 v_texCoord;
out flat int v_texIndex;

void main()
{
    gl_Position = u_viewProjMatrix * vec4(a_pos, 0.0, 1.0);
    v_color = a_color;
    v_texCoord = a_texCoord;
    v_texIndex = a_texIndex;
}
