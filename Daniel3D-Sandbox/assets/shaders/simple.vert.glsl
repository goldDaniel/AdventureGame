#version 460 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texcoord;

out vec3 Color;
out vec2 Texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	Color = a_color;
	Texcoord = a_texcoord;
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
}

