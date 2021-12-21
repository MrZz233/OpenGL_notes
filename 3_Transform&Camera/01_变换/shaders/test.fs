#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D ourTexture1;   //���������1
uniform sampler2D ourTexture2;   //���������2

void main()
{
    FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.5);
}