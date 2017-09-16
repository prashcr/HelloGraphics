#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void glfwInitialize();
GLFWwindow* createWindow();
void processInput(GLFWwindow* window);
void clearWindow();
unsigned int createTriangleVertexArrayObject();
unsigned int createShaderProgram();
unsigned int createShader(const char* shaderSource, GLenum type);
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
	unsigned int shaderProgram = createShaderProgram();

	int nAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
	std::cout << "Maximum number of vertex attributes " << nAttributes << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		clearWindow();

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // uniform location can be retrieved before using program
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // updating uniform must be done after using program

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

unsigned int createShaderProgram()
{
	const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{

	gl_Position = vec4(aPos, 1.0f);
	ourColor = aColor;
}
)";
	unsigned int vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);

	const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;

void main()
{
	FragColor = vec4(ourColor, 1.0);
}
)";
	unsigned int fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shaderProgram;
}

unsigned int createShader(const char* shaderSource, GLenum type)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
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