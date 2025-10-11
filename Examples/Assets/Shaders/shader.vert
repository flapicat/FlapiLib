#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 a_TextureCoords;

out vec4 v_Color;
out vec2 v_TextureCoords;

uniform mat4 u_viewProjectionMatrix;

void main()
{
	gl_Position = u_viewProjectionMatrix * vec4(aPos, 1.0f);
	v_Color = aColor;
	v_TextureCoords = a_TextureCoords;
}
