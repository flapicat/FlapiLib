#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 a_TextureCoords;
layout(location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TextureCoords;
flat out int v_TexIndex;

uniform mat4 u_viewProjectionMatrix;

void main()
{
	v_Color = aColor;
	v_TextureCoords = a_TextureCoords;
	v_TexIndex = int(a_TexIndex);
	gl_Position = u_viewProjectionMatrix * vec4(aPos, 1.0f);
}
