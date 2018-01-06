#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <direct.h>

void processInput(GLFWwindow *window);
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

const unsigned int WindowWidth = 800;
const unsigned int WindowHeight = 600;
int main()
{
	//设置Opengl版本信息
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

	//创建渲染窗口
	GLFWwindow * window =glfwCreateWindow(WindowWidth, WindowHeight, "CoordinateSystems", NULL, NULL);
	if (window==NULL )
	{
		std::cout << "Create Window failed!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//设置Opengl渲染环境
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//开启深度测试
	glEnable(GL_DEPTH_TEST);

	//编译链接着色器
	char buffer[256];
	_getcwd(buffer, 256);
	std::string workDir = std::move(std::string(buffer));
	std::string vShaderPath = workDir + "/data/shaders/coordinatesystems/coordinatesystems.vs";
	std::string fShaderPath = workDir + "/data/shaders/coordinatesystems/coordinatesystems.fs";
	Shader ourShader(vShaderPath.c_str(), fShaderPath.c_str());

	//定义顶点坐标和相应的纹理坐标
	float vertices[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//定义缓存对象和数组对象句柄
	unsigned int VBO, VAO;

	//获取数组对象
	glGenVertexArrays(1, &VAO);
	//获取缓存对象
	glGenBuffers(1, &VBO);

	//绑定为当前数组对象
	glBindVertexArray(VAO);
	
	//绑定为当前缓存对象
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//绑定定点数据和文理数据到VBO索引所对应的缓存对象
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//向顶点着色器传递顶点数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//激活0通道，对应顶点数据
	glEnableVertexAttribArray(0);
	
	//向顶点着色器传递纹理坐标数据
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//激活1通道，对应纹理坐标数据
	glEnableVertexAttribArray(1);
	
	//定义纹理句柄
	unsigned int texture1, texture2;
	
	//获取纹理句柄
	glGenTextures(1, &texture1);
	//绑定为当前纹理句柄，并设置纹理类型为二维纹理
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	//设置S轴的环绕方式为重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//设置T轴的环绕方式为重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//设置缩小的过滤方式为线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//设置放大的过滤方式为线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	//加载箱子纹理数据
	unsigned char *data = stbi_load("data/images/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	 
	glGenTextures(1, &texture2); 
	glBindTexture(GL_TEXTURE_2D, texture2);
	 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	data = stbi_load("data/images/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	//使用当前着色器
	ourShader.use();
	//向着色器传递纹理数据
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	
	//监听纹理数据并完成渲染
	while (!glfwWindowShouldClose(window))
	{
		//处理输入消息
		processInput(window);
		
		//设置要清除的背景颜色为用户自定颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//清除颜色缓存和深度缓存
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
			 
		//激活纹理通道0
		glActiveTexture(GL_TEXTURE0);
		//将纹理对象texture1绑定到纹理通道0
		glBindTexture(GL_TEXTURE_2D, texture1);
		//激活纹理通道1
		glActiveTexture(GL_TEXTURE1);
		//将纹理对象texture2绑定到文理通1
		glBindTexture(GL_TEXTURE_2D, texture2);
			 
		//设置使用当前着色器
		ourShader.use();
		
		//定义视图矩阵和投影矩阵
		glm::mat4 view;
		glm::mat4 projection;
		//获取投诉投影矩阵
		projection = glm::perspective(glm::radians(45.0f), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
		//获取视图矩阵
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		
		//向着色器传递投uniform变量影矩阵和视图矩阵
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
	
		//绑定数组对象VAO
		glBindVertexArray(VAO);
		//循环绘制物体
		for (unsigned int i = 0; i < 10; i++)
		{
			//定义模型矩阵
			glm::mat4 model;
			//平移物体到指定位置
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			//选择物体
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//通过uniform变量设模型矩阵
			ourShader.setMat4("model", model);
			//绘制物体
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//交换opengl前后缓存
		glfwSwapBuffers(window);
		//检测事件
		glfwPollEvents();
	}
	
	//删除数组对象
	glDeleteVertexArrays(1, &VAO);
	//删除缓存对象
	glDeleteBuffers(1, &VBO);
	//结束glfw窗口
	glfwTerminate();

	return 0;
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
