#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>

#include <iostream>

void glfwInitialize();
GLFWwindow* createWindow();
void processInput(GLFWwindow* window);
void clearWindow();
unsigned int createTriangleVertexArrayObject();
void onSetFramebufferSize(GLFWwindow* window, int width, int height);

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;
const unsigned int triangleVtxCount = 3;

int main()
{
	glfwInitialize();

	GLFWwindow* window = createWindow();
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	unsigned int vertexArrayObject = createTriangleVertexArrayObject();
	Shader myShader("shader.vert", "shader.frag");

	int nAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
	std::cout << "Maximum number of vertex attributes " << nAttributes << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		clearWindow();

		myShader.use();

		glBindVertexArray(vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, triangleVtxCount);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void glfwInitialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWwindow* createWindow()
{
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, onSetFramebufferSize);
	return window;
}

void clearWindow()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

unsigned int createTriangleVertexArrayObject()
{
	const float vertices[] = {
		// positions        // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // top
	};

	unsigned int vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	unsigned int vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, triangleVtxCount, GL_FLOAT, GL_FALSE, triangleVtxCount * 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, triangleVtxCount, GL_FLOAT, GL_FALSE, triangleVtxCount * 2 * sizeof(float), (void*)(triangleVtxCount * sizeof(float)));
	glEnableVertexAttribArray(1);

	return vertexArrayObject;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void onSetFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}