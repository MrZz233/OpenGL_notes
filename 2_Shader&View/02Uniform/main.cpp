#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

	//Ӳ���붥����ɫ��
	const char* vertexShaderSource = "#version 330 core\n"   //����GLSL�汾����OpenGL�汾��Ӧ
		"layout(location = 0) in vec3 aPos;\n"    //������������ݻ��0��λ��ȡ
		"layout(location = 1) in vec3 aColor;\n"  //�������ɫ���ݻ��1��λ��ȡ
		"out vec4 vertexColor;\n" // ΪƬ����ɫ��ָ��һ����ɫ���
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0);\n"   //��aPosת��vec4�����gl_PositionΪOpenGL�ڽ�������������¼���������
		"	vertexColor = vec4(aColor, 1.0);\n" //��aClolrת��vec4�����
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//���붥����ɫ��
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//Ӳ����Ƭ����ɫ��
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"  //Ƭ����ɫ������ָ��һ��vec4���͵��������
		"uniform vec4 ourColor;\n" //���ڶ�����ɫ����������vertexColor�����Ի���֮������������������Զ�����ɫ�������
		"void main()\n"
		"{\n"
		"	FragColor = ourColor;\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//����Ƭ����ɫ��
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//���Ӷ�����ɫ����Ƭ����ɫ��
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);	//shaderProgram��ʱ����һ��������shader��
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//����ɫ��
	glUseProgram(shaderProgram);

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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
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
	glDeleteProgram(shaderProgram);
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