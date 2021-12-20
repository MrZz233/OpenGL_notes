#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//顶点数据
float vertices[] = {
	//	  顶点坐标				 顶点颜色
	-0.5f, -0.5f, 0.0f,		1.0f,  0.0f,  0.0f,		//左下角
	 0.5f, -0.5f, 0.0f,		0.0f,  1.0f,  0.0f,		//右下角
	 0.0f,  0.5f, 0.0f,		0.3f,  0.0f,  1.0f,		//正上方
};						

//box顶点数据
float vertices_box[] = {
	 0.5f,  0.5f,  0.0f,	// 右上角
	 0.5f, -0.5f,  0.0f,	// 右下角
	-0.5f, -0.5f,  0.0f,	// 左下角
	-0.5f,  0.5f,  0.0f,	// 左上角
};
//box顶点索引
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

	//硬编码顶点着色器
	const char* vertexShaderSource = "#version 330 core\n"   //声明GLSL版本，和OpenGL版本对应
		"layout(location = 0) in vec3 aPos;\n"    //顶点的坐标数据会从0号位获取
		"layout(location = 1) in vec3 aColor;\n"  //顶点的颜色数据会从1号位获取
		"out vec4 vertexColor;\n" // 为片段着色器指定一个颜色输出
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0);\n"   //将aPos转成vec4输出，gl_Position为OpenGL内建变量，用来记录顶点的坐标
		"	vertexColor = vec4(aColor, 1.0);\n" //将aClolr转成vec4，输出
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译顶点着色器
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//硬编码片段着色器
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"  //片段着色器必须指定一个vec4类型的输出变量
		"uniform vec4 ourColor;\n" //由于顶点着色器中声明了vertexColor，所以会与之关联，这里的输入来自顶点着色器的输出
		"void main()\n"
		"{\n"
		"	FragColor = ourColor;\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//编译片段着色器
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//链接顶点着色器和片段着色器
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);	//shaderProgram此时就是一个完整的shader了
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	//创建VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//创建EBO
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//设置顶点属性之前记得绑定VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//绑定VBO，将顶点数据加载至缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//绑定EBO，将索引数据加载至缓冲
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设置顶点属性
	//现在第一个参数决定了0号位是读取坐标，1号位是读取颜色
	//第四个参数需要改为6了，因为每两个相邻的坐标（或颜色）之间差了6个float
	//第五个参数表示了一开始读取数据时的偏移量，坐标不需要变动，颜色则是离开头偏移了3个loat
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//绑定着色器
	glUseProgram(shaderProgram);

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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
			
		//交换缓冲
		glfwSwapBuffers(window);
		//监听鼠标和按键发生
		glfwPollEvents();
	}
	//释放资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
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