#version 330 core
layout(location = 0) in vec3 aPos;

out vec4 v_Color;

uniform mat4 u_viewProjectionMatrix;

void main()
{
	gl_Position = u_viewProjectionMatrix * vec4(aPos, 1.0f);
	v_Color = vec4(aPos,1.0f);
}
