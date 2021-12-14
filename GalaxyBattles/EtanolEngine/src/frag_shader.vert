#version 330 core

uniform sampler2D uTexture;

in vec2 oTexCoord;
in vec4 color_out;
out vec4 color;

void main()
{
    color = texture(uTexture, oTexCoord)*color_out;
}