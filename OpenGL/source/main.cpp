#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image/stb_image.h"

#include <iostream>
	
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

void framebuffer_size_callback(GLFWwindow*, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {

	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MrX", nullptr, nullptr);
	if (window == NULL) {
		std::cout << "Window failed to initialize" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glPolygonMode(GL_POLYGON_MODE, GL_LINES);

	std::cout << glGetString(GL_VERSION) << std::endl;

	float vertices[] = {
		//positions				/texture coords
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 0.0f, //bottom left
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, //bottom right
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f, //top right
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f  //top left
	};

	VertexArray va;
	va.Bind();

	VertexBuffer vb(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	va.AddAttribute(vb, layout);

	unsigned int indices[] = {
		0, 1, 3,
		1, 3, 2
	};

	IndexBuffer ib(indices, sizeof(indices));
	ib.Bind();

	Shader shader("source/resources/shaders/My.Shader");
	shader.Bind();

	Texture texture("source/resources/textures/sammurray.png");

	glm::mat4 model(1.0f);

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.1f));

	//glm::mat4 proj(1.0f);
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		texture.Bind(0);
		shader.Uniform1i("tex", 0);
		shader.UniformMatrix4fv("model", glm::value_ptr(model));
		shader.UniformMatrix4fv("view", glm::value_ptr(view));
		shader.UniformMatrix4fv("proj", glm::value_ptr(proj));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}