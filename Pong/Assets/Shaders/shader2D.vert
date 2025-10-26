#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TailingFactor;

out vec4 v_Color;
out vec2 v_TexCoord;
flat out int v_TexIndex;
out float v_TailingFactor;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = int(a_TexIndex);
    v_TailingFactor = a_TailingFactor;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}