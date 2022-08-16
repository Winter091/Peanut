#version 450 core

layout (location = 0) in uint a_vertexIndex;
layout (location = 1) in mat4 a_modelMatrix;
layout (location = 5) in uint a_color;
layout (location = 6) in int a_texIndex;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_viewProjMatrix;
};

out vec4 v_color;
out vec2 v_texCoord;
out flat int v_texIndex;

const vec4 positions[4] = vec4[4](
    vec4(0.0, 0.0, 0.0, 1.0),
    vec4(0.0, 1.0, 0.0, 1.0),
    vec4(1.0, 1.0, 0.0, 1.0),
    vec4(1.0, 0.0, 0.0, 1.0)
);

const vec2 texCoords[4] = vec2[4](
    vec2(0.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0),
    vec2(1.0, 0.0)
);

void main()
{
    gl_Position = u_viewProjMatrix * a_modelMatrix * positions[a_vertexIndex];

    vec4 color;
    color.r = (a_color >> 0)  & 0xFF;
    color.g = (a_color >> 8)  & 0xFF;
    color.b = (a_color >> 16) & 0xFF;
    color.a = (a_color >> 32) & 0xFF;
    v_color = color;
    
    v_texCoord = texCoords[a_vertexIndex];
    v_texIndex = a_texIndex;
}
