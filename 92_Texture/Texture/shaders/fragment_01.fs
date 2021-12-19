#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alpha;
void main()
{
    vec4 t2 = texture(texture2, TexCoord);
    t2 = t2.a * t2 + (1 - t2.a)* vec4(1.0, 1.0, 1.0, 1.0);
    t2.a = 1.;
    FragColor = mix(texture(texture1, TexCoord), t2, alpha);
}