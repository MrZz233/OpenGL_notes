#version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // ��OpenGL����������趨�������

void main()
{
    FragColor = ourColor;
}