#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <iostream>

//函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float vertices[] = {
	//     ---- 位置 ----     - 纹理坐标 -
		 0.5f,  0.66f, 0.0f,   1.0f, 1.0f,   // 右上
		 0.5f, -0.66f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.66f, 0.0f,   0.0f, 0.0f,   // 左下	
		-0.5f,  0.66f, 0.0f,   0.0f, 1.0f    // 左上
};


//顶点索引
unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

int main()
{
	//初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//若是Mac OS加上下面这一句
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//创建glfw窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "01_Triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//绑定当前上下文和窗口
	glfwMakeContextCurrent(window);
	
	//调用OpenGL函数之前需要先初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//设置窗口改变的回调函数，可以注释掉观察变化
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader ourShader("shaders/test.vs", "shaders/test.fs");

	//创建texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//加载纹理数据
	stbi_set_flip_vertically_on_load(true); //设置垂直翻转
	int width, height, nrChannels;
	unsigned char* data = stbi_load("images/img_1.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		//加载至缓冲区并生成多级渐远纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	//创建VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//创建EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//设置顶点属性之前记得绑定VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//绑定VBO，将顶点数据加载至缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//绑定EBO，将索引数据加载至缓冲
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设置顶点属性
	//0号位设置顶点坐标，1号位设置顶点颜色，2号位设置uv坐标
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//设置绘图模式为线框模式,绘制三角形时，只画边。
	//第一个参数说明三角形正反面都要绘制
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//主循环
	while (!glfwWindowShouldClose(window))
	{
		//处理按键事件
		processInput(window);	
		//清空屏幕
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//渲染图形
		ourShader.use();
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
		//交换缓冲
		glfwSwapBuffers(window);
		//监听鼠标和按键发生
		glfwPollEvents();
	}
	//释放资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(ourShader.ID);
	//结束glfw
	glfwTerminate();
	return 0;
}

//窗口改变函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//按键处理函数
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}