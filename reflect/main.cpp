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
unsigned int loadCubeTexture(const std::vector<std::string> cubeImagePath);
void readBinaryStl(const std::string &path, std::vector<std::array<float,3>> &triPoints);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
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
 
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

	Shader cubeShader((workDir + "/data/shaders/cubemap/cubemap.vs").c_str(),
		(workDir + "/data/shaders/cubemap/cubemap.fs").c_str());

	float cubeVertices[] = {
		// positions        
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	 
    Shader shader((workDir+"/data/shaders/reflect/reflect.vs").c_str(), 
		(workDir + "/data/shaders/reflect/reflect.fs").c_str());
    

	std::vector<std::array<float, 3>> spherePoints;
	readBinaryStl(workDir +"/data/stl/sphere.stl",spherePoints);
	int sz = spherePoints.size();
	// sphere VAO
	unsigned int sphereVAO, sphereVBO;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sz*3, & (spherePoints[0][0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  
	std::vector<std::string> paths; 
	paths.push_back(workDir + "/data/images/skybox/right.jpg");
	paths.push_back(workDir + "/data/images/skybox/left.jpg");
	paths.push_back(workDir + "/data/images/skybox/top.jpg");
	paths.push_back(workDir + "/data/images/skybox/bottom.jpg");
	paths.push_back(workDir + "/data/images/skybox/front.jpg");
	paths.push_back(workDir + "/data/images/skybox/back.jpg");

	unsigned int cubeTexture =loadCubeTexture(paths); 
    shader.use();
    shader.setInt("cubeTexture", 0);
	glDisable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    { 
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		 
        processInput(window);
		  
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		cubeShader.use();
		glm::mat4 model;
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection); 
		// cubes
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
		cubeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

        shader.use(); 
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
		shader.setVec3("eyePos", camera.Position); 
        glBindVertexArray(sphereVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture); 
		//spheres
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -2.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, sz);
      
      
        glfwSwapBuffers(window);
        glfwPollEvents();
    } 
    glDeleteVertexArrays(1, &sphereVAO);
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}
 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  // camera.ProcessMouseScroll(yoffset);
}
 
unsigned int loadCubeTexture(const std::vector<std::string> cubeImagePath)
{
    unsigned int textureCube;
    glGenTextures(1, &textureCube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube);
	for (int i=0; i<cubeImagePath.size(); i++ )
	{
		const char * path = cubeImagePath.at(i).c_str();
		int width, height, nrComponents;
		unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);  
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return textureCube;
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

void readBinaryStl(const std::string &path, std::vector<std::array<float, 3>> &triPoints)
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


	const char* p = buffer;
	char name[80];
	int i, j;
	memcpy(name, p, 80);
	p += 80;
	int unTriangles = cpyint(p);
	for (i = 0; i < unTriangles; i++)
	{
		p += 12;
		for (j = 0; j < 3; j++)
			triPoints.push_back({ cpyfloat(p), cpyfloat(p), cpyfloat(p) });
		p += 2;
	} 
	free(buffer);
}
