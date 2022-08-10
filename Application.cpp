//inlcude GLFW and glad to load openGL and tell it what to do
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//vertex shader which defines how the primitive shapes should be divided
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"	gl_position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";

//defines the color of the primitive shape im drawing
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0);\n"
"}\0";

int main() {
	//initialize GLFW
	glfwInit();

	//let glfw what version of the GLFW is being used
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//vertices used to draw the triangle
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //lower left
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //lower right
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, //upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f //innder down
	};

	GLuint indices[] = {
		0, 3, 5, //lower left triangle
		3, 2, 4, //lower right triangle
		5, 4, 1 //upper triangle
	};

	//set the profile for what functions and profiles should be used for openGL in this case
	//only the modern functions are being used
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window object and then do some errror checking to make sure a window was created
	//succesfully
	GLFWwindow* window = glfwCreateWindow(800, 800, "openGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//make the window object the active window
	glfwMakeContextCurrent(window);

	//load glad for configuration of opengl
	gladLoadGL();

	//set the viewport width for OpenGL
	glViewport(0, 0, 800, 800);

	//create a vertex shader using the vertexshader written above then compile it to give
	//it to the GPU
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//create a fragment shader using the fragmentshader written above and then compile it 
	//to give to the GPU
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//create shader program to put in the buffer
	GLuint shaderProgram = glCreateProgram();

	//attach the shaders to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//link that program
	glLinkProgram(shaderProgram);

	//delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//create two buffers VAO and VBO
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind vertex array to variables 
	glBindVertexArray(VAO);

	//bind the data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//let them know the data in the buffers and attributes around them then bind them
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//bind the other buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//clear the color buffer and then swap frame buffers color with out color we provided
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	//set the main loop for the window until its closed
	while (!glfwWindowShouldClose(window)) {
		//poll all the events that happen in the window so that we can make the window responsive
		//and do stuff with the polled data
		glfwPollEvents();

		//draw a triangle primitive shape given the two buffers with the vertices
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	}

	//delete the buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	//at end of program destroy the window and then terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}