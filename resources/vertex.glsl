#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 Color;
out vec2 TexCoord;

void main()
{
    // transform the vertex position
    gl_Position = vec4(aPos, 1.0);

    // transform the texture coordinates
    TexCoord = aTexCoord;
    
    highp int indexX = int(aPos.x);
    highp int indexY = int(aPos.y);

    // forward the vertex color
    Color = vec3(1.0, 1.0, 1.0);
}