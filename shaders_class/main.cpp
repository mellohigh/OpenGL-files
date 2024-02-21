#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/// Resizing flexibility for the render window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/// Settings.
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = R"END(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;
	out vec3 ourColor;
	void main()
	{
		gl_Position = vec4(aPos, 1.0);
		ourColor = aColor;
	}
	)END";

const char* fragmentShaderSource = R"END(
	#version 330 core
	out vec4 FragColor;
	in vec3 ourColor;
	void main()
	{
		FragColor = vec4(ourColor, 1.0f);
	}
	)END";

int main()
{
	/// Initializing and configuring OpenGL 3.3.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/// Creating the render window.
	GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics Library that is Open", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/// Creating all the OpenGL function pointers (GLAD).
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/// Building the vertex shader.
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Checking for errors.
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/// Building the fragment shader.
	/// A fragment in OpenGL is all the data required for OpenGL to render a single pixel.
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Checking for errors.
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/// Creating the final shader program. Linking the vertex and fragment shader above.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Checking for errors.
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/// Setting up vertex data, vertex buffers, element buffers, and vertex attributes.
	float vertices[] = {
		// Positions.			// Colors.
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom right.
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom left.
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top.
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Bind the VAOs first, then bind and set VBO(s)/EBO(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// WIREFRAME mode: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/// The "render loop," which allows for the program to run from start to finish.
	while (!glfwWindowShouldClose(window))
	{
		// Input.
		// Processing all input; check whether relevant keys are pressed.
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		// Render.
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing.
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Check/call events and swap the buffers here.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/// Cleans and terminates all of GLFW’s resources that were allocated.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}