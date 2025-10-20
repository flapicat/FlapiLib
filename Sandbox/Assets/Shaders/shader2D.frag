#version 330 core
in vec4 v_Color;
in vec2 v_TexCoord;
flat in int v_TexIndex;
in float v_TailingFactor;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 texColor = texture(u_Textures[v_TexIndex], v_TexCoord * v_TailingFactor);
    FragColor = texColor * v_Color;
}