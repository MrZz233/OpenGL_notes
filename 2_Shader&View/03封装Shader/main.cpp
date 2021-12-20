#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/shader.h"
#include <iostream>

//��������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//��������
float vertices[] = {
	//	  ��������				 ������ɫ
	-0.5f, -0.5f, 0.0f,		1.0f,  0.0f,  0.0f,		//���½�
	 0.5f, -0.5f, 0.0f,		0.0f,  1.0f,  0.0f,		//���½�
	 0.0f,  0.5f, 0.0f,		0.3f,  0.0f,  1.0f,		//���Ϸ�
};						

//box��������
float vertices_box[] = {
	 0.5f,  0.5f,  0.0f,	// ���Ͻ�
	 0.5f, -0.5f,  0.0f,	// ���½�
	-0.5f, -0.5f,  0.0f,	// ���½�
	-0.5f,  0.5f,  0.0f,	// ���Ͻ�
};
//box��������
unsigned int indices[] = { // ע��������0��ʼ! 
	0, 1, 3, // ��һ��������
	1, 2, 3  // �ڶ���������
};

int main()
{
	//��ʼ��glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//����Mac OS����������һ��
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//����glfw����
	GLFWwindow* window = glfwCreateWindow(800, 600, "01_Triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//�󶨵�ǰ�����ĺʹ���
	glfwMakeContextCurrent(window);
	
	//����OpenGL����֮ǰ��Ҫ�ȳ�ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//���ô��ڸı�Ļص�����������ע�͵��۲�仯
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader ourShader("shaders/test.vs", "shaders/test.fs");

	//����VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//����EBO
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//���ö�������֮ǰ�ǵð�VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//��VBO�����������ݼ���������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��EBO�����������ݼ���������
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���ö�������
	//���ڵ�һ������������0��λ�Ƕ�ȡ���꣬1��λ�Ƕ�ȡ��ɫ
	//���ĸ�������Ҫ��Ϊ6�ˣ���Ϊÿ�������ڵ����꣨����ɫ��֮�����6��float
	//�����������ʾ��һ��ʼ��ȡ����ʱ��ƫ���������겻��Ҫ�䶯����ɫ�����뿪ͷƫ����3��loat
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//���û�ͼģʽΪ�߿�ģʽ,����������ʱ��ֻ���ߡ�
	//��һ������˵�������������涼Ҫ����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//��ѭ��
	while (!glfwWindowShouldClose(window))
	{
		//�������¼�
		processInput(window);	
		//�����Ļ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��Ⱦͼ��
		ourShader.use();
		glBindVertexArray(VAO);
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(ourShader.ID, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
			
		//��������
		glfwSwapBuffers(window);
		//�������Ͱ�������
		glfwPollEvents();
	}
	//�ͷ���Դ
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(ourShader.ID);
	//����glfw
	glfwTerminate();
	return 0;
}

//���ڸı亯��
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//����������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}