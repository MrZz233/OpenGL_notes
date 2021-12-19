#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include "shader.h"
#include <algorithm>

float vertices[] = {
	//---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};
unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

//窗口大小回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//事件输入
void processInput(GLFWwindow* window, float& alpha);

void Ajustment(float& alpha, int operation);

//设置窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {

	//初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//创建glfw窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//调用openGL函数前需要初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("shaders/vertex_01.vs", "shaders/fragment_01.fs");

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	//采用重复环绕
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//采用线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//加载图像并生成纹理
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("images/container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		//##纹理目标  ##多级渐远纹理级别  ##存储格式  ##宽度  ##高度  ##0  ##源图格式  ##源图数据类型  ##图像数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//自动生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture." << std::endl;
	}
	//释放图像内存
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	//采用重复环绕
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//采用线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//加载图像并生成纹理
	data = stbi_load("images/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		//##纹理目标  ##多级渐远纹理级别  ##存储格式  ##宽度  ##高度  ##0  ##源图格式  ##源图数据类型  ##图像数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//自动生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture." << std::endl;
	}
	//释放图像内存
	stbi_image_free(data);
	
	ourShader.use(); // 不要忘记在设置uniform变量之前激活着色器程序！
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
	ourShader.setInt("texture2", 1); // 或者使用着色器类设置
	int alphaLocation = glGetUniformLocation(ourShader.ID, "alpha");
	float alpha = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		//处理事件输入
		processInput(window,alpha);

		//渲染背景
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		ourShader.use();
		//float timeValue = glfwGetTime();
		//alpha = sin(timeValue) / 2.0f + 0.5f;
		std::cout << alpha << std::endl;

		glUniform1f(alphaLocation, alpha);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//交换buffer以及处理事件
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	//释放资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	//销毁glfw窗口
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}

void processInput(GLFWwindow* window,float& alpha)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		Ajustment(alpha, 0);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		Ajustment(alpha, 1);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Ajustment(float& alpha, int operation) {
	if (operation == 0 && alpha <= 1.0f) {
		alpha = std::min(1.0f, alpha + 0.01f);
		std::cout << "up:: " << alpha << std::endl;
	}
	if (operation == 1 && alpha >= 0.0f) {
		alpha = std::max(0.0f, alpha - 0.01f);
	}
}