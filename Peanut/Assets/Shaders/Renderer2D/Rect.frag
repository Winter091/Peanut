#version 460 core

in vec4 v_color;
in vec2 v_texCoord;
in flat int v_texIndex;

layout (binding = 0) uniform sampler2D u_texSamplers[16];

out vec4 outColor;

void main()
{
    vec4 color;

    if (v_texIndex < 0) {
        color = v_color;
    } else {
        switch(v_texIndex) {
            case 0:  color = texture(u_texSamplers[0 ], v_texCoord); break;
            case 1:  color = texture(u_texSamplers[1 ], v_texCoord); break;
            case 2:  color = texture(u_texSamplers[2 ], v_texCoord); break;
            case 3:  color = texture(u_texSamplers[3 ], v_texCoord); break;
            case 4:  color = texture(u_texSamplers[4 ], v_texCoord); break;
            case 5:  color = texture(u_texSamplers[5 ], v_texCoord); break;
            case 6:  color = texture(u_texSamplers[6 ], v_texCoord); break;
            case 7:  color = texture(u_texSamplers[7 ], v_texCoord); break;
            case 8:  color = texture(u_texSamplers[8 ], v_texCoord); break;
            case 9:  color = texture(u_texSamplers[9 ], v_texCoord); break;
            case 10: color = texture(u_texSamplers[10], v_texCoord); break;
            case 11: color = texture(u_texSamplers[11], v_texCoord); break;
            case 12: color = texture(u_texSamplers[12], v_texCoord); break;
            case 13: color = texture(u_texSamplers[13], v_texCoord); break;
            case 14: color = texture(u_texSamplers[14], v_texCoord); break;
            case 15: color = texture(u_texSamplers[15], v_texCoord); break;
            default: color = vec4(1.0, 0.0, 1.0, 1.0); break;
        }
    }

    outColor = color;
}
