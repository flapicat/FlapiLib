#version 330 core
out vec4 FragColor;

in vec4 v_Color;
in vec2 v_TextureCoords;
flat in int v_TexIndex;

uniform sampler2D u_textures[8];

void main()
{
    int index = v_TexIndex;
    vec4 texColor = texture(u_textures[index], v_TextureCoords);
    FragColor = texColor * v_Color;
}
