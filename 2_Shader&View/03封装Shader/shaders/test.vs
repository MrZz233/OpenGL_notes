#version 330 core   //����GLSL�汾����OpenGL�汾��Ӧ
layout (location = 0) in vec3 aPos;    //������������ݻ��0��λ��ȡ

void main()
{
    gl_Position = vec4(aPos, 1.0);   //��aPosת��vec4�����gl_PositionΪOpenGL�ڽ�������������¼���������
}