#version 330 core

uniform sampler2D uTexture;

in vec2 oTexCoord;
out vec4 color;

void main()
{
    color = texture(uTexture, oTexCoord);
}