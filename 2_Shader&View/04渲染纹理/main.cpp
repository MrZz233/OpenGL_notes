#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <iostream>

//��������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float vertices[] = {
	//     ---- λ�� ----     - �������� -
		 0.5f,  0.66f, 0.0f,   1.0f, 1.0f,   // ����
		 0.5f, -0.66f, 0.0f,   1.0f, 0.0f,   // ����
		-0.5f, -0.66f, 0.0f,   0.0f, 0.0f,   // ����	
		-0.5f,  0.66f, 0.0f,   0.0f, 1.0f    // ����
};


//��������
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

	//����texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//������������
	stbi_set_flip_vertically_on_load(true); //���ô�ֱ��ת
	int width, height, nrChannels;
	unsigned char* data = stbi_load("images/img_1.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		//�����������������ɶ༶��Զ����
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	//����VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//����EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//���ö�������֮ǰ�ǵð�VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//��VBO�����������ݼ���������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��EBO�����������ݼ���������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���ö�������
	//0��λ���ö������꣬1��λ���ö�����ɫ��2��λ����uv����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
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