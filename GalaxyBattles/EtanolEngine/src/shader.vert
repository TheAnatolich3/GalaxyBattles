#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
out vec2 oTexCoord;
uniform vec2 uScreenSize;
uniform mat3 uTransform;
void main()
{
    vec2 pos = (uTransform * vec3(position, 1.0)).xy;
    vec2 scaledPos = pos / uScreenSize;
    scaledPos.y = 1.0 - scaledPos.y;
    scaledPos = scaledPos * 2.0 - vec2(1.0);
    oTexCoord = texCoord;
    gl_Position = vec4(scaledPos.x, scaledPos.y, 1.0, 1.0);
}