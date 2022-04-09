#version 460 core
out vec4 FragColor;

in vec3 Color;
in vec2 Texcoord;

uniform sampler2D u_smiley;

void main()
{
    FragColor = texture(u_smiley, Texcoord) * vec4(Color, 1.0);
}

