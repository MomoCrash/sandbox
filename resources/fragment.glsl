#version 330 core

out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;

uniform sampler2D inCells;

void main()
{
    // multiply it by the color
    FragColor = texture(inCells, TexCoord);
}