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

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

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
		0.5f, 0.5f, 0.0f, // Top right
		0.5f, -0.5f, 0.0f, // Bottom right
		-0.5f, -0.5f, 0.0f, // Bottom left
		-0.5f, 0.5f, 0.0f // Top left
	};
	unsigned int indices[] = {  // Note that we start from 0.
		0, 1, 3,  // First triangle.
		1, 2, 3   // Second triangle.
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Bind the VAOs first, then bind and set VBO(s)/EBO(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing.
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// FOR MULTIPLE TRIANGLES: glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check/call events and swap the buffers here.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/// Cleans and terminates all of GLFW’s resources that were allocated.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
/* 5.8 Exercises
 To really get a good grasp of the concepts discussed a few exercises were set up.
 It is advised to work through them before continuing to the next subject to make sure
 you get a good grasp of what’s going on. */
	//! 1. Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data.Solution: / src / 1.getting_started / 2.3.hello_triangle_exercise1 /.
	//! 2. Now create the same 2 triangles using two different VAOs and VBOs for their data. Solution : / src / 1.getting_started / 2.4.hello_triangle_exercise2 /.
	//! 3. Create two shader programs where the second program uses a different fragment shader that outputs the color yellow; draw both triangles again where one outputs the color yellow. Solution: / src / 1.getting_started / 2.5.hello_triangle_exercise3 /.