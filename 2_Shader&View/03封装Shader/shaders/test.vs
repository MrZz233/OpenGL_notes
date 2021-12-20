#version 330 core   //声明GLSL版本，和OpenGL版本对应
layout (location = 0) in vec3 aPos;    //顶点的坐标数据会从0号位获取

void main()
{
    gl_Position = vec4(aPos, 1.0);   //将aPos转成vec4输出，gl_Position为OpenGL内建变量，用来记录顶点的坐标
}