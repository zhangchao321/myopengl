#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <vector>
#include <array>

#include <iostream>
#include <direct.h>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window); 
void readBinaryStl(const std::string &path,
	std::vector<std::array<float, 6>> &triPoints,
	std::array<float,6> &bds);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BlinnPhong", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	char buffer[256];
	_getcwd(buffer, 256);
	std::string workDir = std::move(std::string(buffer)); 
	 
	Shader shader((workDir + "/data/shaders/blinnphong/blinnphong.vs").c_str(),
		(workDir + "/data/shaders/blinnphong/blinnphong.fs").c_str());


	std::vector<std::array<float, 6>> points;
	std::array<float, 6> bds;
	readBinaryStl(workDir + "/data/stl/cylinder.stl", points,bds);
	std::array<float, 6> center = {(bds[0]+bds[1])*0.5f,(bds[2] + bds[3])*0.5f, (bds[4] + bds[5])*0.5f,};
	camera =Camera(glm::vec3(center[0], center[1], bds[5]*4.0f));
	int sz = points.size();
	// sphere VAO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sz * 6, &(points[0][0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3));

	shader.use();
	shader.setVec3("lightPos", glm::vec3(center[0], center[0], bds[5]));
	 
	shader.setVec3("specularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("diffuseColor", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("ambientColor", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("matrialColor", glm::vec3(0.5f, 0.3f, 0.5f));

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		 

		shader.use();
		glm::mat4 model;
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("eyePos", camera.Position);
		glBindVertexArray(VAO); 

		model = glm::translate(model, glm::vec3(0,0,-10));
		shader.setMat4("model", model);

		glm::mat4 normalMat = glm::inverse(model);
		normalMat = glm::transpose(normalMat);
		shader.setMat4("modelNormalMat", normalMat);
		glDrawArrays(GL_TRIANGLES, 0, sz);
		 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos; 
	camera.ProcessMouseMovement(xoffset, yoffset,false);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	 camera.ProcessMouseScroll(yoffset);
}

float cpyfloat(const char*& p)
{
	float cpy;
	memcpy((char*)&cpy, p, 4);
	p += 4;
	return cpy;
}

int cpyint(const char*& p)
{
	int cpy;
	memcpy((char*)&cpy, p, 4);
	p += 4;
	return cpy;
}

void readBinaryStl(const std::string &path,
	std::vector<std::array<float, 6>> &triPoints,
	std::array<float, 6> &bds)
{
	FILE * pFile;
	long lSize;
	char* buffer;
	size_t result;

	fopen_s(&pFile, path.c_str(), "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		return;
	}

	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		return;
	}

	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		return;
	}
	fclose(pFile);

	float xMin, xMax, yMin, yMax, zMin, zMax;
	xMin = xMax = yMin = yMax = zMin = zMax = 0.0f;
	float x, y, z, nx, ny, nz;
	const char* p = buffer;
	char name[80];
	int i, j;
	memcpy(name, p, 80);
	p += 80;
	int unTriangles = cpyint(p);
	bool first = true;
	for (i = 0; i < unTriangles; i++)
	{
		nx = cpyfloat(p);
		ny = cpyfloat(p);
		nz = cpyfloat(p); 
		for (j = 0; j < 3; j++)
		{
			x = cpyfloat(p);
			y = cpyfloat(p);
			z = cpyfloat(p);
			triPoints.push_back({ x,y,z, nx,ny,nz});
			if (first)
			{
				xMin = xMax = x;
				yMin = yMax = y;
				zMin = zMax = z;
				first = false;
			}
			if (x<xMin)
			{
				xMin = x;
			}
			if (x>xMax)
			{
				xMax = x;
			}
			
			if (y < yMin)
			{
				yMin = y;
			}
			if (y > yMax)
			{
				yMax = y;
			}

			if (z < zMin)
			{
				zMin = z;
			}
			if (z > zMax)
			{
				zMax = z;
			}
		}
		p += 2;
	}
	free(buffer);
	bds = {xMin,xMax,yMin,yMax,zMin,zMax};
}
