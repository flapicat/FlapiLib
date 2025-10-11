#version 330 core
out vec4 FragColor;

in vec4 v_Color;
in vec2 v_TextureCoords;

uniform sampler2D u_texture;

void main()
{
    FragColor = texture(u_texture, v_TextureCoords) * v_Color;
}