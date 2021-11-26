#version 330 core
in vec2 oPos;
out vec4 color;
void main()
{
	color = vec4(oPos.x*0.5 + 0.5, oPos.y*0.5 + 0.5, 1.0, 1.0);
}